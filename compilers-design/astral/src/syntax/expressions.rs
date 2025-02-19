use crate::prelude::*;
use super::*;
use anyhow::Result;

enum ShuntingYardResult {
    Ok,
    Continue,
}

impl PDA {
    pub fn parse_expression(helper: &mut Box<ExprHelper>, input: &Token, ast: &mut Tree) -> Result<()> {
        match Self::shunting_yard(helper, input)? {
            ShuntingYardResult::Continue => Ok(()),
            ShuntingYardResult::Ok => {
                let (expr_tree, root) = Self::build_expression_ast(helper)?;

                let mut index_map = Vec::new();
                for node in expr_tree.node_indices() {
                    let new_index = ast.data.add_node(Node::Val(expr_tree[node].clone()));
                    index_map.push((node, new_index));
                }

                for edge in expr_tree.edge_indices() {
                    if let Some((source, target)) = expr_tree.edge_endpoints(edge) {
                        let new_source = index_map.iter().find(|(old, _)| *old == source).unwrap().1;
                        let new_target = index_map.iter().find(|(old, _)| *old == target).unwrap().1;
                        ast.data.add_edge(new_source, new_target, ());
                    }
                }

                let new_root = index_map.iter().find(|(old, _)| *old == root).unwrap().1;

                ast.data.add_edge(ast.curr_node.unwrap(), new_root, ());

                println!("{:?}", Dot::with_config(&ast.data, &[Config::EdgeNoLabel]));

                todo!("Return to normal mode after expression parsing");

                Ok(())
            }
        }
    }

    fn shunting_yard(helper: &mut Box<ExprHelper>, input: &Token) -> Result<ShuntingYardResult> {
        // Determines operator precedence
        fn precedence(op: &str) -> i32 {
            match op {
                "+" | "-" => 1, // Lowest precedence
                "*" | "/" => 2, // Higher precedence
                _ => 0,         // Default case
            }
        }
        
        let output = &mut helper.output;
        let operators = &mut helper.operators;
        let arg_count = &mut helper.arg_count;

        match input {
            Token::Number(_) | Token::Identifier(_) => output.push(input.clone()),

            Token::FuncCall(name) => {
                operators.push(Token::FuncCall(name.clone()));
                arg_count.push(1);  // Initialize argument count
            }

            Token::Comma => {
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
            }

            Token::Operator(op) => {
                while let Some(&Token::Operator(top_op)) = operators.last().as_ref() {
                    if precedence(&top_op) >= precedence(op) {
                        output.push(operators.pop().unwrap());  // Pop higher precedence ops
                    } else {
                        break;
                    }
                }
                operators.push(Token::Operator(op.clone()));
            }

            Token::LParen => operators.push(Token::LParen),

            // Right parenthesis triggers popping until left parenthesis is found
            Token::RParen => {
                while let Some(top) = operators.last() {
                    if !matches!(top, Token::LParen) {
                        output.push(operators.pop().unwrap());
                    } else {
                        break;
                    }
                }
                operators.pop();    // Remove '(' from stack

                // If a function is on top, pop it to output
                if let Some(Token::FuncCall(name)) = operators.last().cloned() {
                    let arg_count = arg_count.pop().unwrap_or(1);
                    output.push(Token::FuncCall(name));
                    helper.arg_count_final.push(arg_count);
                    operators.pop();
                }
            }

            // A semicolon indicates the end of the expression
            Token::Semicolon => {
                while let Some(op) = operators.pop() {
                    output.push(op);
                }

                for token in output {
                    println!("{:?}", token);
                }

                println!("{:?}", helper.arg_count_final);

                return Ok(ShuntingYardResult::Ok)
            }

            _ => bail!(format!("Unexpected token found in expression: {:?}", input)),
        }

        Ok(ShuntingYardResult::Continue)
    }

    fn build_expression_ast(helper: &mut Box<ExprHelper>) -> Result<(DiGraph<Token, ()>, NodeIndex)> {
        let mut tree: DiGraph<Token, ()> = DiGraph::new();

        let mut stack: Vec<NodeIndex> = Vec::new(); // Stack to hold nodes
        let postfix_tokens = &helper.output;
        let arg_counts = &mut helper.arg_count_final;

        for token in postfix_tokens {
            match token {
                Token::Number(_) => stack.push(tree.add_node(token.clone())),   // Create number node
                Token::Identifier(_) => stack.push(tree.add_node(token.clone())),   // Create variable node

                // If token is an operator, pop two operands and create an operator node
                Token::Operator(_) => {
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

        Ok((tree, root))
    }
}