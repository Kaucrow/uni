use crate::{
    prelude::*,
    settings::Settings,
};
use anyhow::Result;
use postgres::{ Client, NoTls };
use fake::{ Fake, faker::lorem };

pub fn fill_db(conn_string: &String, settings: &Settings) -> Result<()> {
    let mut client = Client::connect(
        &conn_string,
        NoTls
    )?;

    // Check if table exists
    let table_exists: bool = client
        .query_one(
            "SELECT EXISTS (
                SELECT FROM pg_tables
                WHERE schemaname = 'public'
                AND tablename = $1
            )",
            &[&settings.database.tablename],
        )?
        .get(0);

    if table_exists {
        println!("{}{}{}", "Table '".yellow(), settings.database.tablename.yellow(), "' already exists - skipping creation.".yellow());
        return Ok(());
    }

    // Create table if not exists
    client
        .execute(
            &format!(
                "CREATE TABLE {} (
                    id INT PRIMARY KEY,
                    descript VARCHAR(1000) NOT NULL
                )",
                settings.database.tablename
            ),
            &[],
        )?;
    
    // Prepare the insert statement
    let stmt = client
        .prepare("INSERT INTO mockdata (id, descript) VALUES ($1, $2)")?;

    // Insert 5000 rows
    for i in 1..=5000 {
        // Generate lorem ipsum text
        let text = lorem::en::Words(1..100).fake::<Vec<String>>().join(" ");

        client.execute(&stmt, &[&(i as i32), &text])?;
    }

    Ok(())
}