use crate::prelude::*;
use anyhow::Result;
use clap::{ Arg, Command, ArgMatches };

pub struct Settings {
    pub file_path: PathBuf,
}

impl Settings {
    pub fn new(matches: &ArgMatches) -> Result<Self> {
        let file_path_str = matches
            .get_one::<String>("path")
            .cloned()
            .ok_or(anyhow!("Unexpected value for `path` argument"))?;

        let file_path = PathBuf::try_from(file_path_str)?;

        if !file_path.is_file() {
            bail!("The provided path is not a valid file");
        }

        Ok(Self {
            file_path,
        })
    }
}

pub fn get_command() -> Command {
    Command::new("Web Components Transpiler")
        .version("0.1.0")
        .author("Kaucrow")
        .about(r#"Transpiler which converts a made-up language called Alkemy
into Javascript for building Web Components quickly and efficiently"#
        )
        .arg(
            Arg::new("path")
                .short('p')
                .long("path")
                .num_args(1)
                .required(true)
                .help("Alkemy source code file path")
        )
}

pub fn get_matches() -> ArgMatches {
    get_command().get_matches()
}