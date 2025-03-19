pub mod constants;

use crate::prelude::*;
use constants::*;
use anyhow::Result;
use petgraph::data;
use syntax::tree::{ Node, Id };

pub struct Dict {
    var: VarDict,
    func: FuncDict,
}

impl Dict {
    pub fn new() -> Self {
        Self {
            var: VarDict::new(),
            func: FuncDict::new(),
        }
    }
}

pub struct Helper {
    scope_stack: Vec<(Scope, i32)>,
    act_stack: Vec<(Id, i32)>,
    temp_var: String,
}

impl Helper {
    pub fn new() -> Self {
        Self {
            scope_stack: vec![(Scope::Global, -1)],
            act_stack: Vec::new(),
            temp_var: String::new(),
        }
    }
}

pub fn preorder_traversal(dict: &mut Dict, helper: &mut Helper, node_idx: NodeIndex, ast: &syntax::Tree, depth: i32) -> Result<()> {
    println!("{:?} at depth {}", ast.data[node_idx], depth);

    let act_stack = &mut helper.act_stack;
    let temp_var = &mut helper.temp_var;

    let node = &ast.data[node_idx];

    // Clear up stack actions that have finished
    while let Some((_, act_depth)) = act_stack.last() {
        if depth <= *act_depth {    // If we're moving up the tree
            act_stack.pop(); // Remove the last action
        } else {
            break; // Stop when we reach a higher node
        }
    }

    // Clear up scopes that have finished
    while let Some((_, scope_depth)) = helper.scope_stack.last() {
        if depth <= *scope_depth {    // If we're moving up the tree
            helper.scope_stack.pop(); // Remove the last action
        } else {
            break; // Stop when we reach a higher node
        }
    }

    match node {
        Node::Id(id) if *id == Id::Var || *id == Id::Assign || *id == Id::FuncDecl => {
            act_stack.push((*id, depth));
        }
        Node::Id(Id::Return) => {
            act_stack.push((Id::Return, depth));
        }
        Node::Val(val) => {
            match val {
                Token::Identifier(name) => {
                    if let Some(act) = act_stack.last() {
                        match act {
                            // Variable declaration
                            (Id::Var, _) | (Id::FuncDecl, _) => {
                                *temp_var = name.to_string();
                            }
                            _ => {}
                        }
                    }
                }
                Token::DataType(data_type) => {
                    if let Some(act) = act_stack.last() {
                        match act {
                            // Variable declaration
                            (Id::Var, _) => {
                                let (scope, _) = helper.scope_stack.last().unwrap();

                                let variables = dict.var.entry(scope.clone()).or_insert_with(HashSet::new);
                                let success = variables.insert(Variable::new(temp_var.clone(), data_type.clone()));
                                if !success { bail!("Duplicated variable") }

                                *temp_var = String::new();
                            }
                            // Function declaration parameters
                            (Id::FuncDecl, _) => {
                                let (scope, _) = helper.scope_stack.last().unwrap();

                                // The node is the return type
                                if temp_var.is_empty() {
                                    let func = dict.func.entry(scope.clone()).or_insert_with(FuncDetails::new);
                                    func.returntype = Some(data_type.clone());
                                // The node is a parameter
                                } else {
                                    let func = dict.func.entry(scope.clone()).or_insert_with(FuncDetails::new);
                                    func.parameters.push(data_type.clone());

                                    let variables = dict.var.entry(scope.clone()).or_insert_with(HashSet::new);
                                    let success = variables.insert(Variable::new(temp_var.clone(), data_type.clone()));
                                    if !success { bail!("Duplicated variable") }
                                    
                                    *temp_var = String::new();
                                }
                            }
                            _ => {}
                        }
                    }
                }
                Token::FuncIdent(name) => {
                    if let Some(act) = act_stack.last() {
                        match act {
                            // Function declaration
                            (Id::FuncDecl, _) => {
                                // Remove 2 to the depth to compensate for the fact that a function declaration is higher
                                // than its identifier in the AST
                                helper.scope_stack.push((Scope::Func(name.to_string()), depth - 2));
                            }
                            _ => {}
                        }
                    }
                }
                _ => {}
            }
        } 
        _ => {}
    }

    let children: Vec<_> = ast.data.neighbors(node_idx).collect();
    for &child in children.iter().rev() {
        match node {
            Node::Id(Id::Return) => {
                let (scope, _) = helper.scope_stack.last().ok_or(anyhow!("No scope"))?;

                let exp_ret_type = dict.func.get(scope).ok_or(anyhow!("Function not found"))?
                    .returntype
                    .as_ref()
                    .ok_or(anyhow!("Function has no return type"))?;

                let ret_type = get_expression_type(child, VecDeque::new(), helper, dict, ast)?;

                if *exp_ret_type != ret_type {
                    if let Scope::Func(name) = scope {
                        bail!("Function {} expected {:?} as return type, but got {:?} instead", name, exp_ret_type, ret_type);
                    }
                }
            }
            Node::Id(Id::Assign) => {
                let var_node_idx = children.get(1).ok_or(anyhow!("Assign node has no left hand side child node"))?;
                let var_node = &ast.data[var_node_idx.clone()];

                if let Node::Val(Token::Identifier(name)) = var_node {
                    let (scope, _) = helper.scope_stack.last().ok_or(anyhow!("No scope"))?;

                    let var = dict.var.get(scope).ok_or(anyhow!("Scope not found"))?
                        .get(name.as_str())
                        .ok_or(anyhow!("Found no variable"))?;

                    let exp_type = &var.datatype;

                    let expression_root = children.get(0).ok_or(anyhow!("Assign node has no right hand side child node"))?;
                    let datatype = get_expression_type(*expression_root, VecDeque::new(), helper, dict, ast)?;

                    if *exp_type != datatype {
                        bail!(format!("Invalid assignment: Cannot assign type {:?} to a variable of type {:?}", datatype, exp_type));
                    }
                } else {
                    println!("HERE: {:?}", var_node);
                    bail!("Attempted to assign to something other than a variable")
                }
            }
            Node::Id(Id::If) => {
                let cond_node_idx = children.last().ok_or(anyhow!("'If' node has no children"))?;
                let cond_node = &ast.data[cond_node_idx.clone()];

                let datatype = get_expression_type(*cond_node_idx, VecDeque::new(), helper, dict, ast)?;

                if datatype != DataType::Bool {
                    bail!("The if expression's condition expected type Bool but got type {:?} instead", datatype);
                }
            }
            _ => {
                preorder_traversal(dict, helper, child, ast, depth + 1)?;
            }
        }
    }

    Ok(())
}

