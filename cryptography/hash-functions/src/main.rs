use hash_functions::{
    prelude::*,
    settings::Settings,
};
use anyhow::Result;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    // Configure the parameters
    let settings = Settings::load()?;
    let max_leading_zeros = settings.max_leading_zeros;
    let base_message = settings.base_message;

    // Hash functions to test
    let hash_functions: Vec<(&str, Box<dyn Fn(&[u8]) -> String>)> = vec![
        ("MD5", Box::new(|data| {
            let digest = md5::compute(data);
            format!("{:x}", digest)
        })),
        ("SHA-1", Box::new(|data| {
            let mut hasher = Sha1::new();
            hasher.update(data);
            format!("{:x}", hasher.finalize())
        })),
        ("SHA-256", Box::new(|data| {
            let mut hasher = Sha256::new();
            hasher.update(data);
            format!("{:x}", hasher.finalize())
        })),
    ];

    let mut all_results = Vec::new();

    for (hash_name, hash_fn) in hash_functions {
        println!("{}", format!("\nEjecutando pruebas para {}...", hash_name).magenta());
        let mut hash_results = Vec::new();

        for target_zeros in 1..=max_leading_zeros {
            let target_prefix = "0".repeat(target_zeros);
            println!("{} {} ceros", "Objetivo:".cyan(), target_zeros);

            let mut counter = 0; // Reset counter for each zero count
            let mut attempts = 0; // Reset attempts for each zero count
            let start_time = Instant::now();
            let mut time_points = Vec::new();
            let checkpoint_interval = 50_000;

            loop {
                counter += 1;
                attempts += 1;
                let message = format!("{}:{}", base_message, counter);
                
                let hash = hash_fn(message.as_bytes());
                
                if hash.starts_with(&target_prefix) {
                    time_points.push((attempts, start_time.elapsed()));
                    println!(
                        "{} en {} {} con {} '{}' ({:.2} {}): {} {}",
                        "Hallado".green(),
                        attempts,
                        "intentos".yellow(),
                        "preimagen".yellow(),
                        message,
                        start_time.elapsed().as_secs_f64(),
                        "segundos".yellow(),
                        "Hash".yellow(),
                        hash
                    );
                    hash_results.push((target_zeros, attempts, time_points.clone()));
                    break;
                }

                if attempts % checkpoint_interval == 0 {
                    time_points.push((attempts, start_time.elapsed()));
                    print!(".");
                    std::io::stdout().flush().unwrap();  // Explicitly flush stdout
                }
            }
        }

        generate_graph(&hash_name, &hash_results)?;
        all_results.push((hash_name, hash_results));
    }
    
    generate_comparison_graph(&all_results)?;
    
    Ok(())
}

