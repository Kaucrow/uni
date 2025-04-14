use crate::prelude::*;

#[derive(Debug)]
pub enum CodeBlock {
    Javascript(String),
    Component(WebComponent),
}

#[derive(Debug)]
pub struct Callback {
    pub elem: String,
    pub trigger: String,
    pub action: String,
}

#[derive(Debug)]
pub struct WebComponent {
    pub name: String,
    pub style: Option<String>,
    pub template: Option<String>,
    pub variables: Option<HashMap<String, String>>, // Var name, value
    pub bindings: Option<HashMap<String, String>>,  // Var name, binding
    pub callbacks: Option<Vec<Callback>>,
}

impl WebComponent {
    pub fn new(name: String) -> Self {
        Self {
            name,
            style: None,
            template: None,
            variables: None,
            bindings: None,
            callbacks: None,
        }
    }
}

pub type ParsedFile = Vec<CodeBlock>;

#[derive(Debug, Clone)]
pub enum Keyword {
    Component,
    End,
    Style,
    Template,
    Var,
    Bind,
    Callback,
}

pub const AHOCOR_DICT: Lazy<Vec<(Keyword, Vec<&'static str>)>> = Lazy::new(|| vec![
    (Keyword::Component, vec!["@component"]),
    (Keyword::End, vec!["@end"]),
    (Keyword::Style, vec!["style"]),
    (Keyword::Template, vec!["template"]),
    (Keyword::Var, vec!["var"]),
    (Keyword::Bind, vec!["bind"]),
    (Keyword::Callback, vec!["on"]),
]);