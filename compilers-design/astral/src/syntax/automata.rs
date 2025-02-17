use crate::prelude::*;
use super::{ TreeAction, Tree, Node };
use anyhow::Result;

#[derive(Debug)]
pub enum StackElem {
    Func(String),
    FuncParams,
    FuncReturn,
    Proc(String),
    VarBegun,
    VarCanExit,
    Main,
}

#[derive(PartialEq)]
pub enum StackType {
    Func,
    FuncParams,
    FuncReturn,
    Proc,
    VarBegun,
    VarCanExit,
    Main,
}

impl StackType {
    fn to_stack_elem(&self, input: &(TokenType, String)) -> StackElem {
        match self {
            Self::Func => StackElem::Func(input.1.clone()),
            Self::FuncParams => StackElem::FuncParams,
            Self::FuncReturn => StackElem::FuncReturn,
            Self::Proc => StackElem::Proc(input.1.clone()),
            Self::VarBegun => StackElem::VarBegun,
            Self::VarCanExit => StackElem::VarCanExit,
            Self::Main => StackElem::Main,
        }
    }
}

impl StackElem {
    fn to_stacktype(&self) -> StackType {
        match self {
            StackElem::Func(_) => StackType::Func,
            StackElem::FuncParams => StackType::FuncParams,
            StackElem::FuncReturn => StackType::FuncReturn,
            StackElem::Proc(_) => StackType::Proc,
            StackElem::VarBegun => StackType::VarBegun,
            StackElem::VarCanExit => StackType::VarCanExit,
            StackElem::Main => StackType::Main,
        }
    }
}

#[derive(Clone)]
pub enum Mode {
    Normal,
    Expr,
}

pub enum Action {
    Tree(TreeAction),
    SwitchMode(Mode),
}

pub struct Transition {
    pub to_state: &'static str,
    pub action: Option<Vec<Action>>,
    pub input: (TokenType, Option<&'static str>),
    pub cmp_stack: bool,
    pub pop_stack: Option<StackType>,
    pub push_stack: Option<StackType>,
}

impl Transition {
    pub fn new(
        to_state: &'static str,
        action: Option<Vec<Action>>,
        input: (TokenType, Option<&'static str>),
        cmp_stack: bool,
        pop_stack: Option<StackType>,
        push_stack: Option<StackType>,
    ) -> Self
    {
        Self {
            to_state,
            action,
            input,
            cmp_stack,
            pop_stack,
            push_stack,
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

    pub fn transition(&mut self, input: &(TokenType, String), tree: &mut Tree) -> Result<()> {
        
        match self.mode {
            Mode::Normal => {
                let transition = {
                    let transitions = self.states.get(self.state).ok_or(
                        anyhow!(format!("State `{}` not found in states table", self.state))
                    )?;

                    let next_stack = self.stack.last();
                    let mut transition_ret: Option<&Transition> = None;

                    for transition in transitions {
                        if transition.input.0 == input.0
                        && (transition.input.1.is_none() || transition.input.1 == Some(input.1.as_str())) {
                            let stack_matches = match transition.pop_stack.as_ref() {
                                Some(transition_pop) => next_stack
                                    .as_ref()
                                    .map_or(false, |stack_elem| stack_elem.to_stacktype() == *transition_pop),
                                None => true,
                            };

                            if stack_matches {
                                if transition.cmp_stack {
                                    match next_stack {
                                        Some(StackElem::Func(name)) => if *name != input.1 {
                                            bail!("Bad function return.");
                                        },
                                        Some(StackElem::Proc(name)) => if *name != input.1 {
                                            bail!("Bad process return.");
                                        },
                                        _ => bail!("A stack comparison does not make sense in this context."),
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
                                            tree.append_child(Node::Val(input.1.clone()))?;
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
                        self.stack.push(stack_type.to_stack_elem(input));
                    }
                }

                self.state = transition.to_state;
            }
            Mode::Expr => {
                bail!("expr mode is not yet implemented lol")
            }
        }

        Ok(())
    }
}