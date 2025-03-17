use crate::prelude::*;
use anyhow::Result;

#[derive(Debug, Clone)]
pub enum Node {
    Id(Id),
    Val(Token),
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
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

    pub fn get_preorder_nodes(&self) -> Vec<(NodeIndex, i32)> {
        let tree = &self.data;

        // Find the tree root by looking for the node with no incoming edges
        let root = tree.node_indices().find(|&node| tree.neighbors_directed(node, petgraph::Direction::Incoming).count() == 0).unwrap();

        let mut result = Vec::new();
        let mut stack = vec![(root, 0)];

        while let Some((node, depth)) = stack.pop() {
            result.push((node, depth));
            // Collect children in left-to-right order
            let children: Vec<_> = tree.neighbors(node).collect();
            // Push children in reverse order so the leftmost child is processed first
            for &child in children.iter() {
                stack.push((child, depth + 1));
            }
        }

        result
    }
}