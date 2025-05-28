use db_load_sim_rs::{
    prelude::*,
    settings::{ self, Settings, },
    mock_data::fill_db,
};
use std::{
    thread,
    time::{ Duration, Instant },
    sync::Arc,
};
use anyhow::Result;
use postgres::{ Client, NoTls };

fn main() -> Result<()> {
    let matches = settings::get_matches();

    // If the "help" option was passed, print the help screen and exit
    if matches.contains_id("help") {
        let mut cmd = settings::get_command();
        cmd.print_help().expect("Failed to print help");
        println!();
        return Ok(());
    }

    // Build the settings from the command line arguments and config file
    let settings = Settings::load(&matches)?;

    // Database connection string
    let conn_string = Arc::new(format!(
        "host={} user={} password={} dbname={}",
        settings.database.host,
        settings.database.user,
        settings.database.password,
        settings.database.dbname
    ));

    if settings.fill_db {
        fill_db(&conn_string, &settings)?;
    }

    // Number of simulated users
    let num_users = settings.num_users;

    // How many times each user will try to connect
    let attempts_per_user = settings.attempts_per_user;

    println!("{} {} {}", "Starting simulation with".cyan(), num_users.to_string().yellow(), "users...".cyan());

    let start_time = Instant::now();
    let mut handles = vec![];

    for user_id in 0..num_users {
        let thread_conn_string = Arc::clone(&conn_string);
        let thread_tablename = settings.database.tablename.clone();

        let handle = thread::spawn(move || {
            let mut successful_connections = 0;
            let mut total_query_time = Duration::new(0, 0);

            for attempt in 0..attempts_per_user {
                let connect_start = Instant::now();
                match Client::connect(&thread_conn_string, NoTls) {
                    Ok(mut client) => {
                        let connect_time = connect_start.elapsed();
                        total_query_time += connect_time;
                        successful_connections += 1;

                        // Simulate a simple query
                        let query_start = Instant::now();
                        match client.query(&format!("SELECT * FROM {}", thread_tablename), &[]) {
                            Ok(_) => {
                                let query_time = query_start.elapsed();
                                total_query_time += query_time;
                                println!(
                                    "{} {} {} {}",
                                    "User".green(),
                                    user_id.to_string().yellow(),
                                    "query successful on attempt".green(),
                                    attempt.to_string().yellow()
                                );
                            }
                            Err(e) => {
                                println!(
                                    "{} {} {} {}{} {}",
                                    "User".red(),
                                    user_id.to_string().yellow(),
                                    "query failed on attempt".red(),
                                    attempt.to_string().yellow(),
                                    ": ".red(),
                                    e.to_string().red(),
                                );
                            }
                        }
                    }
                    Err(e) => {
                        println!(
                            "{} {} {} {}{} {}",
                            "User".red(),
                            user_id.to_string().yellow(),
                            "connection failed on attempt".red(),
                            attempt.to_string().yellow(),
                            ": ".red(),
                            e.to_string().red(),
                        );
                    }
                }
            }

            (successful_connections, total_query_time)
        });
        handles.push(handle);
    }

    // Collect results
    let mut total_successful = 0;
    let mut total_query_time = Duration::new(0, 0);
    for handle in handles {
        let (successful, query_time) = handle.join().unwrap();
        total_successful += successful;
        total_query_time += query_time;
    }

    let total_attempts = num_users * attempts_per_user;
    let success_rate = (total_successful as f64 / total_attempts as f64) * 100.0;
    let avg_query_time = total_query_time.as_millis() as f64 / total_successful as f64;

    println!("{} {}", "\nSimulation completed in".magenta(), format_duration(start_time.elapsed()).to_string().yellow());
    println!("{} {}", "Total connection attempts:".magenta(), total_attempts.to_string().yellow());
    println!(
        "{} {} {}",
        "Successful connections:".magenta(),
        total_successful.to_string().yellow(),
        format!("({:.2}%)", success_rate).yellow()
    );
    println!("{} {}", "Average query time:".magenta(), format!("{:.2}ms", avg_query_time.to_string()).yellow());

    Ok(())
}