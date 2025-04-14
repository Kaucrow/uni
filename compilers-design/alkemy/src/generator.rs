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
    let component_tmpl_content = fs::read_to_string("./templates/component")?;
    let reactive_tmpl_content = fs::read_to_string("./templates/reactive")?;
    let callback_tmpl_content = fs::read_to_string("./templates/callback")?;

    let mut env = Environment::new();

    env.add_template("component", &component_tmpl_content).unwrap();
    env.add_template("reactive", &reactive_tmpl_content).unwrap();
    env.add_template("callback", &callback_tmpl_content).unwrap();
    let component_tmpl = env.get_template("component").unwrap();
    let reactive_tmpl = env.get_template("reactive").unwrap();
    let callback_tmpl = env.get_template("callback").unwrap();

    let mut transpiled_code = String::new();

    for block in parsed_file {
        match block {
            CodeBlock::Javascript(code) => transpiled_code.push_str(&code),
            CodeBlock::Component(component) => {
                let mut variables = String::new();

                for (name, val) in component.variables.unwrap_or(HashMap::new()) {
                    if let Some(ref bindings) = component.bindings {
                        // If the variable has a binding, make a setter and getter to make it reactive
                        if let Some(binding) = bindings.get(&name) {
                            let element = &binding[0..binding.find(".").ok_or(anyhow!("The binding is invalid"))?];

                            variables.push_str(format!("{}", reactive_tmpl.render(context!(
                                element,
                                name,
                                binding,
                            )).unwrap()).as_str())
                        }
                    }

                    // Add the variable declaration
                    variables.push_str(&format!("this.{} = {};", name, val));
                }

                let mut callbacks = String::new();

                for callback in component.callbacks.unwrap_or(Vec::new()) {
                    callbacks.push_str(format!("{}", callback_tmpl.render(context!(
                        elem => callback.elem,
                        trigger => callback.trigger,
                        action => callback.action,
                    )).unwrap()).as_str())
                }

                let tagname = pascal_to_kebab(&component.name);

                transpiled_code.push_str(format!("{}", component_tmpl.render(context!(
                    name => component.name,
                    style => component.style.unwrap_or("".to_string()),
                    template => component.template.unwrap_or("".to_string()),
                    variables,
                    callbacks,
                    tagname,
                )).unwrap()).as_str())
            }
        }
    }

    Ok(transpiled_code)
}