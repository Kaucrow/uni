use std::io::{ self, Write };
use std::time::Instant;

#[derive(Clone)]
struct Task {
    name: String,
    start_time: usize,
    duration: usize,
}

fn input_data() -> (Vec<Task>, usize) {
    let mut tasks = Vec::new();
    let mut buffer = String::new();

    println!("Enter the number of tasks:");
    io::stdin().read_line(&mut buffer).unwrap();
    let n: usize = buffer.trim().parse().unwrap();

    for i in 0..n {
        buffer.clear();
        print!("Enter the name of task {}: ", i + 1);
        io::stdout().flush().unwrap();
        io::stdin().read_line(&mut buffer).unwrap();
        let name = buffer.trim().to_string();

        buffer.clear();
        print!("Enter the start time of task {}: ", i + 1);
        io::stdout().flush().unwrap();
        io::stdin().read_line(&mut buffer).unwrap();
        let start_time: usize = buffer.trim().parse().unwrap();

        buffer.clear();
        print!("Enter the duration of task {}: ", i + 1);
        io::stdout().flush().unwrap();
        io::stdin().read_line(&mut buffer).unwrap();
        let duration: usize = buffer.trim().parse().unwrap();

        tasks.push(Task {
            name,
            start_time,
            duration,
        });
    }

    buffer.clear();
    println!("Enter the quantum for Round Robin:");
    io::stdin().read_line(&mut buffer).unwrap();
    let quantum: usize = buffer.trim().parse().unwrap();

    (tasks, quantum)
}

fn fifo(tasks: &[Task]) -> (usize, f64) {
    // Set the clock to the task with the min start time
    let mut clock = tasks.iter().map(|task| task.start_time).min().unwrap();
    let mut total_turnaround = 0.0;
    let mut total_waiting = 0.0;
    let mut total_efficiency = 0.0;

    // Keep track of processed tasks
    let mut processed = vec![false; tasks.len()];

    for _ in 0..tasks.len() {
        // Find the first unprocessed task that can start at or before the current clock
        if let Some((idx, task)) = tasks.iter().enumerate()
            .find(|(i, task)| !processed[*i] && task.start_time <= clock) // Pick the first task with a start time lower or equal than the clock time
        {
            let start_time = task.start_time;
            let duration = task.duration;

            let finish_time = clock + duration;
            let turnaround = finish_time - start_time;
            let waiting = turnaround - duration;
            let efficiency = duration as f64 / turnaround as f64;

            // Accumulate metrics
            total_turnaround += turnaround as f64;
            total_waiting += waiting as f64;
            total_efficiency += efficiency;

            // Print task details
            println!(
                "Task {}: Start: {}, Finish: {}, Turnaround (T): {}, Waiting (E): {}, Efficiency (I): {:.2}",
                task.name, start_time, finish_time, turnaround, waiting, efficiency
            );

            // Mark this task as processed
            processed[idx] = true;

            // Advance the clock
            clock = finish_time;
        } else {
            // If no task is ready, advance the clock to the next unprocessed task's start time
            if let Some(next_task_time) = tasks.iter()
                .enumerate()
                .filter(|(i, _)| !processed[*i]) // Ignore processed tasks
                .map(|(_, task)| task.start_time)
                .min() // Find the next start time
            {
                clock = next_task_time;
            }
        }
    }

    let avg_turnaround = total_turnaround / tasks.len() as f64;
    let avg_efficiency = total_efficiency / tasks.len() as f64;

    println!(
        "\nAverages - Turnaround (T): {:.2}, Waiting (E): {:.2}, Efficiency (I): {:.2}",
        avg_turnaround, total_waiting / tasks.len() as f64, avg_efficiency
    );

    (clock, avg_efficiency)
}

fn lifo(mut tasks: Vec<Task>) -> (usize, f64) {
    tasks.reverse();
    fifo(&tasks)
}

