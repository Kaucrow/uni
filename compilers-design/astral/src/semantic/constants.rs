use crate::prelude::*;
use std::hash::{ Hash, Hasher };

pub type VarDict<'a> = HashMap<Scope<'a>, HashSet<Variable>>;

pub type FuncDict<'a> = HashMap<Scope<'a>, FuncDetails>;

#[derive(Debug)]
pub struct FuncDetails {
    pub returntype: Option<DataType>,
    pub parameters: Vec<DataType>,
}

impl FuncDetails {
    pub fn new() -> Self {
        Self {
            returntype: None,
            parameters: Vec::new(),
        }
    }
}

#[derive(Debug)]
pub struct Variable {
    pub name: String,
    pub datatype: DataType,
}

// Implementing PartialEq to compare only by name
impl PartialEq for Variable {
    fn eq(&self, other: &Self) -> bool {
        self.name == other.name
    }
}

// Implementing Eq trait, required when implementing Hash
impl Eq for Variable {}

// Implementing Hash so HashSet can use the name for equality and hashing
impl Hash for Variable {
    fn hash<H: Hasher>(&self, state: &mut H) {
        self.name.hash(state); // Only hash the `name` field
    }
}

impl Variable {
    pub fn new(name: String, datatype: DataType) -> Self {
        Self {
            name,
            datatype,
        }
    }
}

#[derive(PartialEq, Eq, Hash, Clone, Debug)]
pub enum Scope<'a> {
    Global,
    Main,
    Func(&'a String),
}