use crate::{constants::WebComponent, prelude::*};
use constants::*;
use anyhow::Result;

fn count_file_lines(filename: &PathBuf) -> io::Result<u64> {
    let file = File::open(filename)?;
    let reader = io::BufReader::new(file);
    Ok(reader.lines().count() as u64)
}

fn find_match(line: String, ahocors: &Vec<(Keyword, AhoCorasick)>) -> Option<(Keyword, aho_corasick::Match)> {
    for (keyword, ahocor) in ahocors {
        if let Some(mat) = ahocor.find(&line) {
            return Some((keyword.clone(), mat));
        }
    }

    None
}

pub fn parse_file(settings: &settings::Settings) -> Result<ParsedFile> {
    let file_lines = count_file_lines(&settings.file_path)?;

    let pbar = ProgressBar::new(file_lines);
    pbar.set_style(ProgressStyle::default_bar()
        .template("Parsing source code {spinner:.green} [{elapsed_precise}] [{wide_bar:.cyan/blue}] {pos}/{len} ({eta})\n{msg:.magenta}")
        .unwrap()
        .progress_chars("#>-"));

    let file = File::open(settings.file_path.clone())?;
    let reader = io::BufReader::new(file);
    let mut lines = reader.lines().into_iter();

    // Build Aho-Corasick state machines
    let ahocors: Vec<(Keyword, AhoCorasick)> = AHOCOR_DICT.clone().into_iter().map(|(keyword, matches)| {
        (
            keyword,
            AhoCorasick::new(matches).unwrap()
        )
    }).collect();

    let mut parsed_file = ParsedFile::new();
    let mut javascript: Option<String> = None;

    while let Some(line) = lines.next() {
        let line = line?;

        // Start of component declaration
        if let Some((Keyword::Component, mat)) = find_match(line.trim().to_string(), &ahocors) {
            if mat.start() == 0 {
                let component_name = &line[mat.end()..];
                parse_component(&mut parsed_file, &mut javascript, &mut lines, &ahocors, component_name.trim())?;
            }
        } else {
            // If there's already javascript code parsed, append to it
            if let Some(code) = javascript {
                javascript = Some(code + "\n" + &line);
            } else {
                javascript = Some(line + "\n");
            }
        }

        pbar.inc(1);
    }

    // Push any remaining javascript code
    if let Some(code) = javascript.take() {
        parsed_file.push(CodeBlock::Javascript(code));
    }

    pbar.finish_with_message("Parsing complete! :)");

    Ok(parsed_file)
}

fn parse_component(
    parsed_file: &mut Vec<CodeBlock>,
    javascript: &mut Option<String>,
    lines: &mut Lines<BufReader<File>>,
    ahocors: &Vec<(Keyword, AhoCorasick)>,
    component_name: &str,
) -> Result<()> {
    if let Some(code) = javascript.take() {
        parsed_file.push(CodeBlock::Javascript(code));
    }

    println!("Found the component {:?} NAME: {}", parsed_file, component_name);
    println!();

    let mut component = WebComponent::new(component_name.to_string());

    let mut lbracket_count: Option<usize> = None;
    let mut parsing_directive: Option<Keyword> = None;

    while let Some(line) = lines.next() {
        let line = line?;

        if let (Some(directive), Some(count)) = (&parsing_directive, &mut lbracket_count) {
            *count += &line.chars().filter(|&c| c.to_string() == "{").count();
            *count -= &line.chars().filter(|&c| c.to_string() == "}").count();

            // Exit
            if *count == 0 {
                lbracket_count = None;
                parsing_directive = None;
                println!("Exit on: {}", line);
                continue;
            }

            match directive {
                Keyword::Style => {
                    component.style.get_or_insert_with(|| String::new());

                    component.style.as_mut().unwrap().push_str(&line);
                    component.style.as_mut().unwrap().push('\n');
                }
                Keyword::Template => {
                    component.template.get_or_insert_with(|| String::new());

                    component.template.as_mut().unwrap().push_str(&line);
                    component.template.as_mut().unwrap().push('\n');
                }
                _ => bail!("Unexpected keyword: {:?}", parsing_directive),
            }
        } else if !line.trim().is_empty() {
            let (keyword, mat) = find_match(line.to_string(), ahocors).ok_or(anyhow!(format!(
                "Expected keyword: {}", line
            )))?;

            // If the component end is found, break. Otherwise, a directive block was found
            match keyword {
                Keyword::End => break,
                Keyword::Var => {
                    let split: Vec<&str> = (&line[mat.end()..]).trim().split_whitespace().collect();
                    if split.len() != 3 {
                        bail!("Syntax error on var directive")
                    }

                    let mut split = split.into_iter();

                    let name = split.next().ok_or(anyhow!("Missing variable name"))?;
                    let eq = split.next().ok_or(anyhow!("Missing equal"))?;
                    if eq != "=" { bail!("Missing equal") }
                    let value = split.next().ok_or(anyhow!("Missing value"))?;

                    if component.variables.as_ref().and_then(|vars| vars.get(name)).is_some() {
                        bail!("Variable already exists");
                    }

                    let variables = component.variables.get_or_insert_with(HashMap::new);
                    variables.insert(name.into(), value.into());
                }
                Keyword::Bind => {
                    let split: Vec<&str> = (&line[mat.end()..]).trim().split_whitespace().collect();
                    if split.len() != 3 {
                        bail!("Syntax error on bind directive")
                    }

                    let mut split = split.into_iter();

                    let name = split.next().ok_or(anyhow!("Missing variable name"))?;
                    let to = split.next().ok_or(anyhow!("Missing 'to'"))?;
                    if to != "to" { bail!("Missing 'to'") }
                    let binding = split.next().ok_or(anyhow!("Missing binding"))?;

                    if component.variables.as_ref().map_or(true, |vars| vars.get(name).is_none()) {
                        bail!("Variable has not been defined");
                    }

                    if component.bindings.as_ref().and_then(|vars| vars.get(name)).is_some() {
                        bail!("Variable has already been bound");
                    }

                    let bindings = component.bindings.get_or_insert_with(HashMap::new);
                    bindings.insert(name.into(), binding.into());
                }
                _ if &line[mat.end()..].trim() == &"{" => {
                    lbracket_count = Some(1);
                    parsing_directive = Some(keyword);
                }
                _ => bail!("Failed to parse the keyword")
            }
        }
    }

    parsed_file.push(CodeBlock::Component(component));

    Ok(())
}