use crate::prelude::*;
use anyhow::Result;
use clap::{ Arg, Command, ArgMatches, ArgAction };
use config::{ Config, File };
use serde::Deserialize;

#[derive(Debug, Deserialize)]
pub struct Settings {
    pub fill_db: bool,
    pub num_users: i32,
    pub attempts_per_user: i32,
    pub database: DatabaseSettings,
}

#[derive(Debug, Deserialize)]
pub struct DatabaseSettings {
    pub host: String,
    pub user: String,
    pub password: String,
    pub dbname: String,
    pub tablename: String,
}

struct CliSettings {
    fill_db: bool,
    num_users: i32,
    attempts_per_user: i32,
}

#[derive(Debug, Deserialize)]
struct FileSettings {
    database: DatabaseSettings,
}

impl Settings {
    pub fn load(matches: &ArgMatches) -> Result<Self> {
        let config_path = "./config/config.toml";

        let file_settings= Config::builder()
            .add_source(File::with_name(config_path))
            .build()?
            .try_deserialize()?;

        let cli_settings = CliSettings {
            fill_db: matches.get_flag("fill_db"),
            num_users: matches
                .get_one::<String>("num_users")
                .context("Missing num_users")?
                .parse::<i32>()
                .context("Failed to parse num_users")?,
            attempts_per_user: matches
                .get_one::<String>("user_attempts")
                .map_or(Ok(1), |n| n.parse())?,
        };

        Ok(Self::merge(cli_settings, file_settings))
    }

    fn merge(cli: CliSettings, file: FileSettings) -> Self {
        Self {
            fill_db: cli.fill_db,
            num_users: cli.num_users,
            attempts_per_user: cli.attempts_per_user,
            database: file.database,
        }
    }
}

pub fn get_command() -> Command {
    Command::new("DB Load Simulator")
        .version("0.1.0")
        .author("Kaucrow")
        .about("A database load simulator made for showcasing the usefulness of a connection pool.")
        .arg(
            Arg::new("fill_db")
                .short('f')
                .long("fill_db")
                .action(ArgAction::SetTrue)
                .help("Whether or not to try filling the DB with mock data."),
        )
        .arg(
            Arg::new("num_users")
                .required(true)
                .short('u')
                .long("num_users")
                .num_args(1)
                .help("Number of user connections to simulate."),
        )
        .arg(
            Arg::new("user_attempts")
                .short('a')
                .long("user_attempts")
                .num_args(1)
                .help("How many attempts will each user perform."),
        )
}

pub fn get_matches() -> ArgMatches {
    get_command().get_matches()
}