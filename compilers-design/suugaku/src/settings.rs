use crate::prelude::*;
use anyhow::Result;
use clap::{ Arg, Command, ArgMatches };

pub struct Settings {
    pub expr: String,
}

impl Settings {
    pub fn new(matches: &ArgMatches) -> Result<Self> {
        let expr = matches
            .get_one::<String>("expression")
            .cloned()
            .ok_or(anyhow!("Unexpected value for `expr` argument"))?;

        Ok(Self {
            expr
        })
    }
}

pub fn get_command() -> Command {
    Command::new("Arithmetic expression analyzer")
        .version("0.1.0")
        .author("Kaucrow")
        .about("Evaluates arithmetic expressions and returns the result")
        .arg(
            Arg::new("expression")
                .short('e')
                .long("expr")
                .num_args(1)
                .required(true)
                .help("Arithmetic expression to analyze")
        )
}

pub fn get_matches() -> ArgMatches {
    get_command().get_matches()
}