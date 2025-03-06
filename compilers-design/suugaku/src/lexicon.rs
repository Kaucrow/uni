use crate::prelude::*;
use anyhow::Result;

pub fn get_tokens(expr: String) -> Result<Vec<Token>> {
    let mut tokens_list: Vec<Token> = Vec::new();

    let mut expr = expr.as_str().trim_start();

    // Build Aho-Corasick state machines
    let ahocors: Vec<(TokenProto, AhoCorasick)> = AHOCOR_DICT.clone().into_iter().map(|(token_proto, matches)| {
        (token_proto, AhoCorasick::new(matches).unwrap())
    }).collect();

    while !expr.is_empty() {
        let mut found_match = false;

        // Try to find a match using Aho-Corasick algorithm
        for (token_proto, ahocor) in &ahocors {
            if let Some(mat) = ahocor.find(&expr) {
                // Ensure the token matched starts at the beginning of the line
                if mat.start() != 0 {
                    continue;
                }

                found_match = true;

                let mat_str = &expr[mat.start()..mat.end()];

                tokens_list.push(token_proto.to_token(mat_str.to_string()));
                expr = &expr[mat.end()..].trim_start();

                break;
            }
        }

        // If no match was found, try to find a match with Regex
        if !found_match {
            for (token_proto, regex) in REGEX_DICT.iter() {
                if let Some(mat) = regex.find(&expr) {
                    // Ensure the token matched starts at the beginning of the line
                    if mat.start() != 0 {
                        continue;
                    }

                    found_match = true;

                    let mat_start = mat.start();
                    let mat_end = mat.end();

                    let mat_str = &expr[mat_start..mat_end];

                    tokens_list.push(token_proto.to_token(mat_str.to_string()));
                    expr = &expr[mat_end..].trim_start();

                    break;
                }
            }
        }

        if !found_match {
            panic!("{}", format!(
                "Failed to find a suitable token for the remaining line: `{}`",
                expr
            ));
        }
    }

    tokens_list.push(Token::EOF);

    Ok(tokens_list)
}

pub fn build_ident_map(tokens: &Vec<Token>) -> Result<IdentMap> {
    let mut ident_map = IdentMap::new();

    for token in tokens {
        // If the token is an identifier
        if let Token::Identifier(identifier) = token {
            // And the identifier isn't already on the map
            if ident_map.get(identifier).is_none() {
                // Get its value
                print!("Input value for {}: ", identifier.cyan());
                io::stdout().flush()?;

                let mut input = String::new();
                io::stdin().read_line(&mut input).expect("Failed to read input");

                match input.trim().parse::<f64>() {
                    Ok(num) => ident_map.insert(identifier.into(), Some(num)),
                    Err(_) => bail!("Error: Input is not a valid number!"),
                };
            }
        }
    }

    Ok(ident_map)
}