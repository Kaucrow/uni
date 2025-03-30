use alkemy::prelude::*;
use anyhow::Result;

fn main() -> Result<()> {
    let matches = settings::get_matches();

    // If the "help" option was passed, print the help screen and exit
    if matches.contains_id("help") {
        let mut cmd = settings::get_command();
        cmd.print_help().expect("Failed to print help");
        println!();
        return Ok(());
    }

    // Build the settings from the command line arguments
    let settings = settings::Settings::new(&matches)?;

    let parsed_file = parser::parse_file(&settings)?;

    println!("Parsed file: {:#?}", &parsed_file);

    let transpiled_code = generator::generate_js(parsed_file)?;

    println!("Transpiled code:");
    println!("{}", transpiled_code);

    Ok(())
}