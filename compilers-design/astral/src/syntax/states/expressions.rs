use crate::prelude::*;
use super::*;
use anyhow::Result;

impl PDA {
    pub fn add_expression_states(&mut self) {
        self.add_state(
            "q_exp_value",
            ModeProto::Expr,
            vec![
                TransitionBuilder::new("q_got_value", Input::Token(TokenProto::Number))
                    .action(vec![Action::ParseExpr(value)])
                    .build(),

                TransitionBuilder::new("q_got_value", Input::Token(TokenProto::Identifier))
                    .action(vec![Action::ParseExpr(value)])
                    .build(),

                TransitionBuilder::new("q_got_value", Input::Token(TokenProto::Boolean))
                    .action(vec![Action::ParseExpr(value)])
                    .build(),

                TransitionBuilder::new("q_exp_value", Input::Token(TokenProto::LParen))
                    .action(vec![Action::ParseExpr(lparen)])
                    .push_stack(StackType::LParen)
                    .build(),

                TransitionBuilder::new("q_got_func", Input::Token(TokenProto::FuncCall))
                    .action(vec![Action::ParseExpr(func)])
                    .push_stack(StackType::Func)
                    .build(),
            ]
        );

        self.add_state(
            "q_got_value",
            ModeProto::Expr,
            vec![
                // Operators
                TransitionBuilder::new("q_exp_value", Input::Token(TokenProto::Operator))
                    .action(vec![Action::ParseExpr(operator)])
                    .build(),

                TransitionBuilder::new("q_exp_value", Input::Token(TokenProto::Comparison))
                    .action(vec![Action::ParseExpr(operator)])
                    .build(),

                TransitionBuilder::new("q_exp_value", Input::Token(TokenProto::LogicalAnd))
                    .action(vec![Action::ParseExpr(operator)])
                    .build(),

                TransitionBuilder::new("q_exp_value", Input::Token(TokenProto::LogicalOr))
                    .action(vec![Action::ParseExpr(operator)])
                    .build(),

                // Comma
                TransitionBuilder::new("q_exp_value", Input::Token(TokenProto::Comma))
                    .action(vec![Action::ParseExpr(comma)])
                    .pop_stack(StackType::Func)
                    .push_stack(StackType::Func)
                    .build(),

                // RParen
                TransitionBuilder::new("q_got_value", Input::Token(TokenProto::RParen))
                    .action(vec![Action::ParseExpr(rparen)])
                    .pop_stack(StackType::Func)
                    .build(),

                TransitionBuilder::new("q_got_value", Input::Token(TokenProto::RParen))
                    .action(vec![Action::ParseExpr(rparen)])
                    .pop_stack(StackType::LParen)
                    .build(),

                // Exits
                TransitionBuilder::new("q_exp_end", Input::Token(TokenProto::Semicolon))
                    .action(vec![
                        Action::ParseExpr(expression_end),
                        Action::ParseExpr(build_expr_tree),
                        Action::SwitchMode(Mode::Normal),
                    ])
                    .pop_stack(StackType::Declarations)
                    .push_stack(StackType::Declarations)
                    .build(),

                TransitionBuilder::new("q_neutral", Input::Token(TokenProto::Semicolon))
                    .action(vec![
                        Action::ParseExpr(expression_end),
                        Action::ParseExpr(build_expr_tree),
                        Action::SwitchMode(Mode::Normal),
                        Action::Tree(TreeAction::GoUp),
                    ])
                    .build(),

                // "If" expression end
                TransitionBuilder::new("q_exp_begin", Input::Token(TokenProto::Then))
                    .action(vec![
                        Action::ParseExpr(expression_end),
                        Action::ParseExpr(build_expr_tree),
                        Action::SwitchMode(Mode::Normal),
                    ])
                    .build(),
            ]
        );

        self.add_state(
            "q_got_lparen",
            ModeProto::Expr,
            vec![
                TransitionBuilder::new("q_got_value", Input::Token(TokenProto::Number))
                    .action(vec![Action::ParseExpr(value)])
                    .build(),

                TransitionBuilder::new("q_got_value", Input::Token(TokenProto::Identifier))
                    .action(vec![Action::ParseExpr(value)])
                    .build(),

                TransitionBuilder::new("q_got_lparen", Input::Token(TokenProto::LParen))
                    .action(vec![Action::ParseExpr(lparen)])
                    .build(),
            ]
        );

        self.add_state(
            "q_got_func",
            ModeProto::Expr,
            vec![
                TransitionBuilder::new("q_exp_value", Input::Token(TokenProto::LParen))
                    .action(vec![Action::ParseExpr(lparen)])
                    .build(),
            ]
        );
    }
}

// Determines operator precedence
fn precedence(op: &str) -> i32 {
    match op {
        "||" => 1, // Lowest precedence
        "&&" => 2,
        "=" | "<>" => 3,
        "<" | "<=" | ">" | ">=" => 4,
        "+" | "-" => 5,
        "*" | "/" => 6, // Highest precedence
        _ => 0,         // Default case
    }
}

fn value(helper: &mut Box<ExprHelper>, input: &Token, _: &mut Tree) -> Result<()> {
    let output = &mut helper.output;

    if let Token::Number(_) | Token::Identifier(_) | Token::Boolean(_) = input {
        output.push(input.clone());
    } else {
        bail!(format!("Tried to parse as a value: {:?}", input))
    }

    Ok(())
}