fn round_robin(tasks: &[Task], quantum: usize) -> (usize, f64) {
    let mut remaining_tasks: Vec<&Task> = tasks.iter().map(|task| task).collect();
    let mut remaining_times: Vec<usize> = tasks.iter().map(|task| task.duration).collect();
    let mut finish_times = vec![0; tasks.len()];
    let mut clock = tasks.iter().map(|task| task.start_time).min().unwrap();

    let mut total_turnaround = 0.0;
    let mut total_waiting = 0.0;
    let mut total_efficiency = 0.0;

    while !remaining_tasks.is_empty() {
        // Check if any task is ready to execute
        if !remaining_tasks.iter().any(|&vec_task| vec_task.start_time <= clock) {
            // Find the next task's start_time and update the clock
            if let Some(next_start_time) = remaining_tasks.iter().map(|&vec_task| vec_task.start_time).min() {
                clock = next_start_time;
            }
        }

        for index in 0..tasks.len() {
            let task = &tasks[index];

            // Check if the task is ready to execute (start_time <= current clock)
            // and if it has not been finished yet (finish_times[index] == 0)
            if task.start_time <= clock && finish_times[index] == 0 {
                // Determine how much time to execute this task in this round:
                // Either the remaining time or the quantum (whichever is smaller)
                let execute_time = remaining_times[index].min(quantum);

                // Advance the clock by the execution time
                clock += execute_time;

                // Decrease the task's remaining time by the executed amount
                remaining_times[index] -= execute_time;

                // If the task is now finished (remaining time is 0)
                if remaining_times[index] == 0 {
                    // Record the finish time of the task
                    finish_times[index] = clock;

                    // Remove the finished task from the `remaining_tasks` Vec
                    if let Some(pos) = remaining_tasks.iter().position(|&vec_task| std::ptr::eq(vec_task, task)) {
                        remaining_tasks.remove(pos);
                    }
                }
            }
        }
    }

    // Calculate metrics for each task
    for (i, task) in tasks.iter().enumerate() {
        let turnaround = finish_times[i] - task.start_time;
        let waiting = turnaround - task.duration;
        let efficiency = task.duration as f64 / turnaround as f64;

        total_turnaround += turnaround as f64;
        total_waiting += waiting as f64;
        total_efficiency += efficiency;

        println!(
            "Task {}: Start: {}, Finish: {}, Turnaround (T): {}, Waiting (E): {}, Efficiency (I): {:.2}",
            task.name, task.start_time, finish_times[i], turnaround, waiting, efficiency
        );
    }

    // Print averages
    let avg_turnaround = total_turnaround / tasks.len() as f64;
    let avg_waiting = total_waiting / tasks.len() as f64;
    let avg_efficiency = total_efficiency / tasks.len() as f64;

    println!(
        "\nAverages - Turnaround (T): {:.2}, Waiting (E): {:.2}, Efficiency (I): {:.2}",
        avg_turnaround, avg_waiting, avg_efficiency
    );

    (clock, avg_efficiency)
}

fn main() {
    let (tasks, quantum) = input_data();

    // Measure execution time for FIFO
    println!("\n—————————————————————————— Executing FIFO ——————————————————————————\n");

    let start_fifo = Instant::now();
    let (clock_fifo, efficiency_fifo) = fifo(&tasks);
    let time_fifo = start_fifo.elapsed();

    println!("\nFIFO completed in {:.5} seconds.", time_fifo.as_secs_f64());

    // Measure execution time for LIFO
    println!("\n—————————————————————————— Executing LIFO ——————————————————————————\n");

    let start_lifo = Instant::now();
    let (clock_lifo, efficiency_lifo) = lifo(tasks.clone());
    let time_lifo = start_lifo.elapsed();

    println!("\nLIFO completed in {:.5} seconds.", time_lifo.as_secs_f64());

    // Measure execution time for Round Robin
    println!("\n——————————————————————— Executing Round Robin ——————————————————————\n");

    let start_rr = Instant::now();
    let (clock_rr, efficiency_rr) = round_robin(&tasks, quantum);
    let time_rr = start_rr.elapsed();

    println!("\nRound Robin completed in {:.5} seconds.", time_rr.as_secs_f64());

    // Final Comparison
    println!("\nResults:");
    println!("FIFO Clock: {}, LIFO Clock: {}, RR Clock: {}", clock_fifo, clock_lifo, clock_rr);
    if clock_fifo == clock_lifo && clock_fifo == clock_rr {
        println!("All algorithms have consistent clock times.");
    } else {
        println!("Inconsistent clock times. Check the implementations.");
    }

    println!("Best Efficiency:");
    if efficiency_fifo > efficiency_lifo && efficiency_fifo > efficiency_rr {
        println!("FIFO was the most efficient: {:.2}", efficiency_fifo);
    } else if efficiency_lifo > efficiency_fifo && efficiency_lifo > efficiency_rr {
        println!("LIFO was the most efficient: {:.2}", efficiency_lifo);
    } else {
        println!("Round Robin was the most efficient: {:.2}", efficiency_rr);
    }

    println!();
}