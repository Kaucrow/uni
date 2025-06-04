use crypto_math_fundamentals::{
    prelude::*,
    settings::Settings,
};
use anyhow::Result;
use num_bigint::BigInt;

fn main() -> Result<()> {
    let settings = Settings::load()?;
    let ci = settings.ci;

    // Get the last six digits from the C.I.
    let last_six = if ci.len() >= 6 {
        &ci[ci.len()-6..]
    } else {
        &ci // If string is shorter than 6 chars, return whole string
    };

    let mut a: BigInt = last_six[0..2].parse()?;
    let mut m: BigInt = last_six[2..4].parse()?;
    let mut n: BigInt = last_six[4..6].parse()?;

    if a < BigInt::from(3) { a += 13 }
    if m < BigInt::from(3) { m += 13 }
    if n < BigInt::from(3) { n += 13 }

    println!("{}", "——— Variables ———".cyan());
    println!("{} {}", "a:".yellow(), a.to_string());
    println!("{} {}", "m:".yellow(), m.to_string());
    println!("{} {}", "n:".yellow(), n.to_string());

    // 1. Calcula los restos de:
    println!("{}", "\n1. Calcula los restos de: ".cyan());

    // a) (a+1)^(m+1) mod n
    println!("{} {}", "(a+1)^(m+1) mod n:".yellow(), (&a+1u32).pow((&m+1u32).to_u32_digits().1[0]) % &n);
    // b) (n+10)^(a+10) mod m
    println!("{} {}", "(n+10)^(a+10) mod m:".yellow(), (&n+10u32).pow((&a+10u32).to_u32_digits().1[0]) % &m);
    // c) (m+10)^(a+5) mod n
    println!("{} {}", "(m+10)^(a+5) mod n:".yellow(), (&m+10u32).pow((&a+5u32).to_u32_digits().1[0]) % &n);
    // d) (n+3)^(m+2) mod a
    println!("{} {}", "(n+3)^(m+2) mod a:".yellow(), (&n+3u32).pow((&m+2u32).to_u32_digits().1[0]) % &a);

    // 2. Determina si (2n + 3m) ≡ 3(mod a). Justifica tu respuesta.
    println!("\n{}", "2. Determina si (2n + 3m) ≡ 3(mod a). Justifica tu respuesta:".cyan());

    let left_side = (BigInt::from(2u32) * &n + BigInt::from(3u32) * &m) % &a;
    let right_side = BigInt::from(3u32) % &a;

    println!("{} {}", "(2n + 3m) =".yellow(), 2u32 * &n + 3u32 * &m);
    println!("{} {}", "(2n + 3m) mod a =".yellow(), left_side); 
    println!("{} {}", "3 mod a =".yellow(), right_side);

    // Determina si es congruente
    let is_congruent = left_side == right_side;
    println!("\n{}", "Resultado:".green());
    print!("(2n + 3m) ≡ 3 (mod a) es ");
    if is_congruent { println!("{}", "verdadero".green()) } else { println!("{}", "falso".red()) }

    // Justificacion
    println!("{}", "\nJustificacion:".green());
    if is_congruent {
        println!("Ya que {} ≡ {} (mod {}), existe congruencia.", 
            left_side, right_side, a);
    } else {
        println!("Ya que {} ≠ {} (mod {}), no existe congruencia.", 
            left_side, right_side, a);
    }

    // 3. Aplica congruencias para responder los siguientes planteamientos:
    println!("\n{}", "3. Aplica congruencias para responder los siguientes planteamientos:".cyan());

    // a) Supongamos que estamos en noviembre, ¿qué mes será dentro de 1000 meses?
    print!("{}", "a) Supongamos que estamos en noviembre, ¿qué mes será dentro de 1000 meses?: ".yellow());

    let months = HashMap::from([
        (0, "Enero"),
        (1, "Febrero"),
        (2, "Marzo"),
        (3, "Abril"),
        (4, "Mayo"),
        (5, "Junio"),
        (6, "Julio"),
        (7, "Agosto"),
        (8, "Septiembre"),
        (9, "Octubre"),
        (10, "Noviembre"),
        (11, "Diciembre"),
    ]);

    let curr_month = 10;
    let amt_months = 1000;

    let residue = amt_months % 12;
    
    // Calculate the month index (0-11)
    let month_index = (curr_month + residue) % 12;
    
    // Get the month name
    let month_name = months.get(&month_index)
        .ok_or(anyhow!("Invalid month index: {}", month_index))?;

    println!("{}", month_name);

    // b) Si hoy es domingo,
    println!("{}", "b) Si hoy es domingo,".yellow());

    let days = HashMap::from([
        (0, "Lunes"),
        (1, "Martes"),
        (2, "Miércoles"),
        (3, "Jueves"),
        (4, "Viernes"),
        (5, "Sábado"),
        (6, "Domingo"),
    ]);

    let curr_day = 6;

    //¿qué día será en 15 días?
    print!("{}", "  ¿qué día será en 15 días?: ".green());
    let day_name = get_map_name(curr_day, 15, 7, &days)?;
    println!("{}", day_name);

    // Y en 26 días?
    print!("{}", "  ¿y en 26 días?: ".green());
    let day_name = get_map_name(curr_day, 26, 7, &days)?;
    println!("{}", day_name);
    
    // Y en 234 días?
    print!("{}", "  ¿y en 234 días?: ".green());
    let day_name = get_map_name(curr_day, 234, 7, &days)?;
    println!("{}", day_name);
    
    // Y en 1000 días?
    print!("{}", "  ¿y en 1000 días?: ".green());
    let day_name = get_map_name(curr_day, 1000, 7, &days)?;
    println!("{}", day_name);

    // c) Si en este momento son las 9am, ¿qué hora será dentro de 50 horas?
    print!("{}", "c) Si en este momento son las 9am, ¿qué hora será dentro de 50 horas?: ".yellow());
    let curr_hour = 9;
    let amt_hours = 50;
    let residue = amt_hours % 24;
    let res_hour = {
        let hour = (curr_hour + residue) % 24;
        let (display_hour, period) = match hour {
            0 => (12, "am"),  // Midnight (12am)
            12 => (12, "pm"), // Noon (12pm)
            h if h < 12 => (h, "am"),
            h => (h - 12, "pm"),
        };
        format!("{}{}", display_hour, period)
    };

    println!("{}", res_hour);

    // 4. Verifica el cumplimiento de las Leyes de Morgan para los siguientes 3 conjuntos:
    // A=Z7​, B=Z12​ y C=Z5​, definidos dentro del conjunto universal Z15.
    println!("\n{}", 
"4. Verifica el cumplimiento de las Leyes de Morgan para los siguientes 3 conjuntos:
A=Z7, B=Z12 y C=Z5, definidos dentro del conjunto universal Z15.".cyan()
    );

    let a_set: BTreeSet<_> = (0..7).collect();
    let b_set: BTreeSet<_> = (0..12).collect();
    let c_set: BTreeSet<_> = (0..5).collect();
    let uni_set: BTreeSet<_> = (0..15).collect();

    verify_de_morgan(&a_set, &b_set, &uni_set, "A", "B");
    verify_de_morgan(&b_set, &c_set, &uni_set, "B", "C");
    verify_de_morgan(&c_set, &a_set, &uni_set, "C", "A");

    // 5. Calcular A ∆ B
    // donde A = Z15 \ Z7 y B = Z12 \ Z5
    print!("\n{}", "5. Calcular A ∆ B donde A = Z15\\Z7 y B = Z12\\Z5: ".cyan());
    let a_set: BTreeSet<_> = (7..15).collect();
    let b_set: BTreeSet<_> = (5..12).collect();
    let a_symdiff_b: BTreeSet<_> = a_set.symmetric_difference(&b_set).collect();
    println!("{:?}", a_symdiff_b);

    Ok(())
}

