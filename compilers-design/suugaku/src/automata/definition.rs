use crate::prelude::*;
use anyhow::Result;

pub enum StackElem {
    LParen,
    Func,
}

#[derive(PartialEq, Eq)]
pub enum StackType {
    LParen,
    Func,
}

impl StackElem {
    fn to_stacktype(&self) -> StackType {
        match self {
            Self::LParen => StackType::LParen,
            Self::Func => StackType::Func,
        }
    }
}

impl StackType {
    fn to_stack_elem(&self, _: Token) -> Result<StackElem> {
        match self {
            Self::LParen => Ok(StackElem::LParen),
            Self::Func => Ok(StackElem::Func),
        }
    }
}

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

pub enum Action {
    ParseExpr(fn(&mut ExprHelper, &Token) -> Result<()>)
}

pub struct Transition {
    pub to_state: &'static str,
    pub action: Option<Vec<Action>>,
    pub input: TokenProto,
    pub cmp_stack: Option<StackType>,
    pub pop_stack: Option<StackType>,
    pub push_stack: Option<StackType>,
}

pub struct PDA {
    pub state: &'static str,
    states: HashMap<&'static str, Vec<Transition>>,
    pub stack: Vec<StackElem>,
    pub helper: ExprHelper,
}

impl PDA {
    pub fn new() -> Self {
        let mut pda = Self {
            state: "q_exp_value",
            states: HashMap::new(),
            stack: Vec::new(),
            helper: ExprHelper::new(),
        };

        pda.add_expression_states();

        pda
    }

    pub fn add_state(&mut self, name: &'static str, transitions: Vec<Transition>) {
        self.states.insert(name, transitions);
    }

    pub fn transition(&mut self, input: &Token) -> Result<()> {
        let states = &self.states;

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
                    Action::ParseExpr(parse_fn) => {
                        parse_fn(&mut self.helper, input)?;
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

    pub fn run(&mut self, tokens: Vec<Token>) -> Result<Vec<Token>> {
        for token in tokens {
            self.transition(&token)?
        }

        Ok(self.helper.output.clone())
    }
}