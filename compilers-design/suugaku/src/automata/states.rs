use crate::prelude::*;
use super::*;
use anyhow::Result;

impl PDA {
    pub fn add_expression_states(&mut self) {
        self.add_state(
            "q_exp_value",
            vec![
                Transition {
                    to_state: "q_got_value",
                    action: Some(vec![
                        Action::ParseExpr(value)
                    ]),
                    input: TokenProto::Number,
                    cmp_stack: None,
                    pop_stack: None,
                    push_stack: None,
                },
                Transition {
                    to_state: "q_got_value",
                    action: Some(vec![
                        Action::ParseExpr(value)
                    ]),
                    input: TokenProto::Identifier,
                    cmp_stack: None,
                    pop_stack: None,
                    push_stack: None,
                },
                Transition {
                    to_state: "q_exp_value",
                    action: Some(vec![
                        Action::ParseExpr(lparen)
                    ]),
                    input: TokenProto::LParen,
                    cmp_stack: None,
                    pop_stack: None,
                    push_stack: Some(StackType::LParen),
                },
            ]
        );

        self.add_state(
            "q_got_value",
            vec![
                // Operators
                Transition {
                    to_state: "q_exp_value",
                    action: Some(vec![
                        Action::ParseExpr(operator)
                    ]),
                    input: TokenProto::Operator,
                    cmp_stack: None,
                    pop_stack: None,
                    push_stack: None,
                },
                Transition {
                    to_state: "q_got_value",
                    action: Some(vec![
                        Action::ParseExpr(rparen)
                    ]),
                    input: TokenProto::RParen,
                    cmp_stack: None,
                    pop_stack: Some(StackType::LParen),
                    push_stack: None,
                },
                // Exits
                // Error
                Transition {
                    to_state: "q_end",
                    action: Some(vec![
                        Action::ParseExpr(error),
                    ]),
                    input: TokenProto::EOF,
                    cmp_stack: None,
                    pop_stack: Some(StackType::LParen),
                    push_stack: Some(StackType::LParen),
                },
                // Accept
                Transition {
                    to_state: "q_end",
                    action: Some(vec![
                        Action::ParseExpr(expression_end),
                    ]),
                    input: TokenProto::EOF,
                    cmp_stack: None,
                    pop_stack: None,
                    push_stack: None,
                },
            ]
        );

        self.add_state(
            "q_got_lparen",
            vec![
                Transition {
                    to_state: "q_got_value",
                    action: Some(vec![
                        Action::ParseExpr(value)
                    ]),
                    input: TokenProto::Number,
                    cmp_stack: None,
                    pop_stack: None,
                    push_stack: None,
                },
                Transition {
                    to_state: "q_got_value",
                    action: Some(vec![
                        Action::ParseExpr(value)
                    ]),
                    input: TokenProto::Identifier,
                    cmp_stack: None,
                    pop_stack: None,
                    push_stack: None,
                },
                Transition {
                    to_state: "q_got_lparen",
                    action: Some(vec![
                        Action::ParseExpr(lparen)
                    ]),
                    input: TokenProto::LParen,
                    cmp_stack: None,
                    pop_stack: None,
                    push_stack: Some(StackType::LParen),
                }
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

fn value(helper: &mut ExprHelper, input: &Token) -> Result<()> {
    let output = &mut helper.output;

    if let Token::Number(_) | Token::Identifier(_) = input {
        output.push(input.clone());
    } else {
        bail!(format!("Tried to parse as a value: {:?}", input))
    }

    Ok(())
}

fn operator(helper: &mut ExprHelper, input: &Token) -> Result<()> {
    let output = &mut helper.output;
    let operators = &mut helper.operators;

    let op = if let Token::Operator(op) = input {
        op
    } else {
        bail!("Tried to parse as an operator: {:?}", input)
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

fn lparen(helper: &mut ExprHelper, input: &Token) -> Result<()> {
    let operators = &mut helper.operators;

    if let Token::LParen = input {
        operators.push(Token::LParen)
    } else {
        bail!(format!("Tried to parse as an LParen: {:?}", input))
    }

    Ok(())
}

// Right parenthesis triggers popping until left parenthesis is found
fn rparen(helper: &mut ExprHelper, input: &Token) -> Result<()> {
    let output = &mut helper.output;
    let operators = &mut helper.operators;

    if let Token::RParen = input {
        while let Some(top) = operators.last() {
            if !matches!(top, Token::LParen) {
                output.push(operators.pop().unwrap());
            } else {
                break;
            }
        }
        operators.pop(); // Remove '(' from stack
    } else {
        bail!(format!("Tried to parse as an rparen: {:?}", input))
    }

    Ok(())
}

fn expression_end(helper: &mut ExprHelper, _: &Token) -> Result<()> {
    let output = &mut helper.output;
    let operators = &mut helper.operators;

    while let Some(op) = operators.pop() {
        output.push(op);
    }

    Ok(())
}

fn error(_: &mut ExprHelper, _: &Token) -> Result<()> {
    bail!("The parsing failed. There's likely an unclosed left parenthesis in the expression.")
}

/*
fn build_expr_tree(helper: &mut Box<ExprHelper>, _: &Token, ast: &mut Tree) -> Result<()> {
    let output = &helper.output;

    let mut tree: DiGraph<Token, ()> = DiGraph::new();

    let mut stack: Vec<NodeIndex> = Vec::new(); // Stack to hold nodes
    let arg_counts = &mut helper.arg_count_final;

    for token in output {
        match token {
            Token::Number(_) => stack.push(tree.add_node(token.clone())),   // Create number node
            Token::Identifier(_) => stack.push(tree.add_node(token.clone())),   // Create variable node

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
*/