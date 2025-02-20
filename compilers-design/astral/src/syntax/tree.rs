use crate::prelude::*;
use anyhow::Result;

#[derive(Debug, Clone)]
pub enum Node {
    Id(Id),
    Val(Token),
}

//type Node = (Option<NodeType>, String);
//type NodeProto = (Option<NodeType>, Option<&'static str>);

#[derive(Debug, Clone, Copy)]
pub enum Id {
    Program,
    Name,
    Declarations,
    Var,
    FuncDecl,
    Params,
    ReturnType,
    Body,
    Return,
    Main,
    Assign,
    Writeln,
    If,
    Else,
}

pub enum TreeAction {
    AddNode(Option<Node>),
    AppendChild(Option<Node>),
    GoUp,
}

pub struct Tree {
    pub curr_node: Option<NodeIndex>,
    pub data: DiGraph<Node, ()>,
}

impl Tree {
    pub fn new() -> Self {
        Self {
            curr_node: None,
            data: DiGraph::new(),
        }
    }

    pub fn add_node(&mut self, value: Node) {
        if let Some(parent) = self.curr_node {
            self.curr_node = Some(self.data.add_node(value));
            self.data.add_edge(parent, self.curr_node.unwrap(), ());
        } else {
            self.curr_node = Some(self.data.add_node(value));
        }
    }

    pub fn append_child(&mut self, value: Node) -> Result<()> {
        if let Some(parent) = self.curr_node {
            let child = self.data.add_node(value);
            self.data.add_edge(parent, child, ());
            Ok(())
        } else {
            bail!("Failed to append child to AST node: No current node");
        }
    }

    pub fn go_up(&mut self) -> Result<()> {
        if let Some(curr_node) = &mut self.curr_node {
            if let Some(parent) = self.data.neighbors_directed(*curr_node, Direction::Incoming).next() {
                *curr_node = parent;
            }
        }

        Ok(())
    }
}