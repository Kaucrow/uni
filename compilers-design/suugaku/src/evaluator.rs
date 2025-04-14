use crate::prelude::*;
use anyhow::Result;

pub fn evaluate_postfix(expr: Vec<Token>, ident_map: IdentMap) -> Result<f64> {
    let mut stack: Vec<f64> = Vec::new(); 

    for token in expr {
        match token {
            Token::Number(num) => {
                let num = num.parse::<f64>()?;
                stack.push(num);
            }
            Token::Identifier(name) => {
                let value = ident_map.get(&name)
                    .ok_or(anyhow!(format!("Identifier not found in map: '{}'", name)))?
                    .ok_or(anyhow!(format!("Identifier has no value set: '{}'", name)))?;

                stack.push(value);
            }
            Token::Pow => {
                if stack.len() < 2 {
                    bail!(format!("Not enough operands for operation: '{}'", "pow"));
                }

                let exponent = stack.pop().unwrap();
                let base = stack.pop().unwrap();

                stack.push(base.powf(exponent));
            }
            Token::Operator(op) => {
                if stack.len() < 2 {
                    bail!(format!("Not enough operands for operation: '{}'", op));
                }

                let b = stack.pop().unwrap();
                let a = stack.pop().unwrap();

                let result = match op.as_str() {
                    "+" => a + b,
                    "-" => a - b,
                    "*" => a * b,
                    "/" => {
                        if b == 0.0 {
                            bail!("Division by zero")
                        }
                        a / b
                    }
                    _ => bail!(format!("Unknown operator '{}'", op))
                };

                stack.push(result);
            }
            _ => bail!(format!("Token not expected in postfix expression: '{:?}'", token))
        }
    }

    if stack.len() != 1 {
        bail!("Invalid postfix expression");
    }

    Ok(stack.pop().unwrap())
}