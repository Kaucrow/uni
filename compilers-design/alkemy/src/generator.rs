use crate::prelude::*;
use constants::*;
use anyhow::Result;
use minijinja::{ Environment, context };

fn pascal_to_kebab(name: &str) -> String {
    let mut result = String::new();
    let mut prev_lowercase = false;

    for c in name.chars() {
        if c.is_uppercase() {
            if prev_lowercase { 
                result.push('-'); 
            }
            result.push(c.to_ascii_lowercase());
            prev_lowercase = false;
        } else {
            result.push(c);
            prev_lowercase = true;
        }
    }

    result
}

pub fn generate_js(parsed_file: ParsedFile) -> Result<String> {
    // Read the template file
    let template_content = fs::read_to_string("./templates/component")?;

    let mut env = Environment::new();

    env.add_template("component", &template_content).unwrap();
    let tmpl = env.get_template("component").unwrap();

    let mut transpiled_code = String::new();

    for block in parsed_file {
        match block {
            CodeBlock::Javascript(code) => transpiled_code.push_str(&code),
            CodeBlock::Component(component) => {
                let tagname = pascal_to_kebab(&component.name);

                transpiled_code.push_str(format!("{}", tmpl.render(context!(
                    name => component.name,
                    style => component.style.unwrap_or("".to_string()),
                    template => component.template.unwrap_or("".to_string()),
                    tagname,
                )).unwrap()).as_str())
            }
        }
    }

    Ok(transpiled_code)
}