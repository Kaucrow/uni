use crate::prelude::*;

#[derive(Debug)]
pub enum CodeBlock {
    Javascript(String),
    Component(WebComponent),
}

#[derive(Debug)]
pub struct WebComponent {
    pub name: String,
    pub style: Option<String>,
    pub template: Option<String>,
}

impl WebComponent {
    pub fn new(name: String) -> Self {
        Self {
            name,
            style: None,
            template: None,
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
}

pub const AHOCOR_DICT: Lazy<Vec<(Keyword, Vec<&'static str>)>> = Lazy::new(|| vec![
    (Keyword::Component, vec!["@component"]),
    (Keyword::End, vec!["@end"]),
    (Keyword::Style, vec!["style"]),
    (Keyword::Template, vec!["template"]),
]);