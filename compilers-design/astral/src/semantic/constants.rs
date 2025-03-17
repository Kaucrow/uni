use crate::prelude::*;

pub type VarDict = HashMap<Scope, HashSet<Variable>>;

pub type FuncDict = HashMap<String, HashSet<Variable>>;

pub struct Variable {
    name: String,
    datatype: DataType,
}


pub enum Scope {
    Global,
    Main,
    Func(String),
}