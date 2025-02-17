use crate::prelude::*;
use super::{
    Mode, Action, Id, Node, StackType, Transition, TreeAction, PDA
};

impl PDA {
    pub fn build(&mut self) {
        self.add_state(
            "q_start",
            vec![
                Transition {
                    to_state: "q_exp_identifier",
                    action: Some(vec![Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::Program))))]),
                    input: (TokenType::Keyword, Some("program")),
                    cmp_stack: false,
                    pop_stack: None,
                    push_stack: None,
                }
            ]
        );

        self.add_state(
            "q_exp_identifier",
            vec![
                Transition {
                    to_state: "q_exp_semicolon",
                    action: Some(vec![
                        Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::Name)))),
                        Action::Tree(TreeAction::AppendChild(None)),
                        Action::Tree(TreeAction::GoUp),
                    ]),
                    input: (TokenType::Identifier, None),
                    cmp_stack: false,
                    pop_stack: None,
                    push_stack: None,
                }
            ]
        );

        self.add_state(
            "q_exp_semicolon",
            vec![
                Transition {
                    to_state: "q_var_decl",
                    action: None,
                    input: (TokenType::Symbol, Some(";")),
                    cmp_stack: false,
                    pop_stack: Some(StackType::VarBegun),
                    push_stack: Some(StackType::VarCanExit),
                },
                Transition {
                    to_state: "q_var_decl",
                    action: None,
                    input: (TokenType::Symbol, Some(";")),
                    cmp_stack: false,
                    pop_stack: Some(StackType::VarCanExit),
                    push_stack: Some(StackType::VarCanExit),
                },
                Transition {
                    to_state: "q_exp_begin",
                    action: None,
                    input: (TokenType::Symbol, Some(";")),
                    cmp_stack: false,
                    pop_stack: Some(StackType::FuncReturn),
                    push_stack: None,
                },
                Transition {
                    to_state: "q_declarations",
                    action: Some(vec![Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::Declarations))))]),
                    input: (TokenType::Symbol, Some(";")),
                    cmp_stack: false,
                    pop_stack: None,
                    push_stack: None,
                },
            ]
        );

        self.add_state(
            "q_declarations",
            vec![
                Transition {
                    to_state: "q_var_decl",
                    action: Some(vec![Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::Var))))]),
                    input: (TokenType::Keyword, Some("var")),
                    cmp_stack: false,
                    pop_stack: None,
                    push_stack: Some(StackType::VarBegun),
                },
                Transition {
                    to_state: "q_func_decl",
                    action: Some(vec![Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::FuncDecl))))]),
                    input: (TokenType::Keyword, Some("function")),
                    cmp_stack: false,
                    pop_stack: None,
                    push_stack: None,
                },
                Transition {
                    to_state: "q_main_fn",
                    action: None,
                    input: (TokenType::Keyword, Some("begin")),
                    cmp_stack: false,
                    pop_stack: None,
                    push_stack: Some(StackType::Main),
                }
            ]
        );

        self.add_state(
            "q_var_decl",
            vec![
                Transition {
                    to_state: "q_exp_colon",
                    action: Some(vec![Action::Tree(TreeAction::AppendChild(None))]),
                    input: (TokenType::Identifier, None),
                    cmp_stack: false,
                    pop_stack: None,
                    push_stack: None,
                },
                // Exits
                Transition {
                    to_state: "q_func_decl",
                    action: Some(vec![
                        Action::Tree(TreeAction::GoUp),
                        Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::FuncDecl)))),
                    ]),
                    input: (TokenType::Keyword, Some("function")),
                    cmp_stack: false,
                    pop_stack: Some(StackType::VarCanExit),
                    push_stack: None,
                }
            ]
        );

        self.add_state(
            "q_exp_colon",
            vec![
                Transition {
                    to_state: "q_exp_datatype",
                    action: None,
                    input: (TokenType::Symbol, Some(":")),
                    cmp_stack: false,
                    pop_stack: None,
                    push_stack: None,
                }
            ]
        );

        self.add_state(
            "q_exp_datatype",
            vec![
                Transition {
                    to_state: "q_exp_semicolon",
                    action: Some(vec![
                        Action::Tree(TreeAction::AppendChild(None))
                    ]),
                    input: (TokenType::DataType, None),
                    cmp_stack: false,
                    pop_stack: Some(StackType::VarBegun),
                    push_stack: Some(StackType::VarBegun),
                },
                Transition {
                    to_state: "q_exp_semicolon",
                    action: Some(vec![
                        Action::Tree(TreeAction::AppendChild(None))
                    ]),
                    input: (TokenType::DataType, None),
                    cmp_stack: false,
                    pop_stack: Some(StackType::VarCanExit),
                    push_stack: Some(StackType::VarCanExit),
                },
                Transition {
                    to_state: "q_func_params_end",
                    action: Some(vec![
                        Action::Tree(TreeAction::AppendChild(None))
                    ]),
                    input: (TokenType::DataType, None),
                    cmp_stack: false,
                    pop_stack: Some(StackType::FuncParams),
                    push_stack: Some(StackType::FuncParams),
                },
                Transition {
                    to_state: "q_exp_semicolon",
                    action: Some(vec![
                        Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::ReturnType)))),
                        Action::Tree(TreeAction::AppendChild(None)),
                        Action::Tree(TreeAction::GoUp),
                    ]),
                    input: (TokenType::DataType, None),
                    cmp_stack: false,
                    pop_stack: Some(StackType::FuncReturn),
                    push_stack: Some(StackType::FuncReturn),
                }
            ]
        );

        self.add_state(
            "q_func_decl",
            vec![
                Transition {
                    to_state: "q_exp_lparen",
                    action: Some(vec![
                        Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::Name)))),
                        Action::Tree(TreeAction::AppendChild(None)),
                        Action::Tree(TreeAction::GoUp),
                    ]),
                    input: (TokenType::Identifier, None),
                    cmp_stack: false,
                    pop_stack: None,
                    push_stack: Some(StackType::Func),
                }
            ]
        );

        self.add_state(
            "q_exp_lparen",
            vec![
                Transition {
                    to_state: "q_func_params",
                    action: Some(vec![
                        Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::Params)))),
                    ]),
                    input: (TokenType::Symbol, Some("(")),
                    cmp_stack: false,
                    pop_stack: None,
                    push_stack: Some(StackType::FuncParams),
                }
            ]
        );

        self.add_state(
            "q_func_params",
            vec![
                Transition {
                    to_state: "q_exp_colon",
                    action: Some(vec![Action::Tree(TreeAction::AppendChild(None))]),
                    input: (TokenType::Identifier, None),
                    cmp_stack: false,
                    pop_stack: None,
                    push_stack: None,
                }
            ]
        );

        self.add_state(
            "q_func_params_end",
            vec![
                Transition {
                    to_state: "q_func_params",
                    action: None,
                    input: (TokenType::Symbol, Some(";")),
                    cmp_stack: false,
                    pop_stack: None,
                    push_stack: None,
                },
                Transition {
                    to_state: "q_exp_colon",
                    action: Some(vec![
                        Action::Tree(TreeAction::GoUp),
                    ]),
                    input: (TokenType::Symbol, Some(")")),
                    cmp_stack: false,
                    pop_stack: Some(StackType::FuncParams),
                    push_stack: Some(StackType::FuncReturn),
                },
            ]
        );

        self.add_state(
            "q_exp_begin",
            vec![
                Transition {
                    to_state: "q_neutral",
                    action: Some(vec![Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::Body))))]),
                    input: (TokenType::Keyword, Some("begin")),
                    cmp_stack: false,
                    pop_stack: Some(StackType::Func),
                    push_stack: Some(StackType::Func),
                }
            ]
        );

        self.add_state(
            "q_neutral",
            vec![
                Transition {
                    to_state: "q_exp_assignment",
                    action: None,
                    input: (TokenType::Identifier, None),
                    cmp_stack: true,
                    pop_stack: Some(StackType::Func),
                    push_stack: None,
                }
            ]
        );

        self.add_state(
            "q_exp_assignment",
            vec![
                Transition {
                    to_state: "q_expression",
                    action: Some(vec![
                        Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::Return)))),
                        Action::SwitchMode(Mode::Expr),
                    ]),
                    input: (TokenType::Symbol, Some(":=")),
                    cmp_stack: false,
                    pop_stack: None,
                    push_stack: None,
                }
            ]
        );

        self.add_state(
            "q_expression",
            vec![]
        )
    }
}