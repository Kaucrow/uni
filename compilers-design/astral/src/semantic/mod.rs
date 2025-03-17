pub mod constants;

use crate::prelude::*;
use constants::*;
use anyhow::Result;
use syntax::tree::{ Node, Id };

pub fn run_semantic_analysis(ast: syntax::Tree) -> Result<()> {
    let mut scope_stack = Vec::new();
    let mut act_stack = Vec::new();
    let mut var_dict = VarDict::new();
    let mut func_dict = FuncDict::new();
    let mut temp_var = String::new();

    let preorder_nodes = ast.get_preorder_nodes();
 
    for (node, depth) in preorder_nodes {
        let node = &ast.data[node];
        println!("{:?} - {}", node, depth);

        // Clear up stack actions that have finished
        while let Some((_, act_depth)) = act_stack.last() {
            if depth <= *act_depth {    // If we're moving up the tree
                act_stack.pop(); // Remove the last action
            } else {
                break; // Stop when we reach a deeper node
            }
        }

        // Clear up scopes that have finished
        while let Some((_, scope_depth)) = scope_stack.last() {
            if depth <= *scope_depth {    // If we're moving up the tree
                scope_stack.pop(); // Remove the last action
            } else {
                break; // Stop when we reach a deeper node
            }
        }

        match node {
            Node::Id(Id::Declarations) => {
                scope_stack.push((Scope::Global, depth));
            }
            Node::Id(id) if *id == Id::Var || *id == Id::Assign || *id == Id::FuncDecl => {
                act_stack.push((id, depth));
            }
            Node::Val(val) => {
                match val {
                    Token::Identifier(name) => {
                        if let Some(act) = act_stack.last() {
                            match act {
                                // Variable declaration
                                (Id::Var, _) | (Id::FuncDecl, _) => {
                                    temp_var = name.to_string();
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
                                    let (scope, _) = scope_stack.last().unwrap();

                                    let variables = var_dict.entry(scope.clone()).or_insert_with(HashSet::new);
                                    let success = variables.insert(Variable::new(temp_var, data_type.clone()));
                                    if !success { bail!("Duplicated variable") }

                                    temp_var = String::new();
                                }
                                // Function declaration parameters
                                (Id::FuncDecl, _) => {
                                    let (scope, _) = scope_stack.last().unwrap();

                                    // The node is the return type
                                    if temp_var.is_empty() {
                                        let func = func_dict.entry(scope.clone()).or_insert_with(FuncDetails::new);
                                        func.returntype = Some(data_type.clone());
                                    // The node is a parameter
                                    } else {
                                        let func = func_dict.entry(scope.clone()).or_insert_with(FuncDetails::new);
                                        func.parameters.push(data_type.clone());

                                        let variables = var_dict.entry(scope.clone()).or_insert_with(HashSet::new);
                                        let success = variables.insert(Variable::new(temp_var, data_type.clone()));
                                        if !success { bail!("Duplicated variable") }
                                        
                                        temp_var = String::new();
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
                                    scope_stack.push((Scope::Func(name), depth - 2));
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
    }

    println!("{:#?}", var_dict);
    println!("{:#?}", func_dict);

    Ok(())
}