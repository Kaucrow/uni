use postgres::{ Client, NoTls };
use std::thread;
use std::time::{ Duration, Instant };
use rand::Rng;

fn main() {
    // Database connection string
    let conn_string = "host=localhost user=postgres password=postgrespass dbname=test";

    // Number of simulated users
    let num_users = 1000;

    // How many times each user will try to connect
    let attempts_per_user = 10;

    println!("Starting simulation with {} users...", num_users);

    let start_time = Instant::now();
    let mut handles = vec![];

    for user_id in 0..num_users {
        let conn_string = conn_string.to_string();
        let handle = thread::spawn(move || {
            let mut rng = rand::rng();
            let mut successful_connections = 0;
            let mut total_query_time = Duration::new(0, 0);

            for attempt in 0..attempts_per_user {
                // Random delay between attempts (0-100ms)
                let delay = rng.random_range(0..100);
                thread::sleep(Duration::from_millis(delay));

                let connect_start = Instant::now();
                match Client::connect(&conn_string, NoTls) {
                    Ok(mut client) => {
                        let connect_time = connect_start.elapsed();
                        total_query_time += connect_time;
                        successful_connections += 1;

                        // Simulate a simple query
                        match client.query_one("SELECT 1", &[]) {
                            Ok(_) => {
                                println!("User {} query succesful on attempt {}", user_id, attempt);
                                // Simulate some "work" time (10-50ms)
                                thread::sleep(Duration::from_millis(rng.random_range(10..50)));
                            }
                            Err(e) => {
                                eprintln!("User {} query failed on attempt {}: {}", user_id, attempt, e);
                            }
                        }
                    }
                    Err(e) => {
                        eprintln!("User {} connection failed on attempt {}: {}", user_id, attempt, e);
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

    println!("\nSimulation completed in {:.2?}", start_time.elapsed());
    println!("Total connection attempts: {}", total_attempts);
    println!("Successful connections: {} ({:.2}%)", total_successful, success_rate);
    println!("Average connection time: {:.2}ms", avg_query_time);
}