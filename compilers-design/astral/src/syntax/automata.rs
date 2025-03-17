use std::hash::Hash;

use crate::prelude::*;
use super::{ TreeAction, Tree, Node };
use anyhow::Result;

#[derive(Debug)]
pub enum StackElem {
    // Normal
    FuncParams,
    FuncReturn,
    ExpReturn(String),
    Returned,
    VarBegun,
    VarCanExit,
    Declarations,
    Main,
    If,
    Else,
    // Expression parsing
    LParen,
    Func,
    // Builtin functions
    Writeln,
}

#[derive(PartialEq)]
pub enum StackType {
    // Normal
    FuncParams,
    FuncReturn,
    ExpReturn,
    Returned,
    VarBegun,
    VarCanExit,
    Declarations,
    Main,
    If,
    Else,
    // Expression parsing
    LParen,
    Func,
    // Builtin functions
    Writeln,
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
            Self::Declarations => Ok(StackElem::Declarations),
            Self::Main => Ok(StackElem::Main),
            Self::If => Ok(StackElem::If),
            Self::Else => Ok(StackElem::Else),
            Self::LParen => Ok(StackElem::LParen),
            Self::Func => Ok(StackElem::Func),
            Self::Writeln => Ok(StackElem::Writeln),
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
            Self::Declarations => StackType::Declarations,
            Self::Main => StackType::Main,
            Self::If => StackType::If,
            Self::Else => StackType::Else,
            Self::LParen => StackType::LParen,
            Self::Func => StackType::Func,
            Self::Writeln => StackType::Writeln,
        }
    }
}

#[derive(Debug, Clone)]
pub enum Mode {
    Normal,
    Expr(Box<ExprHelper>),
}

#[derive(Debug, PartialEq, Eq, Hash)]
pub enum ModeProto {
    Normal,
    Expr,
}

impl Mode {
    fn proto(&self) -> ModeProto {
        match self {
            Self::Normal => ModeProto::Normal,
            Self::Expr(_) => ModeProto::Expr,
        }
    }
}

pub enum Action {
    Tree(TreeAction),
    SwitchMode(Mode),
    ParseExpr(fn(&mut Box<ExprHelper>, &Token, &mut Tree) -> Result<()>)
}

pub struct Transition {
    pub to_state: &'static str,
    pub action: Option<Vec<Action>>,
    pub input: TokenProto,
    pub cmp_stack: Option<StackType>,
    pub pop_stack: Option<StackType>,
    pub push_stack: Option<StackType>,
}

#[derive(Clone, PartialEq, Eq, Hash, Debug)]
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
    pub mode: Mode,
    states: HashMap<ModeProto, HashMap<&'static str, Vec<Transition>>>,
    pub stack: Vec<StackElem>,
}

impl PDA {
    pub fn new() -> Self {
        let mut pda = Self {
            state: "q_start",
            mode: Mode::Normal,
            states: HashMap::from([
                (ModeProto::Normal, HashMap::new()),
                (ModeProto::Expr, HashMap::new()),
            ]),
            stack: Vec::new(),
        };
        pda.build();
        pda
    }

    pub fn add_state(&mut self, name: &'static str, mode: ModeProto, transitions: Vec<Transition>) {
        let states = self.states.get_mut(&mode).expect(format!("Mode is not in PDA states: {:?}", mode).as_str());
        states.insert(name, transitions);
    }

    pub fn transition(&mut self, input: &Token, tree: &mut Tree) -> Result<()> {
        let states = self.states.get(&self.mode.proto()).expect(format!("Mode is not in PDA states: {:?}", self.mode).as_str());

        let transition = {
            let transitions = states.get(self.state).ok_or(
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
                                if let Some(_) = node {
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
                    Action::ParseExpr(parse_fn) => {
                        if let Mode::Expr(helper) = &mut self.mode {
                            parse_fn(helper, input, tree)?;
                        } else {
                            bail!("Cannot trigger a ParseExpr action on a mode other than Expr mode")
                        }
                    }
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

        Ok(())
    }
}