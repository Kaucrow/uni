use anyhow::Result;
use config::{ Config, File };
use serde::Deserialize;

#[derive(Debug, Deserialize)]
pub struct Settings {
    pub ci: String,
}

impl Settings {
    pub fn load() -> Result<Self> {
        let config_path = "./config/config.toml";

        let file_settings: Settings = Config::builder()
            .add_source(File::with_name(config_path))
            .build()?
            .try_deserialize()?;

        Ok(file_settings)
    }
}