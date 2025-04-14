use crate::prelude::*;
use super::*;

impl PDA {
    pub fn add_normal_states(&mut self) {
        self.add_state(
            "q_start",
            ModeProto::Normal,
            vec![
                TransitionBuilder::new("q_exp_identifier", Input::Token(TokenProto::Program))
                    .action(vec![Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::Program))))])
                    .push_stack(StackType::Declarations)
                    .build(),
            ]
        );

        self.add_state(
            "q_exp_identifier",
            ModeProto::Normal,
            vec![
                TransitionBuilder::new("q_exp_semicolon", Input::Token(TokenProto::Identifier))
                    .action(vec![
                        Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::Name)))),
                        Action::Tree(TreeAction::AppendChild(None)),
                        Action::Tree(TreeAction::GoUp),
                    ])
                    .build(),
            ]
        );

        self.add_state(
            "q_exp_semicolon",
            ModeProto::Normal,
            vec![
                TransitionBuilder::new("q_var_decl", Input::Token(TokenProto::Semicolon))
                    .pop_stack(StackType::VarBegun)
                    .push_stack(StackType::VarCanExit)
                    .build(),

                TransitionBuilder::new("q_var_decl", Input::Token(TokenProto::Semicolon))
                    .pop_stack(StackType::VarCanExit)
                    .push_stack(StackType::VarCanExit)
                    .build(),

                TransitionBuilder::new("q_exp_begin", Input::Token(TokenProto::Semicolon))
                    .pop_stack(StackType::FuncReturn)
                    .build(),

                TransitionBuilder::new("q_declarations", Input::Token(TokenProto::Semicolon))
                    .action(vec![Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::Declarations))))])
                    .pop_stack(StackType::Declarations)
                    .push_stack(StackType::Declarations)
                    .build(),

                TransitionBuilder::new("q_neutral", Input::Token(TokenProto::Semicolon))
                    .build(),
            ]
        );

        self.add_state(
            "q_declarations",
            ModeProto::Normal,
            vec![
                TransitionBuilder::new("q_var_decl", Input::Token(TokenProto::Var))
                    .action(vec![
                        Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::Var))))
                    ])
                    .push_stack(StackType::VarBegun)
                    .build(),

                TransitionBuilder::new("q_func_decl", Input::Token(TokenProto::Func))
                    .action(vec![
                        Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::FuncDecl))))
                    ])
                    .build(),

                TransitionBuilder::new("q_neutral", Input::Token(TokenProto::Begin))
                    .action(vec![
                        Action::Tree(TreeAction::GoUp),
                        Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::Main)))),
                    ])
                    .pop_stack(StackType::Declarations)
                    .push_stack(StackType::Main)
                    .build(),
            ]
        );

        self.add_state(
            "q_var_decl",
            ModeProto::Normal,
            vec![
                TransitionBuilder::new("q_exp_colon", Input::Token(TokenProto::Identifier))
                    .action(vec![Action::Tree(TreeAction::AppendChild(None))])
                    .peek_input(TokenProto::Colon)
                    .build(),

                // Exits
                TransitionBuilder::new("q_declarations", Input::Any)
                    .action(vec![Action::Tree(TreeAction::GoUp)])
                    .peek_stack(StackType::Declarations)
                    .pop_stack(StackType::VarCanExit)
                    .build(),

                TransitionBuilder::new("q_neutral", Input::Any)
                    .action(vec![Action::Tree(TreeAction::GoUp)])
                    .pop_stack(StackType::VarCanExit)
                    .build(),
            ]
        );

        self.add_state(
            "q_exp_colon",
            ModeProto::Normal,
            vec![
                TransitionBuilder::new("q_exp_datatype", Input::Token(TokenProto::Colon))
                    .build(),
            ]
        );

        self.add_state(
            "q_exp_datatype",
            ModeProto::Normal,
            vec![
                TransitionBuilder::new("q_exp_semicolon", Input::Token(TokenProto::DataType))
                    .action(vec![Action::Tree(TreeAction::AppendChild(None))])
                    .pop_stack(StackType::VarBegun)
                    .push_stack(StackType::VarBegun)
                    .build(),

                TransitionBuilder::new("q_exp_semicolon", Input::Token(TokenProto::DataType))
                    .action(vec![Action::Tree(TreeAction::AppendChild(None))])
                    .pop_stack(StackType::VarCanExit)
                    .push_stack(StackType::VarCanExit)
                    .build(),

                TransitionBuilder::new("q_func_params_end", Input::Token(TokenProto::DataType))
                    .action(vec![Action::Tree(TreeAction::AppendChild(None))])
                    .pop_stack(StackType::FuncParams)
                    .push_stack(StackType::FuncParams)
                    .build(),

                TransitionBuilder::new("q_exp_semicolon", Input::Token(TokenProto::DataType))
                    .action(vec![
                        Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::ReturnType)))),
                        Action::Tree(TreeAction::AppendChild(None)),
                        Action::Tree(TreeAction::GoUp),
                    ])
                    .pop_stack(StackType::FuncReturn)
                    .push_stack(StackType::FuncReturn)
                    .build(),
            ]
        );

        self.add_state(
            "q_func_decl",
            ModeProto::Normal,
            vec![
                TransitionBuilder::new("q_exp_lparen", Input::Token(TokenProto::FuncIdent))
                    .action(vec![
                        Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::Name)))),
                        Action::Tree(TreeAction::AppendChild(None)),
                        Action::Tree(TreeAction::GoUp),
                    ])
                    .push_stack(StackType::ExpReturn)
                    .build(),
            ]
        );

        self.add_state(
            "q_exp_lparen",
            ModeProto::Normal,
            vec![
                TransitionBuilder::new("q_exp_writeln_str", Input::Token(TokenProto::LParen))
                    .pop_stack(StackType::Writeln)
                    .push_stack(StackType::Writeln)
                    .build(),

                TransitionBuilder::new("q_func_params", Input::Token(TokenProto::LParen))
                    .action(vec![Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::Params))))])
                    .push_stack(StackType::FuncParams)
                    .build(),
            ]
        );

        self.add_state(
            "q_func_params",
            ModeProto::Normal,
            vec![
                TransitionBuilder::new("q_exp_colon", Input::Token(TokenProto::Identifier))
                    .action(vec![Action::Tree(TreeAction::AppendChild(None))])
                    .build(),
            ]
        );

        self.add_state(
            "q_func_params_end",
            ModeProto::Normal,
            vec![
                TransitionBuilder::new("q_func_params", Input::Token(TokenProto::Semicolon)).build(),
                
                TransitionBuilder::new("q_exp_colon", Input::Token(TokenProto::RParen))
                    .action(vec![Action::Tree(TreeAction::GoUp)])
                    .pop_stack(StackType::FuncParams)
                    .push_stack(StackType::FuncReturn)
                    .build(),
            ]
        );

        self.add_state(
            "q_exp_begin",
            ModeProto::Normal,
            vec![
                TransitionBuilder::new("q_neutral", Input::Token(TokenProto::Begin))
                    .action(vec![Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::Body))))])
                    .pop_stack(StackType::ExpReturn)
                    .push_stack(StackType::ExpReturn)
                    .build(),

                TransitionBuilder::new("q_neutral", Input::Token(TokenProto::Begin))
                    .pop_stack(StackType::If)
                    .push_stack(StackType::If)
                    .build(),

                TransitionBuilder::new("q_neutral", Input::Token(TokenProto::Begin))
                    .pop_stack(StackType::Else)
                    .push_stack(StackType::Else)
                    .build(),
            ]
        );

        self.add_state(
            "q_neutral",
            ModeProto::Normal,
            vec![
                // Declarations
                TransitionBuilder::new("q_var_decl", Input::Token(TokenProto::Var))
                    .action(vec![Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::Var))))])
                    .push_stack(StackType::VarBegun)
                    .build(),

                TransitionBuilder::new("q_func_decl", Input::Token(TokenProto::Func))
                    .action(vec![Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::FuncDecl))))])
                    .build(),

                // Function return
                TransitionBuilder::new("q_exp_assignment", Input::Token(TokenProto::Identifier))
                    .action(vec![Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::Return))))])
                    .cmp_stack(StackType::ExpReturn)
                    .pop_stack(StackType::ExpReturn)
                    .build(),

                // If statement
                TransitionBuilder::new("q_exp_value", Input::Token(TokenProto::If))
                    .action(vec![
                        Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::If)))),
                        Action::SwitchMode(Mode::Expr(Box::new(ExprHelper::new()))),
                    ])
                    .push_stack(StackType::If)
                    .build(),

                TransitionBuilder::new("q_if_ended", Input::Token(TokenProto::End))
                    .pop_stack(StackType::If)
                    .push_stack(StackType::If)
                    .build(),

                TransitionBuilder::new("q_exp_semicolon", Input::Token(TokenProto::End))
                    .action(vec![
                        Action::Tree(TreeAction::GoUp),
                        Action::Tree(TreeAction::GoUp),
                    ])
                    .pop_stack(StackType::Else)
                    .build(),

                // Assignment
                TransitionBuilder::new("q_exp_assignment", Input::Token(TokenProto::Identifier))
                    .action(vec![
                        Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::Assign)))),
                        Action::Tree(TreeAction::AppendChild(None)),
                    ])
                    .build(),

                // Built-in function call
                TransitionBuilder::new("q_exp_lparen", Input::Token(TokenProto::Writeln))
                    .action(vec![Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::Writeln))))])
                    .push_stack(StackType::Writeln)
                    .build(),

                // EOF End
                TransitionBuilder::new("q_exp_eof", Input::Token(TokenProto::End))
                    .pop_stack(StackType::Main)
                    .push_stack(StackType::Main)
                    .build(),
            ]
        );

        self.add_state(
            "q_exp_assignment",
            ModeProto::Normal,
            vec![
                TransitionBuilder::new("q_exp_value", Input::Token(TokenProto::Assignment))
                    .action(vec![
                        Action::SwitchMode(Mode::Expr(Box::new(ExprHelper::new()))),
                    ])
                    .build(),
                TransitionBuilder::new("q_exp_value", Input::Token(TokenProto::Assignment))
                    .action(vec![
                        Action::SwitchMode(Mode::Expr(Box::new(ExprHelper::new()))),
                    ])
                    .pop_stack(StackType::Main)
                    .push_stack(StackType::Main)
                    .build(),
            ]
        );

        self.add_state(
            "q_exp_end",
            ModeProto::Normal,
            vec![
                TransitionBuilder::new("q_func_ended", Input::Token(TokenProto::End))
                    .action(vec![
                        Action::Tree(TreeAction::GoUp),
                        Action::Tree(TreeAction::GoUp),
                        Action::Tree(TreeAction::GoUp),
                    ])
                    .pop_stack(StackType::Declarations)
                    .push_stack(StackType::Declarations)
                    .build(),
            ]
        );

        self.add_state(
            "q_func_ended",
            ModeProto::Normal,
            vec![
                TransitionBuilder::new("q_declarations", Input::Token(TokenProto::Semicolon))
                    .pop_stack(StackType::Declarations)
                    .push_stack(StackType::Declarations)
                    .build(),
            ]
        );

        self.add_state(
            "q_exp_writeln_str",
            ModeProto::Normal,
            vec![
                TransitionBuilder::new("q_exp_comma", Input::Token(TokenProto::String))
                    .action(vec![
                        Action::Tree(TreeAction::AppendChild(None)),
                    ])
                    .pop_stack(StackType::Writeln)
                    .push_stack(StackType::Writeln)
                    .build(),
            ]
        );

        self.add_state(
            "q_exp_comma",
            ModeProto::Normal,
            vec![
                TransitionBuilder::new("q_exp_value", Input::Token(TokenProto::Comma))
                    .action(vec![
                        Action::SwitchMode(Mode::Expr(Box::new(ExprHelper::new()))),
                    ])
                    .pop_stack(StackType::Writeln)
                    .push_stack(StackType::LParen)
                    .build(),
            ]
        );

        self.add_state(
            "q_if_ended",
            ModeProto::Normal,
            vec![
                TransitionBuilder::new("q_neutral", Input::Token(TokenProto::Semicolon))
                    .action(vec![
                        Action::Tree(TreeAction::GoUp),
                    ])
                    .pop_stack(StackType::If)
                    .build(),
                TransitionBuilder::new("q_exp_begin", Input::Token(TokenProto::Else))
                    .action(vec![
                        Action::Tree(TreeAction::AddNode(Some(Node::Id(Id::Else)))),
                    ])
                    .pop_stack(StackType::If)
                    .push_stack(StackType::Else)
                    .build(),
            ]
        );

        self.add_state(
            "q_exp_eof",
            ModeProto::Normal,
            vec![
                TransitionBuilder::new("q_accepting", Input::Token(TokenProto::Dot))
                    .pop_stack(StackType::Main)
                    .build(),
            ]
        );

        self.add_state(
            "q_accepting",
            ModeProto::Normal,
            vec![]
        );
    }
}