pub fn get_expression_type(node: NodeIndex, mut results: VecDeque<DataType>, helper: &Helper, dict: &Dict, ast: &syntax::Tree) -> Result<DataType> {
    let children: Vec<_> = ast.data.neighbors(node).collect();

    for child in children.iter().rev() {
        let result = get_expression_type(*child, VecDeque::new(), helper, dict, ast)?;
        results.push_back(result);
    }

    let node = &ast.data[node];

    match node {
        Node::Val(Token::Number(_)) => Ok(DataType::Int),
        Node::Val(Token::Boolean(_)) => Ok(DataType::Bool),
        Node::Val(Token::String(_)) => Ok(DataType::String),
        Node::Val(Token::Identifier(name)) => {
            let mut datatype: Option<DataType> = None;

            for (scope, _) in helper.scope_stack.iter().rev() {
                let variable = &dict.var.get(scope).ok_or(anyhow!("Scope not found"))?
                    .get(name.as_str());

                if let Some(variable) = variable {
                    datatype = Some(variable.datatype.clone());
                    break;
                }
            }

            Ok(datatype.ok_or(anyhow!("Variable not found"))?)
        }
        Node::Val(Token::Operator(op)) => {
            if results.len() != 2 {
                bail!("Expected two values to operate")
            }

            let lhs = results.pop_front().ok_or(anyhow!("LHS value missing"))?;
            let rhs = results.pop_front().ok_or(anyhow!("RHS value missing"))?;

            match (&lhs, op.as_str(), &rhs) {
                // Arithmetic operations
                (DataType::Int, "+", DataType::Int) |
                (DataType::Int, "-", DataType::Int) |
                (DataType::Int, "*", DataType::Int) |
                (DataType::Int, "/", DataType::Int) => Ok(DataType::Int),

                // String concatenation
                (DataType::String, "+", DataType::String) => Ok(DataType::String),

                // Char ASCII operations
                (DataType::Char, "+", DataType::Int) |
                (DataType::Char, "-", DataType::Int) => Ok(DataType::Char),

                // Boolean logic operations
                (DataType::Bool, "&&", DataType::Bool) |
                (DataType::Bool, "||", DataType::Bool) => Ok(DataType::Bool),

                // Comparison operators (returning Bool)
                (DataType::Int, "=", DataType::Int) |
                (DataType::Int, "<>", DataType::Int) |
                (DataType::Int, "<", DataType::Int) |
                (DataType::Int, "<=", DataType::Int) |
                (DataType::Int, ">", DataType::Int) |
                (DataType::Int, ">=", DataType::Int) |

                (DataType::String, "=", DataType::String) |
                (DataType::String, "<>", DataType::String) |
                (DataType::String, "<", DataType::String) |
                (DataType::String, "<=", DataType::String) |
                (DataType::String, ">", DataType::String) |
                (DataType::String, ">=", DataType::String) |

                (DataType::Char, "=", DataType::Char) |
                (DataType::Char, "<>", DataType::Char) |
                (DataType::Char, "<", DataType::Char) |
                (DataType::Char, "<=", DataType::Char) |
                (DataType::Char, ">", DataType::Char) |
                (DataType::Char, ">=", DataType::Char) |

                (DataType::Bool, "=", DataType::Bool) |
                (DataType::Bool, "<>", DataType::Bool) |
                (DataType::Bool, "<", DataType::Bool) |
                (DataType::Bool, "<=", DataType::Bool) |
                (DataType::Bool, ">", DataType::Bool) |
                (DataType::Bool, ">=", DataType::Bool) => Ok(DataType::Bool),

                _ => bail!(format!("Invalid operation: {:?} {} {:?}", lhs, op, rhs))
            }
        }
        Node::Val(Token::FuncCall(name)) => {
            let func = dict.func.get(&Scope::Func(name.clone())).ok_or(anyhow!("Function not found"))?;

            if results.len() > func.parameters.len() {
                bail!("Function received more arguments than required")
            }

            for (i, (param_type, arg_type)) in func.parameters.iter().zip(results.clone()).enumerate() {
                if *param_type != arg_type {
                    bail!(format!("Parameter {} was expected to be of type {:?} but got {:?} as argument", i, param_type, arg_type));
                }
            }

            if func.parameters.len() > results.len() {
                println!("{:?}", func.parameters);
                bail!(format!("Missing argument of type {:?}", func.parameters.get(results.len())));
            }

            results.clear();

            Ok(func.returntype.clone().ok_or(anyhow!("Function has no return type"))?)
        }
        _ => bail!(format!("Unexpected token: {:?}", node))
    }
}

pub fn run_semantic_analysis(ast: syntax::Tree) -> Result<()> {
    let mut dict = Dict::new();
    let mut helper = Helper::new();

    // Find the tree root by looking for the node with no incoming edges
    let root = ast.data.node_indices().find(|&node| ast.data.neighbors_directed(node, petgraph::Direction::Incoming).count() == 0).unwrap();

    preorder_traversal(&mut dict, &mut helper, root, &ast, 0)?;

    Ok(())
}