use suugaku::{
    prelude::*,
    automata::definition::PDA,
    evaluator::evaluate_postfix,
    settings::{ self, Settings },
    lexicon::{ build_ident_map, get_tokens },
};
use std::{
    thread,
    time::Duration,
    sync::{ Arc, Mutex },
};
use anyhow::Result;

fn main() -> Result<()> {
    println!();

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

    // Create a shared state for controlling the spinner
    let done = Arc::new(Mutex::new(false));

    // Clone the Arc to share with the thread
    let done_clone = Arc::clone(&done);

    // Spawn a new thread to spin the spinner indefinitely
    let handle = thread::spawn(move || {
        // Create a spinner with a "waiting" message
        let spinner = ProgressBar::new_spinner();
        spinner.set_style(ProgressStyle::default_spinner()
            .template("{spinner:.green} {msg}").unwrap()
        );
        
        // Set the message for the spinner
        spinner.set_message("Building tokens...");

        while !*done_clone.lock().unwrap() {
            spinner.tick(); // Update the spinner
            thread::sleep(Duration::from_millis(100)); // Simulate work
        }

        // Finish the spinner with a "done" message
        spinner.finish_and_clear();
    });

    // Build the token list
    let tokens = get_tokens(settings.expr)?;

    // Set the done flag to stop the spinner
    {
        let mut done_lock = done.lock().unwrap();
        *done_lock = true;
    }

    // Wait for the spinner thread to finish
    handle.join().unwrap();

    println!(":) Done building tokens!");

    println!("{:?}", &tokens);

    let ident_map = build_ident_map(&tokens)?; 

    println!("{:?}", ident_map);

    let mut pda = PDA::new();

    let postfix = pda.run(tokens)?;

    println!("OUTPUT: {:?}", postfix);

    let result = evaluate_postfix(postfix, ident_map)?;

    println!("RESULT: {}", result);

    Ok(())
}