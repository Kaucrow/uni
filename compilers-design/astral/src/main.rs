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

    // Build Aho-Corasick state machines
    let ahocors: Vec<(TokenProto, AhoCorasick)> = LEX_AHOCOR_DICT.clone().into_iter().map(|(token_proto, matches)| {
        (token_proto, AhoCorasick::new(matches).unwrap())
    }).collect();

    let mut tokens_list: Vec<Vec<Token>> = Vec::new();

    for line in reader.lines() {
        let line = line?.to_lowercase();
        let mut line = line.as_str().trim_start();
        let mut line_tokens: Vec<Token> = Vec::new();

        while !line.is_empty() {
            let mut found_match = false;

            // Try to find a match using Aho-Corasick algorithm
            for (token_proto, ahocor) in &ahocors {
                if let Some(mat) = ahocor.find(&line) {
                    // Ensure the token matched starts at the beginning of the line
                    if mat.start() != 0 {
                        continue;
                    }

                    found_match = true;

                    let mat_str = &line[mat.start()..mat.end()];

                    line_tokens.push(token_proto.to_token(mat_str.to_string()));
                    line = &line[mat.end()..].trim_start();

                    break;
                }
            }

            // If no match was found, try to find a match with Regex
            if !found_match {
                for (token_proto, regex) in LEX_REGEX_DICT.iter() {
                    if let Some(mat) = regex.find(&line) {
                        // Ensure the token matched starts at the beginning of the line
                        if mat.start() != 0 {
                            continue;
                        }

                        found_match = true;

                        let mat_start = mat.start();
                        let mut mat_end= mat.end();

                        let mut token_proto = token_proto.clone();

                        if token_proto == TokenProto::FuncIdent {
                            mat_end -= 1;

                            match line_tokens.last() {
                                Some(Token::Func) => {}
                                Some(Token::Proc) => token_proto = TokenProto::ProcIdent,
                                _ => token_proto = TokenProto::FuncCall,
                            }
                        }

                        let mat_str = &line[mat_start..mat_end];

                        if token_proto != TokenProto::Comment {
                            line_tokens.push(token_proto.to_token(mat_str.to_string()));
                        }
                        line = &line[mat_end..].trim_start();

                        break;
                    }
                }
            }

            if !found_match {
                panic!("{}", format!(
                    "Failed to find a suitable token for the remaining line: `{}`",
                    line
                ));
            }
        }

        tokens_list.push(line_tokens);
    }

    println!("{:?}", tokens_list);

    panic!();

    /*
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
    */

    /*pda.transition(tokens_list.first().unwrap().first().unwrap())?;
    pda.transition(tokens_list.first().unwrap().get(1).unwrap())?;
    pda.transition(tokens_list.first().unwrap().get(2).unwrap())?;
    pda.transition(tokens_list.get(2).unwrap().get(0).unwrap())?;
    */

    Ok(())
}