use astral::prelude::*;
use astral::settings::{ self, Settings };
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
    let settings = Settings::new(&matches)?;

    let file = File::open(settings.file_path)?;
    let reader = io::BufReader::new(file);

    let mut tokens_list: Vec<Vec<(TokenType, String)>> = Vec::new();

    for line in reader.lines() {
        let line = line?.to_lowercase();
        let mut line = line.as_str().trim_start();
        let mut line_tokens: Vec<(TokenType, String)> = Vec::new();

        while !line.is_empty() {
            for (token_type, regex) in LEX_DICT.iter() {
                if let Some(matched) = regex.find(&line) {
                    if matched.start() != 0 {
                        continue;
                    }

                    let mut token_type = token_type;

                    if KEYWORDS.contains(matched.as_str()) {
                        token_type = &TokenType::Keyword;
                    }

                    let matched_str = matched.as_str().to_string();

                    line = &line[matched.end()..].trim_start();
                    
                    if token_type != &TokenType::Comment {
                        line_tokens.push((token_type.clone(), matched_str));
                    }

                    break;
                }
            }
        }

        tokens_list.push(line_tokens);
    }

    let mut pda = syntax::PDA::new();

    pda.build();

    let mut ast = syntax::Tree::new();

    for line in tokens_list {
        for token in line {
            pda.transition(&token, &mut ast).or_else(|err| {
                println!("{:?}", Dot::with_config(&ast.data, &[Config::EdgeNoLabel]));
                println!();
                bail!(format!("{:#?} State: {}, Input: {:?}, Stack: {:?}", err, pda.state, token, pda.stack));
            })?;
        }
    }

    /*pda.transition(tokens_list.first().unwrap().first().unwrap())?;
    pda.transition(tokens_list.first().unwrap().get(1).unwrap())?;
    pda.transition(tokens_list.first().unwrap().get(2).unwrap())?;
    pda.transition(tokens_list.get(2).unwrap().get(0).unwrap())?;
    */

    Ok(())
}