use crate::prelude::*;
use std::str::FromStr;

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
    If,
    Then,
    Else,
    // Built-in functions
    Writeln,
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
    Boolean,
    Char,
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
    DataType(DataType),
    If,
    Then,
    Else,
    // Built-in functions
    Writeln(String),
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
    Number(String),
    String(String),
    Char(String),
    Boolean(String),
    Comment(String),
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
            TokenProto::DataType => Token::DataType(DataType::from_str(&input).unwrap()),
            TokenProto::If => Token::If,
            TokenProto::Then => Token::Then,
            TokenProto::Else => Token::Else,
            TokenProto::Writeln => Token::Writeln(input),
            TokenProto::LParen => Token::LParen,
            TokenProto::RParen => Token::RParen,
            TokenProto::Assignment => Token::Assignment,
            TokenProto::Operator => Token::Operator(input),
            TokenProto::Semicolon => Token::Semicolon,
            TokenProto::Colon => Token::Colon,
            TokenProto::Comma => Token::Comma,
            TokenProto::Dot => Token::Dot,
            TokenProto::Identifier => Token::Identifier(input),
            TokenProto::Number => Token::Number(input),
            TokenProto::String => Token::String(input),
            TokenProto::Boolean => Token::Boolean(input),
            TokenProto::Char => Token::Char(input),
            TokenProto::Comment => Token::Comment(input),
        }
    }
}

impl Token {
    pub fn proto(&self) -> TokenProto {
        match self {
            Token::Var => TokenProto::Var,
            Token::Begin => TokenProto::Begin,
            Token::End => TokenProto::End,
            Token::Program => TokenProto::Program,
            Token::Proc => TokenProto::Proc,
            Token::ProcIdent(_) => TokenProto::ProcIdent,
            Token::Func => TokenProto::Func,
            Token::FuncIdent(_) => TokenProto::FuncIdent,
            Token::FuncCall(_) => TokenProto::FuncCall,
            Token::DataType(_) => TokenProto::DataType,
            Token::If => TokenProto::If,
            Token::Then => TokenProto::Then,
            Token::Else => TokenProto::Else,
            Token::Writeln(_) => TokenProto::Writeln,
            Token::LParen => TokenProto::LParen,
            Token::RParen => TokenProto::RParen,
            Token::Assignment => TokenProto::Assignment,
            Token::Operator(_) => TokenProto::Operator,
            Token::Semicolon => TokenProto::Semicolon,
            Token::Colon => TokenProto::Colon,
            Token::Comma => TokenProto::Comma,
            Token::Dot => TokenProto::Dot,
            Token::Identifier(_) => TokenProto::Identifier,
            Token::Number(_) => TokenProto::Number,
            Token::String(_) => TokenProto::String,
            Token::Boolean(_) => TokenProto::Boolean,
            Token::Char(_) => TokenProto::Char,
            Token::Comment(_) => TokenProto::Comment,
        }
    }
}

pub const LEX_AHOCOR_DICT: Lazy<Vec<(TokenProto, Vec<&'static str>)>> = Lazy::new(|| vec![
    (TokenProto::DataType, vec!["integer", "string", "char", "boolean"]),
    (TokenProto::Program, vec!["program"]),
    (TokenProto::Writeln, vec!["writeln"]),
    (TokenProto::LParen, vec!["("]),
    (TokenProto::RParen, vec![")"]),
    (TokenProto::Assignment, vec![":="]),
    (TokenProto::Operator, vec!["+", "-", "*", "/"]),
    (TokenProto::Operator, vec!["<>", "<=", ">="]),
    (TokenProto::Operator, vec!["=", "<", ">", "&&", "||"]),
    (TokenProto::Semicolon, vec![";"]),
    (TokenProto::Colon, vec![":"]),
    (TokenProto::Comma, vec![","]),
    (TokenProto::Dot, vec!["."]),
    (TokenProto::Func, vec!["function"]),
    (TokenProto::Proc, vec!["process"]),
    (TokenProto::Var, vec!["var"]),
    (TokenProto::Begin, vec!["begin"]),
    (TokenProto::End, vec!["end"]),
    (TokenProto::Boolean, vec!["true", "false"]),
    (TokenProto::If, vec!["if"]),
    (TokenProto::Then, vec!["then"]),
    (TokenProto::Else, vec!["else"]),
]);

pub const LEX_REGEX_DICT: Lazy<Vec<(TokenProto, Regex)>> = Lazy::new(|| vec![
    (TokenProto::FuncIdent, Regex::new(r"\b[a-zA-Z_][a-zA-Z0-9_]*\(").unwrap()),
    (TokenProto::Identifier, Regex::new(r"\b[a-zA-Z_][a-zA-Z0-9_]*\b").unwrap()),
    (TokenProto::Number, Regex::new(r"\b-?\d+(\.\d+)?\b").unwrap()),
    (TokenProto::Char, Regex::new(r"'([^']|''{1})'").unwrap()),
    (TokenProto::String, Regex::new(r"'([^']|'')*'").unwrap()),
    (TokenProto::Comment, Regex::new(r"\{[^}]*\}").unwrap())
]);

#[derive(Debug, PartialEq, Eq, Hash, Clone)]
pub enum DataType {
    Int,
    String,
    Char,
    Bool,
}

impl FromStr for DataType {
    type Err = anyhow::Error;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        match s {
            "integer" => Ok(DataType::Int),
            "string" => Ok(DataType::String),
            "char" => Ok(DataType::Char),
            "boolean" => Ok(DataType::Bool),
            _ => bail!(format!("'{}' is not a valid datatype", s))
        }
    }
}