fn get_map_name(
    curr_idx: i32,
    amt: i32,
    modulo: i32,
    map: &HashMap<i32,
    &'static str>
) -> Result<&'static str> {
    let residue = amt % modulo;
    let map_idx = (curr_idx + residue) % modulo;

    let str = map.get(&map_idx)
        .ok_or(anyhow!("Invalid index: {}", map_idx))?;

    Ok(str)
}

fn verify_de_morgan(
    x: &BTreeSet<i32>,
    y: &BTreeSet<i32>,
    universal: &BTreeSet<i32>,
    x_name: &str,
    y_name: &str,
) {
    // Complements
    let comp_x: BTreeSet<_> = universal.difference(x).cloned().collect();
    let comp_y: BTreeSet<_> = universal.difference(y).cloned().collect();

    // First Law: ¬(X ∪ Y) = ¬X ∩ ¬Y
    let union_xy: BTreeSet<_> = x.union(y).cloned().collect();
    let left1: BTreeSet<_> = universal.difference(&union_xy).cloned().collect();
    let right1: BTreeSet<_> = comp_x.intersection(&comp_y).cloned().collect();
    let law1_holds = left1 == right1;

    // Second Law: ¬(X ∩ Y) = ¬X ∪ ¬Y
    let inter_xy: BTreeSet<_> = x.intersection(y).cloned().collect();
    let left2: BTreeSet<_> = universal.difference(&inter_xy).cloned().collect();
    let right2: BTreeSet<_> = comp_x.union(&comp_y).cloned().collect();
    let law2_holds = left2 == right2;

    println!("{}", format!("\nLeyes De Morgan para {} y {}:", x_name, y_name).green());
    println!("{} {}",
        format!("1. ¬({} ∪ {}) = ¬{} ∩ ¬{}:", x_name, y_name, x_name, y_name).yellow(),
        law1_holds
    );
    println!("{} {}",
        format!("2. ¬({} ∩ {}) = ¬{} ∪ ¬{}:", 
        x_name, y_name, x_name, y_name).yellow(),
        law2_holds
    );
}