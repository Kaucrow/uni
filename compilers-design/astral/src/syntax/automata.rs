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
    Expr(Box<ExprHelper>),
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

#[derive(Clone)]
pub struct ExprHelper {
    pub output: Vec<String>,
    pub operators: Vec<String>,
    pub arg_count: Vec<usize>,
    pub ident_found: Option<String>,
}

impl ExprHelper {
    pub fn new() -> Self {
        Self {
            output: Vec::new(),
            operators: Vec::new(),
            arg_count: Vec::new(),
            ident_found: None,
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
        
        match &mut self.mode {
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
            Mode::Expr(helper) => {
                Self::parse_expression(helper, input, ';')?;
            }
        }

        Ok(())
    }

    pub fn parse_expression(helper: &mut Box<ExprHelper>, input: &(TokenType, String), eof: char) -> Result<()> {
        // Determines operator precedence
        fn precedence(op: &str) -> i32 {
            match op {
                "+" | "-" => 1, // Lowest precedence
                "*" | "/" => 2, // Higher precedence
                _ => 0,         // Default case
            }
        }
        let token_type = &input.0;
        let value = &input.1;
        
        let output = &mut helper.output;
        let operators = &mut helper.operators;
        let arg_count = &mut helper.arg_count;

        match token_type {
            TokenType::Number => output.push(value.clone()),

            TokenType::Identifier => helper.ident_found = Some(value.clone()),

            TokenType::Symbol if value == "(" => {
                // If a function identifier is stored
                if let Some(function) = &helper.ident_found {
                    operators.push(function.clone());
                    operators.push(value.clone());
                    arg_count.push(1);  // Initialize argument count
                } else {
                    operators.push(value.clone());
                }
            }

            TokenType::Symbol if value == "," => {
                while let Some(top) = operators.last() {
                    // Pop until we find a left parenthesis
                    if !matches!(top.as_str(), "(") {
                        output.push(operators.pop().unwrap());
                    } else {
                        break;
                    }
                }
                // Increase argument count
                if let Some(last) = arg_count.last_mut() {
                    *last += 1;
                }
            }

            TokenType::Symbol if matches!(value.as_str(), "+" | "-" | "*" | "/") => {
                while let Some(top) = operators.last() {
                    if matches!(top.as_str(), "+" | "-" | "*" | "/") {
                        if precedence(top) >= precedence(value) {
                            output.push(operators.pop().unwrap());  // Pop higher precedence ops
                        } else {
                            break;
                        }
                    } else {
                        break;
                    }
                }
                operators.push(value.clone());
            }

            // Right parenthesis triggers popping until left parenthesis is found
            TokenType::Symbol if value == ")" => {
                while let Some(top) = operators.last() {
                    if !matches!(top.as_str(), "(") {
                        output.push(operators.pop().unwrap());
                    } else {
                        break;
                    }
                }
                operators.pop();    // Remove '(' from stack

                // If a function is on top, pop it to output
                todo!()
                //if let Some(TokenType::Identifier) = operators
            }

            _ => todo!()
        }

        helper.ident_found = None;

        Ok(())
    }
}