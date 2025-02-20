use crate::prelude::*;
use super::*;

impl PDA {
    pub fn add_normal_states(&mut self) {
        self.add_state(
            "q_start",
            ModeProto::Normal,
            vec![
                Transition {
                    to_state: "q_exp_identifier",
                    action: Some(vec![Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::Program))))]),
                    input: TokenProto::Program,
                    cmp_stack: None,
                    pop_stack: None,
                    push_stack: Some(StackType::Declarations),
                }
            ]
        );

        self.add_state(
            "q_exp_identifier",
            ModeProto::Normal,
            vec![
                Transition {
                    to_state: "q_exp_semicolon",
                    action: Some(vec![
                        Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::Name)))),
                        Action::Tree(TreeAction::AppendChild(None)),
                        Action::Tree(TreeAction::GoUp),
                    ]),
                    input: TokenProto::Identifier,
                    cmp_stack: None,
                    pop_stack: None,
                    push_stack: None,
                }
            ]
        );

        self.add_state(
            "q_exp_semicolon",
            ModeProto::Normal,
            vec![
                Transition {
                    to_state: "q_var_decl",
                    action: None,
                    input: TokenProto::Semicolon,
                    cmp_stack: None,
                    pop_stack: Some(StackType::VarBegun),
                    push_stack: Some(StackType::VarCanExit),
                },
                Transition {
                    to_state: "q_var_decl",
                    action: None,
                    input: TokenProto::Semicolon,
                    cmp_stack: None,
                    pop_stack: Some(StackType::VarCanExit),
                    push_stack: Some(StackType::VarCanExit),
                },
                Transition {
                    to_state: "q_exp_begin",
                    action: None,
                    input: TokenProto::Semicolon,
                    cmp_stack: None,
                    pop_stack: Some(StackType::FuncReturn),
                    push_stack: None,
                },
                Transition {
                    to_state: "q_declarations",
                    action: Some(vec![Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::Declarations))))]),
                    input: TokenProto::Semicolon,
                    cmp_stack: None,
                    pop_stack: None,
                    push_stack: None,
                },
            ]
        );

        self.add_state(
            "q_declarations",
            ModeProto::Normal,
            vec![
                Transition {
                    to_state: "q_var_decl",
                    action: Some(vec![
                        Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::Var))))
                    ]),
                    input: TokenProto::Var,
                    cmp_stack: None,
                    pop_stack: None,
                    push_stack: Some(StackType::VarBegun),
                },
                Transition {
                    to_state: "q_func_decl",
                    action: Some(vec![
                        Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::FuncDecl))))
                    ]),
                    input: TokenProto::Func,
                    cmp_stack: None,
                    pop_stack: None,
                    push_stack: None,
                },
                Transition {
                    to_state: "q_neutral",
                    action: Some(vec![
                        Action::Tree(TreeAction::GoUp),
                        Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::Main)))),
                    ]),
                    input: TokenProto::Begin,
                    cmp_stack: None,
                    pop_stack: Some(StackType::Declarations),
                    push_stack: Some(StackType::Main),
                }
            ]
        );

        self.add_state(
            "q_var_decl",
            ModeProto::Normal,
            vec![
                Transition {
                    to_state: "q_exp_colon",
                    action: Some(vec![Action::Tree(TreeAction::AppendChild(None))]),
                    input: TokenProto::Identifier,
                    cmp_stack: None,
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
                    input: TokenProto::Func,
                    cmp_stack: None,
                    pop_stack: Some(StackType::VarCanExit),
                    push_stack: None,
                }
            ]
        );

        self.add_state(
            "q_exp_colon",
            ModeProto::Normal,
            vec![
                Transition {
                    to_state: "q_exp_datatype",
                    action: None,
                    input: TokenProto::Colon,
                    cmp_stack: None,
                    pop_stack: None,
                    push_stack: None,
                }
            ]
        );

        self.add_state(
            "q_exp_datatype",
            ModeProto::Normal,
            vec![
                Transition {
                    to_state: "q_exp_semicolon",
                    action: Some(vec![
                        Action::Tree(TreeAction::AppendChild(None))
                    ]),
                    input: TokenProto::DataType,
                    cmp_stack: None,
                    pop_stack: Some(StackType::VarBegun),
                    push_stack: Some(StackType::VarBegun),
                },
                Transition {
                    to_state: "q_exp_semicolon",
                    action: Some(vec![
                        Action::Tree(TreeAction::AppendChild(None))
                    ]),
                    input: TokenProto::DataType,
                    cmp_stack: None,
                    pop_stack: Some(StackType::VarCanExit),
                    push_stack: Some(StackType::VarCanExit),
                },
                Transition {
                    to_state: "q_func_params_end",
                    action: Some(vec![
                        Action::Tree(TreeAction::AppendChild(None))
                    ]),
                    input: TokenProto::DataType,
                    cmp_stack: None,
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
                    input: TokenProto::DataType,
                    cmp_stack: None,
                    pop_stack: Some(StackType::FuncReturn),
                    push_stack: Some(StackType::FuncReturn),
                }
            ]
        );

        self.add_state(
            "q_func_decl",
            ModeProto::Normal,
            vec![
                Transition {
                    to_state: "q_exp_lparen",
                    action: Some(vec![
                        Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::Name)))),
                        Action::Tree(TreeAction::AppendChild(None)),
                        Action::Tree(TreeAction::GoUp),
                    ]),
                    input: TokenProto::FuncIdent,
                    cmp_stack: None,
                    pop_stack: None,
                    push_stack: Some(StackType::ExpReturn),
                }
            ]
        );

        self.add_state(
            "q_exp_lparen",
            ModeProto::Normal,
            vec![
                Transition {
                    to_state: "q_exp_writeln_str",
                    action: None,
                    input: TokenProto::LParen,
                    cmp_stack: None,
                    pop_stack: Some(StackType::Writeln),
                    push_stack: Some(StackType::Writeln),
                },
                Transition {
                    to_state: "q_func_params",
                    action: Some(vec![
                        Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::Params)))),
                    ]),
                    input: TokenProto::LParen,
                    cmp_stack: None,
                    pop_stack: None,
                    push_stack: Some(StackType::FuncParams),
                },
            ]
        );

        self.add_state(
            "q_func_params",
            ModeProto::Normal,
            vec![
                Transition {
                    to_state: "q_exp_colon",
                    action: Some(vec![Action::Tree(TreeAction::AppendChild(None))]),
                    input: TokenProto::Identifier,
                    cmp_stack: None,
                    pop_stack: None,
                    push_stack: None,
                }
            ]
        );

        self.add_state(
            "q_func_params_end",
            ModeProto::Normal,
            vec![
                Transition {
                    to_state: "q_func_params",
                    action: None,
                    input: TokenProto::Semicolon,
                    cmp_stack: None,
                    pop_stack: None,
                    push_stack: None,
                },
                Transition {
                    to_state: "q_exp_colon",
                    action: Some(vec![
                        Action::Tree(TreeAction::GoUp),
                    ]),
                    input: TokenProto::RParen,
                    cmp_stack: None,
                    pop_stack: Some(StackType::FuncParams),
                    push_stack: Some(StackType::FuncReturn),
                },
            ]
        );

        self.add_state(
            "q_exp_begin",
            ModeProto::Normal,
            vec![
                Transition {
                    to_state: "q_neutral",
                    action: Some(vec![Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::Body))))]),
                    input: TokenProto::Begin,
                    cmp_stack: None,
                    pop_stack: None,
                    push_stack: None,
                }
            ]
        );

        self.add_state(
            "q_neutral",
            ModeProto::Normal,
            vec![
                // Declarations
                Transition {
                    to_state: "q_var_decl",
                    action: Some(vec![
                        Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::Var))))
                    ]),
                    input: TokenProto::Var,
                    cmp_stack: None,
                    pop_stack: None,
                    push_stack: Some(StackType::VarBegun),
                },
                Transition {
                    to_state: "q_func_decl",
                    action: Some(vec![
                        Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::FuncDecl))))
                    ]),
                    input: TokenProto::Func,
                    cmp_stack: None,
                    pop_stack: None,
                    push_stack: None,
                },
                // General
                // Function declaration's return
                Transition {
                    to_state: "q_exp_assignment",
                    action: Some(vec![
                        Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::Return)))),
                    ]),
                    input: TokenProto::Identifier,
                    cmp_stack: Some(StackType::ExpReturn),
                    pop_stack: Some(StackType::ExpReturn),
                    push_stack: None,
                },
                // Assignment
                Transition {
                    to_state: "q_exp_assignment",
                    action: Some(vec![
                        Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::Assign)))),
                        Action::Tree(TreeAction::AppendChild(None)),
                    ]),
                    input: TokenProto::Identifier,
                    cmp_stack: None,
                    pop_stack: None,
                    push_stack: None,
                },
                // Builtin function call
                Transition {
                    to_state: "q_exp_lparen",
                    action: Some(vec![
                        Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::Writeln)))),
                    ]),
                    input: TokenProto::Writeln,
                    cmp_stack: None,
                    pop_stack: None,
                    push_stack: Some(StackType::Writeln),
                },
                // EOF End
                Transition {
                    to_state: "q_exp_eof",
                    action: None,
                    input: TokenProto::End,
                    cmp_stack: None,
                    pop_stack: Some(StackType::Main),
                    push_stack: Some(StackType::Main),
                },
            ]
        );

        self.add_state(
            "q_exp_assignment",
            ModeProto::Normal,
            vec![
                Transition {
                    to_state: "q_exp_value",
                    action: Some(vec![
                        Action::SwitchMode(Mode::Expr(Box::new(ExprHelper::new()))),
                    ]),
                    input: TokenProto::Assignment,
                    cmp_stack: None,
                    pop_stack: None,
                    push_stack: None,
                },
                Transition {
                    to_state: "q_exp_value",
                    action: Some(vec![
                        Action::SwitchMode(Mode::Expr(Box::new(ExprHelper::new()))),
                    ]),
                    input: TokenProto::Assignment,
                    cmp_stack: None,
                    pop_stack: Some(StackType::Main),
                    push_stack: Some(StackType::Main),
                }
            ]
        );

        self.add_state(
            "q_exp_end",
            ModeProto::Normal,
            vec![
                Transition {
                    to_state: "q_func_ended",
                    action: Some(vec![
                        Action::Tree(TreeAction::GoUp),
                        Action::Tree(TreeAction::GoUp),
                        Action::Tree(TreeAction::GoUp),
                    ]),
                    input: TokenProto::End,
                    cmp_stack: None,
                    pop_stack: Some(StackType::Declarations),
                    push_stack: Some(StackType::Declarations),
                }
            ]
        );

        self.add_state(
            "q_func_ended",
            ModeProto::Normal,
            vec![
                Transition {
                    to_state: "q_declarations",
                    action: None,
                    input: TokenProto::Semicolon,
                    cmp_stack: None,
                    pop_stack: Some(StackType::Declarations),
                    push_stack: Some(StackType::Declarations),
                }
            ] 
        );

        self.add_state(
            "q_exp_writeln_str",
            ModeProto::Normal,
            vec![
                Transition {
                    to_state: "q_exp_comma",
                    action: Some(vec![
                        Action::Tree(TreeAction::AppendChild(None)),
                    ]),
                    input: TokenProto::String,
                    cmp_stack: None,
                    pop_stack: Some(StackType::Writeln),
                    push_stack: Some(StackType::Writeln),
                }
            ]
        );

        self.add_state(
            "q_exp_comma",
            ModeProto::Normal,
            vec![
                Transition {
                    to_state: "q_exp_value",
                    action: Some(vec![
                        Action::SwitchMode(Mode::Expr(Box::new(ExprHelper::new()))),
                    ]),
                    input: TokenProto::Comma,
                    cmp_stack: None,
                    pop_stack: Some(StackType::Writeln),
                    push_stack: Some(StackType::LParen),
                }
            ]
        );

        self.add_state(
            "q_exp_eof",
            ModeProto::Normal,
            vec![
                Transition {
                    to_state: "q_accepting",
                    action: None,
                    input: TokenProto::Dot,
                    cmp_stack: None,
                    pop_stack: Some(StackType::Main),
                    push_stack: None,
                }
            ]
        );

        self.add_state(
            "q_accepting",
            ModeProto::Normal,
            vec![]
        );
    }
}