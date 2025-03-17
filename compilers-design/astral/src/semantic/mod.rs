pub mod constants;

use crate::prelude::*;
use constants::*;
use anyhow::Result;
use syntax::tree::{ Node, Id };

pub fn run_semantic_analysis(ast: syntax::Tree) -> Result<()> {
    let mut scope_stack = Vec::new();
    let mut act_stack = Vec::new();
    let mut var_dict = VarDict::new();
    let mut temp = String::new();

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

        match node {
            Node::Id(Id::Declarations) => {
                scope_stack.push(Scope::Global);
            }
            Node::Id(id) if *id == Id::Var || *id == Id::Assign => {
                act_stack.push((id, depth));
            }
            Node::Val(val) => {
                match val {
                    Token::Identifier(name) => {
                        if let Some(act) = act_stack.last() {
                            match act {
                                // Variable declaration
                                (Id::Var, _) => {
                                    temp = name.to_string();
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
                                    let scope = scope_stack.last().unwrap();

                                    let variables = var_dict.entry(scope.clone()).or_insert_with(HashSet::new);
                                    let success = variables.insert(Variable::new(temp, data_type.clone()));
                                    if !success { bail!("Duplicated variable") }

                                    temp = String::new();
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

    Ok(())
}