fn operator(helper: &mut Box<ExprHelper>, input: &Token, _: &mut Tree) -> Result<()> {
    let output = &mut helper.output;
    let operators = &mut helper.operators;

    let op = match input {
        Token::Operator(op) | Token::Comparison(op) => op,
        Token::LogicalAnd => "&&",
        Token::LogicalOr => "||",
        _ => bail!("Tried to parse as an operator: {:?}", input)
    };

    while let Some(&Token::Operator(top_op)) = operators.last().as_ref() {
        if precedence(&top_op) >= precedence(op) {
            output.push(operators.pop().unwrap()); // Pop higher precedence ops
        } else {
            break;
        }
    }
    operators.push(input.clone());

    Ok(())
}

fn lparen(helper: &mut Box<ExprHelper>, input: &Token, _: &mut Tree) -> Result<()> {
    let operators = &mut helper.operators;

    if let Token::LParen = input {
        operators.push(Token::LParen)
    } else {
        bail!(format!("Tried to parse as an LParen: {:?}", input))
    }

    Ok(())
}

fn comma(helper: &mut Box<ExprHelper>, input: &Token, _: &mut Tree) -> Result<()> {
    let output = &mut helper.output;
    let operators = &mut helper.operators;
    let arg_count = &mut helper.arg_count;

    if let Token::Comma = input {
        while let Some(top) = operators.last() {
            // Pop until we find a left parenthesis
            if !matches!(top, Token::LParen) {
                output.push(operators.pop().unwrap());
            } else {
                break;
            }
        }
        // Increase argument count
        if let Some(last) = arg_count.last_mut() {
            *last += 1;
        }
    } else {
        bail!(format!("Tried to parse as a comma: {:?}", input))
    }

    Ok(())
}

fn func(helper: &mut Box<ExprHelper>, input: &Token, _: &mut Tree) -> Result<()> {
    let operators = &mut helper.operators;
    let arg_count = &mut helper.arg_count;

    if let Token::FuncCall(_) = input {
        operators.push(input.clone());
        arg_count.push(1);
    } else {
        bail!(format!("Tried to parse as a function: {:?}", input))
    }

    Ok(())
}

// Right parenthesis triggers popping until left parenthesis is found
fn rparen(helper: &mut Box<ExprHelper>, input: &Token, _: &mut Tree) -> Result<()> {
    let output = &mut helper.output;
    let operators = &mut helper.operators;
    let arg_count = &mut helper.arg_count;

    if let Token::RParen = input {
        while let Some(top) = operators.last() {
            if !matches!(top, Token::LParen) {
                output.push(operators.pop().unwrap());
            } else {
                break;
            }
        }
        operators.pop(); // Remove '(' from stack

        // If a function is on top, pop it to output
        if let Some(Token::FuncCall(name)) = operators.last().cloned() {
            let arg_count = arg_count.pop().unwrap_or(1);
            output.push(Token::FuncCall(name));
            helper.arg_count_final.push(arg_count);
            operators.pop();
        }
    } else {
        bail!(format!("Tried to parse as an rparen: {:?}", input))
    }

    Ok(())
}

fn expression_end(helper: &mut Box<ExprHelper>, _: &Token, _: &mut Tree) -> Result<()> {
    let output = &mut helper.output;
    let operators = &mut helper.operators;

    while let Some(op) = operators.pop() {
        output.push(op);
    }

    Ok(())
}

fn build_expr_tree(helper: &mut Box<ExprHelper>, _: &Token, ast: &mut Tree) -> Result<()> {
    let output = &helper.output;

    let mut tree: DiGraph<Token, ()> = DiGraph::new();

    let mut stack: Vec<NodeIndex> = Vec::new(); // Stack to hold nodes
    let arg_counts = &mut helper.arg_count_final;

    for token in output {
        match token {
            Token::Number(_) | Token::Identifier(_) | Token::Boolean(_) =>
                stack.push(tree.add_node(token.clone())),   // Create value node

            // If token is an operator, pop two operands and create an operator node
            Token::Operator(_) | Token::Comparison(_) | Token::LogicalAnd | Token::LogicalOr => {
                let right = stack.pop().expect("Right operand missing");
                let left = stack.pop().expect("Left operand missing");

                let op_node = tree.add_node(token.clone()); // Create operator node
                tree.add_edge(op_node, left, ()); // Connect left operand
                tree.add_edge(op_node, right, ()); // Connect right operand

                stack.push(op_node); // Push the new subtree root back onto stack
            }

            // If token is a function, pop arguments and create function node
            Token::FuncCall(_) => {
                let arg_count = arg_counts.remove(0);

                let func_node = tree.add_node(token.clone());
                let mut args = Vec::new();

                // Pop the correct number of arguments
                for _ in 0..arg_count {
                    args.push(stack.pop().expect("Missing function arguments"));
                }

                // Attach arguments to function node
                for arg in args.into_iter().rev() {
                    tree.add_edge(func_node, arg, ());
                }

                stack.push(func_node); // Push function node onto stack
            }

            _ => bail!(format!("Unexpected token on expression AST build: {:?}", token))
        }
    }

    // The last remaining node in the stack is the root of the expression tree
    let root = stack.pop().expect("Expression AST should have a root node");

    let mut index_map = Vec::new();
    for node in tree.node_indices() {
        let new_index = ast.data.add_node(Node::Val(tree[node].clone()));
        index_map.push((node, new_index));
    }

    for edge in tree.edge_indices() {
        if let Some((source, target)) = tree.edge_endpoints(edge) {
            let new_source = index_map.iter().find(|(old, _)| *old == source).unwrap().1;
            let new_target = index_map.iter().find(|(old, _)| *old == target).unwrap().1;
            ast.data.add_edge(new_source, new_target, ());
        }
    }

    let new_root = index_map.iter().find(|(old, _)| *old == root).unwrap().1;

    ast.data.add_edge(ast.curr_node.unwrap(), new_root, ());

    Ok(())
}