fn generate_graph(
    hash_name: &str,
    results: &[(usize, u64, Vec<(u64, std::time::Duration)>)]
) -> Result<(), Box<dyn std::error::Error>> {
    let backend_str = format!("{}_rendimiento.png", hash_name.to_lowercase());
    let backend = BitMapBackend::new(&backend_str, (1024, 768));
    let root = backend.into_drawing_area();
    root.fill(&WHITE)?;

    // Calculate maximum time dynamically
    let max_time = results.iter()
        .flat_map(|(_, _, time_points)| time_points.iter().map(|(_, t)| t.as_secs_f64()))
        .max_by(|a, b| a.partial_cmp(b).unwrap_or(std::cmp::Ordering::Equal))
        .unwrap_or(10.0) * 1.1;

    let max_attempts = results.iter()
        .map(|(_, attempts, _)| *attempts as f64)
        .max_by(|a, b| a.partial_cmp(b).unwrap_or(std::cmp::Ordering::Equal))
        .unwrap_or(1000.0) * 1.1;

    let mut chart = ChartBuilder::on(&root)
        .caption(
            format!("Rendimiento de Preimagen {}", hash_name), 
            ("sans-serif", 30).into_font())
        .margin(10)
        .x_label_area_size(40)
        .y_label_area_size(40)
        .build_cartesian_2d(0f64..max_attempts, 0f64..max_time)?;

    chart.configure_mesh()
        .x_desc("Intentos")
        .y_desc("Tiempo (segundos)")
        .draw()?;

    let colors = [&RED, &GREEN, &BLUE, &MAGENTA, &CYAN];

    // Plot each trial
    for (_, (zeros, total_attempts, time_points)) in results.iter().enumerate() {
        let color = colors[*zeros % colors.len()].clone();
        let label = format!("{} ceros", zeros);

        // Convert time points and prepend (0,0)
        let mut data: Vec<(f64, f64)> = time_points.iter()
            .map(|(a, t)| (*a as f64, t.as_secs_f64()))
            .collect();
        
        // Insert (0,0) at start if not already present
        if data.is_empty() || data[0] != (0.0, 0.0) {
            data.insert(0, (0.0, 0.0));
        }

        // Draw the line series (this will connect to origin)
        chart.draw_series(LineSeries::new(
            data.iter().map(|(x, y)| (*x, *y)),
            color.stroke_width(2),
        ))?.label(label)
           .legend(move |(x, y)| PathElement::new(vec![(x, y), (x + 20, y)], color.stroke_width(2)));

        // Mark only the final point (not origin)
        if let Some(&(x, y)) = data.last() {
            chart.draw_series(
                std::iter::once(Circle::new((x, y), 5, color.filled()))
            )?;
        }
    }

    // Configure and draw the legend
    chart.configure_series_labels()
        .background_style(&WHITE.mix(0.8))
        .border_style(&BLACK)
        .draw()?;
    
    Ok(())
}

fn generate_comparison_graph(
    all_results: &[(&str, Vec<(usize, u64, Vec<(u64, std::time::Duration)>)>)]
) -> Result<(), Box<dyn std::error::Error>> {
    let root = BitMapBackend::new("comparacion_hashes.png", (1024, 768)).into_drawing_area();
    root.fill(&WHITE)?;
    
    let max_attempts = *all_results.iter()
        .flat_map(|(_, res)| res.iter().map(|(_, a, _)| a))
        .max()
        .unwrap_or(&1000) as f64 * 1.1;
    
    let max_time = all_results.iter()
        .flat_map(|(_, res)| res.iter().flat_map(|(_, _, tp)| tp.iter().map(|(_, t)| t.as_secs_f64())))
        .max_by(|a, b| a.partial_cmp(b).unwrap_or(std::cmp::Ordering::Equal))
        .unwrap_or(10.0) * 1.1;
    
    let mut chart = ChartBuilder::on(&root)
        .caption(
            "Comparacion de Funciones Hash",
            ("sans-serif", 30).into_font())
        .margin(10)
        .x_label_area_size(40)
        .y_label_area_size(40)
        .build_cartesian_2d(0f64..max_attempts, 0f64..max_time)?;

    chart.configure_mesh()
        .x_desc("Intentos")
        .y_desc("Tiempo (segundos)")
        .draw()?;

    let colors = [&RED, &GREEN, &BLUE];

    for (i, (hash_name, results)) in all_results.iter().enumerate() {
        let color = colors[i % colors.len()].clone();

        let data: Vec<(f64, f64, usize)> = results.iter()
            .map(|(z, a, tp)| (*a as f64, tp.last().unwrap().1.as_secs_f64(), *z))
            .collect();

        chart.draw_series(LineSeries::new(
            data.iter().map(|(x, y, _)| (*x, *y)),
            color.stroke_width(3),
        ))?.label(*hash_name)
           .legend(move |(x, y)| PathElement::new(vec![(x, y), (x + 20, y)], color.stroke_width(3)));

        chart.draw_series(
            data.iter().map(|(x, y, _)| Circle::new((*x, *y), 5, color.filled()))
        )?;

        chart.draw_series(
            data.iter().map(|(x, y, z)| {
                Text::new(
                    format!("{}", z),
                    (*x, *y + 0.1),
                    ("sans-serif", 12).into_font()
                )
            })
        )?;
    }

    chart.configure_series_labels()
        .background_style(&WHITE.mix(0.8))
        .border_style(&BLACK)
        .draw()?;

    Ok(())
}