use crate::prelude::*;
use super::{ TreeAction, Tree, Node, ProgressResult };
use anyhow::Result;

#[derive(Debug)]
pub enum StackElem {
    FuncParams,
    FuncReturn,
    ExpReturn(String),
    Returned,
    VarBegun,
    VarCanExit,
    Main,
}

#[derive(PartialEq)]
pub enum StackType {
    FuncParams,
    FuncReturn,
    ExpReturn,
    Returned,
    VarBegun,
    VarCanExit,
    Main,
}

impl StackType {
    fn to_stack_elem(&self, token: Token) -> Result<StackElem> {
        match self {
            Self::FuncParams => Ok(StackElem::FuncParams),
            Self::FuncReturn => Ok(StackElem::FuncReturn),
            Self::ExpReturn => {
                if let Token::FuncIdent(val) = token {
                    Ok(StackElem::ExpReturn(val))
                } else {
                    bail!(format!("Cannot build an ExpReturn stack element from token: {:?}", token))
                }
            }
            Self::Returned => Ok(StackElem::Returned),
            Self::VarBegun => Ok(StackElem::VarBegun),
            Self::VarCanExit => Ok(StackElem::VarCanExit),
            Self::Main => Ok(StackElem::Main),
        }
    }
}

impl StackElem {
    fn to_stacktype(&self) -> StackType {
        match self {
            Self::FuncParams => StackType::FuncParams,
            Self::FuncReturn => StackType::FuncReturn,
            Self::ExpReturn(_) => StackType::ExpReturn,
            Self::Returned => StackType::Returned,
            Self::VarBegun => StackType::VarBegun,
            Self::VarCanExit => StackType::VarCanExit,
            Self::Main => StackType::Main,
        }
    }
}

#[derive(Clone)]
pub enum Mode {
    Normal,
    Expr(Box<ExprHelper>),
}

pub enum Action {
    Tree(TreeAction),
    SwitchMode(Mode),
}

pub struct Transition {
    pub to_state: &'static str,
    pub action: Option<Vec<Action>>,
    pub input: TokenProto,
    pub cmp_stack: Option<StackType>,
    pub pop_stack: Option<StackType>,
    pub push_stack: Option<StackType>,
}

#[derive(Clone)]
pub struct ExprHelper {
    pub output: Vec<Token>,
    pub operators: Vec<Token>,
    pub arg_count: Vec<usize>,
    pub arg_count_final: Vec<usize>,
}

impl ExprHelper {
    pub fn new() -> Self {
        Self {
            output: Vec::new(),
            operators: Vec::new(),
            arg_count: Vec::new(),
            arg_count_final: Vec::new(),
        }
    }
}

pub struct PDA {
    pub state: &'static str,
    mode: Mode,
    states: HashMap<&'static str, Vec<Transition>>,
    pub stack: Vec<StackElem>,
}

impl PDA {
    pub fn new() -> Self {
        Self {
            state: "q_start",
            mode: Mode::Normal,
            states: HashMap::new(),
            stack: Vec::new(),
        }
    }

    pub fn add_state(&mut self, name: &'static str, transitions: Vec<Transition>) {
        self.states.insert(name, transitions);
    }

    pub fn transition(&mut self, input: &Token, tree: &mut Tree) -> Result<()> {

        match &mut self.mode {
            Mode::Normal => {
                let transition = {
                    let transitions = self.states.get(self.state).ok_or(
                        anyhow!(format!("State `{}` not found in states table", self.state))
                    )?;

                    let next_stack = self.stack.last();
                    let mut transition_ret: Option<&Transition> = None;

                    for transition in transitions {
                        if transition.input == input.proto() {
                            let stack_matches = match transition.pop_stack.as_ref() {
                                Some(transition_pop) => next_stack
                                    .as_ref()
                                    .map_or(false, |stack_elem| stack_elem.to_stacktype() == *transition_pop),
                                None => true,
                            };

                            if stack_matches {
                                if transition.cmp_stack.is_some() {
                                    let mut cmp_succeed = false;

                                    if matches!(transition.cmp_stack, Some(StackType::ExpReturn)) {
                                        if let Token::Identifier(val) = input {
                                            if let Some(StackElem::ExpReturn(exp_return)) = self.stack.last() {
                                                if exp_return == val {
                                                    cmp_succeed = true;
                                                }
                                            } else {
                                                bail!("Stack contains no elements");
                                            }
                                        }
                                    }

                                    if !cmp_succeed {
                                        continue;
                                    }
                                }

                                transition_ret = Some(transition);
                                break;
                            }
                        }
                    }

                    transition_ret
                }
                .ok_or(anyhow!("No suitable transition was found."))?;

                let popped = {
                    if transition.pop_stack.is_some() {
                        self.stack.pop()
                    } else {
                        None
                    }
                };

                if let Some(actions) = &transition.action {
                    for action in actions {
                        match action {
                            Action::SwitchMode(mode) => self.mode = mode.clone(),
                            Action::Tree(action) => {
                                match action {
                                    TreeAction::AddNode(node) => {
                                        if let Some(value) = node {
                                            tree.add_node(value.clone());
                                        } else {
                                            unimplemented!("add node without defined value");
                                        }
                                    }
                                    TreeAction::AppendChild(node) => {
                                        if let Some(value) = node {
                                            unimplemented!("append child with defined value");
                                        } else {
                                            tree.append_child(Node::Val(input.clone()))?;
                                        }
                                    }
                                    TreeAction::GoUp => {
                                        tree.go_up()?
                                    }
                                }
                            },
                        }
                    }
                }

                if let Some(stack_type) = &transition.push_stack {
                    if let Some(popped) = popped.filter(|p| *stack_type == p.to_stacktype()) {
                        self.stack.push(popped);
                    } else {
                        self.stack.push(stack_type.to_stack_elem(input.clone())?);
                    }
                }

                self.state = transition.to_state;
            }
            Mode::Expr(helper) => {
                if let ProgressResult::Ok = Self::parse_expression(helper, input, tree)? {
                    self.mode = Mode::Normal;
                }
            }
        }

        Ok(())
    }
}