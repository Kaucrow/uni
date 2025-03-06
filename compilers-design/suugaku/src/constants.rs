use crate::prelude::*;

#[derive(PartialEq, Eq, Clone)]
pub enum TokenProto {
    // Symbols
    LParen,
    RParen,
    Operator,
    Semicolon,
    // Other
    Identifier,
    Number,
    EOF,
}

#[derive(Debug, Clone)]
pub enum Token {
    // Symbols
    LParen,
    RParen,
    Operator(String),
    Semicolon,
    // Other
    Identifier(String),
    Number(String),
    EOF,
}

impl TokenProto {
    pub fn to_token(&self, input: String) -> Token {
        match self {
            TokenProto::LParen => Token::LParen,
            TokenProto::RParen => Token::RParen,
            TokenProto::Operator => Token::Operator(input),
            TokenProto::Semicolon => Token::Semicolon,
            TokenProto::Identifier => Token::Identifier(input),
            TokenProto::Number => Token::Number(input),
            TokenProto::EOF => Token::EOF,
        }
    }
}

impl Token {
    pub fn proto(&self) -> TokenProto {
        match self {
            Token::LParen => TokenProto::LParen,
            Token::RParen => TokenProto::RParen,
            Token::Operator(_) => TokenProto::Operator,
            Token::Semicolon => TokenProto::Semicolon,
            Token::Identifier(_) => TokenProto::Identifier,
            Token::Number(_) => TokenProto::Number,
            Token::EOF => TokenProto::EOF,
        }
    }
}

pub const AHOCOR_DICT: Lazy<Vec<(TokenProto, Vec<&'static str>)>> = Lazy::new(|| vec![
    (TokenProto::LParen, vec!["("]),
    (TokenProto::RParen, vec![")"]),
    (TokenProto::Operator, vec!["+", "-", "*", "/"])
]);

pub const REGEX_DICT: Lazy<Vec<(TokenProto, Regex)>> = Lazy::new(|| vec![
    (TokenProto::Identifier, Regex::new(r"\b[a-zA-Z_][a-zA-Z0-9_]*\b").unwrap()),
    (TokenProto::Number, Regex::new(r"\b-?\d+(\.\d+)?\b").unwrap()),
]);

pub type IdentMap = HashMap<String, Option<f64>>;