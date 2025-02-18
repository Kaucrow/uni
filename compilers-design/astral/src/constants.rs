use crate::prelude::*;

#[derive(PartialEq, Clone)]
pub enum TokenProto {
    // General keywords
    Var,
    Begin,
    End,
    Program,
    Proc,
    ProcIdent,
    Func,
    FuncIdent,
    FuncCall,
    DataType,
    // Built-in functions
    BuiltinFunc,
    // Symbols
    LParen,
    RParen,
    Assignment,
    Operator,
    Semicolon,
    Colon,
    Comma,
    Dot,
    // Other
    Identifier,
    Number,
    String,
    Comment,
}

#[derive(Debug, PartialEq, Eq, Hash, Clone)]
pub enum Token {
    // General keywords
    Var,
    Begin,
    End,
    Program,
    Proc,
    ProcIdent(String),
    Func,
    FuncIdent(String),
    FuncCall(String),
    DataType(String),
    // Built-in functions
    BuiltinFunc(String),
    // Symbols
    LParen,
    RParen,
    Assignment,
    Operator(String),
    Semicolon,
    Colon,
    Comma,
    Dot,
    // Other
    Identifier(String),
    Number,
    String,
    Comment,
}

impl TokenProto {
    pub fn to_token(&self, input: String) -> Token {
        match self {
            TokenProto::Var => Token::Var,
            TokenProto::Begin => Token::Begin,
            TokenProto::End => Token::End,
            TokenProto::Program => Token::Program,
            TokenProto::Proc => Token::Proc,
            TokenProto::ProcIdent => Token::ProcIdent(input),
            TokenProto::Func => Token::Func,
            TokenProto::FuncIdent => Token::FuncIdent(input),
            TokenProto::FuncCall => Token::FuncCall(input),
            TokenProto::DataType => Token::DataType(input),
            TokenProto::BuiltinFunc => Token::BuiltinFunc(input),
            TokenProto::LParen => Token::LParen,
            TokenProto::RParen => Token::RParen,
            TokenProto::Assignment => Token::Assignment,
            TokenProto::Operator => Token::Operator(input),
            TokenProto::Semicolon => Token::Semicolon,
            TokenProto::Colon => Token::Colon,
            TokenProto::Comma => Token::Comma,
            TokenProto::Dot => Token::Dot,
            TokenProto::Identifier => Token::Identifier(input),
            TokenProto::Number => Token::Number,
            TokenProto::String => Token::String,
            TokenProto::Comment => Token::Comment,
        }
    }
}

pub const LEX_AHOCOR_DICT: Lazy<Vec<(TokenProto, Vec<&'static str>)>> = Lazy::new(|| vec![
    (TokenProto::DataType, vec!["integer", "string", "char", "boolean"]),
    (TokenProto::Program, vec!["program"]),
    (TokenProto::BuiltinFunc, vec!["writeln"]),
    (TokenProto::LParen, vec!["("]),
    (TokenProto::RParen, vec![")"]),
    (TokenProto::Assignment, vec![":="]),
    (TokenProto::Operator, vec!["+", "-", "*", "/"]),
    (TokenProto::Semicolon, vec![";"]),
    (TokenProto::Colon, vec![":"]),
    (TokenProto::Comma, vec![","]),
    (TokenProto::Dot, vec!["."]),
    (TokenProto::Func, vec!["function"]),
    (TokenProto::Proc, vec!["process"]),
    (TokenProto::Var, vec!["var"]),
    (TokenProto::Begin, vec!["begin"]),
    (TokenProto::End, vec!["end"]),
]);

pub const LEX_REGEX_DICT: Lazy<Vec<(TokenProto, Regex)>> = Lazy::new(|| vec![
    (TokenProto::FuncIdent, Regex::new(r"\b[a-zA-Z_][a-zA-Z0-9_]*\(").unwrap()),
    (TokenProto::Identifier, Regex::new(r"\b[a-zA-Z_][a-zA-Z0-9_]*\b").unwrap()),
    (TokenProto::Number, Regex::new(r"\b-?\d+(\.\d+)?\b").unwrap()),
    (TokenProto::String, Regex::new(r"'([^']|'')*'").unwrap()),
    (TokenProto::Comment, Regex::new(r"\{[^}]*\}").unwrap())
]);