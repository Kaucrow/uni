use crate::prelude::*;

#[derive(Debug, PartialEq, Eq, Hash, Clone)]
pub enum TokenType {
    Keyword,
    Identifier,
    Symbol,
    Operator,
    Number,
    String,
    Comment,
}

pub const KEYWORDS: Lazy<HashSet<&'static str>> = Lazy::new(|| HashSet::from([
    // Variables
    "var",
    // Data types
    "integer", "string", "char", "boolean",
    // Flow control
    "begin", "end",
    // Metadata
    "program",
    // Functions
    "procedure", "function",
]));

pub const LEX_DICT: Lazy<Vec<(TokenType, Regex)>> = Lazy::new(|| vec![
    (TokenType::Identifier, Regex::new(r"\b[a-zA-Z_][a-zA-Z0-9_]*\b").unwrap()),
    (TokenType::Symbol, Regex::new(r":=|[();:,.]").unwrap()),
    (TokenType::Operator, Regex::new(r"(:=|[+\-*/])").unwrap()),
    (TokenType::Number, Regex::new(r"\b-?\d+(\.\d+)?\b").unwrap()),
    (TokenType::String, Regex::new(r"'([^']|'')*'").unwrap()),
    (TokenType::Comment, Regex::new(r"\{[^}]*\}").unwrap())
]);