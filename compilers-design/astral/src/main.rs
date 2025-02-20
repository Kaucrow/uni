use astral::prelude::*;
use astral::settings::{ self, Settings };
use anyhow::Result;
use std::process::Command;

fn open_in_browser(url: &str) {
    if cfg!(target_os = "windows") {
        Command::new("cmd")
            .args(["/C", "start", url]) // Opens in default browser
            .spawn()
            .expect("Failed to open URL");
    } else if cfg!(target_os = "macos") {
        Command::new("open")
            .arg(url)
            .spawn()
            .expect("Failed to open URL");
    } else {
        Command::new("xdg-open")
            .arg(url)
            .spawn()
            .expect("Failed to open URL");
    }
}

fn count_file_lines(filename: &PathBuf) -> io::Result<u64> {
    let file = File::open(filename)?;
    let reader = io::BufReader::new(file);
    Ok(reader.lines().count() as u64)
}

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

    let file_lines = count_file_lines(&settings.file_path)?;

    let lex_bar = ProgressBar::new(file_lines);
    lex_bar.set_style(ProgressStyle::default_bar()
        .template("Analyzing lexic {spinner:.green} [{elapsed_precise}] [{wide_bar:.cyan/blue}] {pos}/{len} ({eta})\n{msg:.green}")
        .unwrap()
        .progress_chars("#>-"));

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

        lex_bar.inc(1);
    }

    lex_bar.finish_with_message("Lexic analysis complete! :)");

    println!("\n\nLexic tokens:");

    for line in &tokens_list {
        println!("{:?}", line);
    }

    println!();

    let mut pda = syntax::PDA::new();

    pda.build();

    let mut ast = syntax::Tree::new();

    let tokens_amt: usize = tokens_list.iter().map(|v| v.len()).sum();

    let syntax_bar = ProgressBar::new(tokens_amt as u64);
    syntax_bar.set_style(ProgressStyle::default_bar()
        .template("Analyzing syntax {spinner:.green} [{elapsed_precise}] [{wide_bar:.cyan/blue}] {pos}/{len} ({eta})\n{msg:.green}")
        .unwrap()
        .progress_chars("#>-"));

    for (i, line) in tokens_list.iter().enumerate() {
        for token in line {
            pda.transition(&token, &mut ast).or_else(|err| {
                eprintln!("{} {}\n", "Syntax error found in line".red(), i + 1);
                eprintln!("{}", "Automata status:".magenta());
                eprintln!("{} {:?}", "- Mode: ".yellow(), pda.mode);
                eprintln!("{} {}", "- State: ".yellow(), pda.state);
                eprintln!("{} {:?}", "- Input: ".yellow(), token);
                eprintln!("{} {:?}\n", "- Stack: ".yellow(), pda.stack);
                eprintln!();
                bail!(err);
                //bail!(format!("{:#?} Mode: {:?}, State: {}, Input: {:?}, Stack: {:?}", err, pda.mode, pda.state, token, pda.stack));
            })?;

            syntax_bar.inc(1);
        }
    }

    syntax_bar.finish_with_message("Syntax analysis complete! :D");

    println!("A browser window with the Abstract Syntactical Tree diagram should open now.\nIf it doesn't, copy and paste the following link in a browser to view it:");

    let ast_dot = format!("{:?}", Dot::with_config(&ast.data, &[Config::EdgeNoLabel]));
    let encoded = encode(&ast_dot);
    let url = format!("https://quickchart.io/graphviz?graph={}", encoded);

    println!("{}", url.green().bold());

    open_in_browser(&url);

    Ok(())
}