use std::env;
use std::fs;
use std::process;
use std::path::PathBuf;
use std::error::Error;
use std::time::Instant;

use std::collections::HashMap;

mod year_2015;
mod input_utils;

type Solutions = HashMap<i32, HashMap<i32, HashMap<i32, fn(&Vec<String>)->String>>>;

fn main() {
    //let solutions = HashMap<i32, HashMap<i32, HashMap<i32, fn(&Vec<String>)->String>>>::new();
    let mut solutions: Solutions = HashMap::new();
    year_2015::add_solutions(&mut solutions);

    let args: Vec<String> = env::args().collect();
    if args.len() > 1 {
        if let Err(e) = run_from_command_line(args, &solutions) {
            println!("Error: {e}");
            process::exit(1);
        }
        return;
    }

    //run_all();
    //run_year(2015);
    if let Err(e) = run_one(2015, 7, &solutions) {
        println!("Error: {e}");
        process::exit(1);
    }
}

fn run_from_command_line(args: Vec<String>, solutions: &Solutions) -> Result<(), Box<dyn Error>> {
    if args[1] == "*" {
        run_all(solutions)
    } else {
        let year: i32 = args[1].parse().expect("Failed to parse year");
        if args.len() == 2 {
            run_year(year, solutions)
        }
        else if args[2] == "*" {
            run_year(year, solutions)
        } else {
            let day: i32 = args[2].parse().expect("Failed to parse day");
            run_one(year, day, solutions)
        }
    }
}

fn run_all(solutions: &Solutions) -> Result<(), Box<dyn Error>> {
    for (year, _) in solutions {
        run_year(*year, solutions)?;
    }
    //for year in years
    //run_year(year);
    Ok(())
}

fn run_year(year: i32, solutions: &Solutions) -> Result<(), Box<dyn Error>> {
    for(day, _) in solutions.get(&year).unwrap() {
        run_one(year, *day, solutions)?;
    }
    Ok(())
}

fn run_one(year: i32, day: i32, solutions: &Solutions) -> Result<(), Box<dyn Error>> {
    let file = get_file_name(year, day)?;
    let lines = read_input_file(&file)?;

    println!("### {} {} ###", year, day);

    for(part, func) in solutions.get(&year).unwrap().get(&day).unwrap() {
        let now = Instant::now();
        println!("Part {part}: {}", func(&lines));
        println!("Elapsed Time: {:?}", now.elapsed());
    }
    Ok(())
}

fn get_file_name(year: i32, day: i32) -> Result<PathBuf, Box<dyn Error>> {
    let mut buffer = [0; 2];
    let day_string = String::from("d") + &day.to_string() + &String::from(".txt");
    let file = [".", "rust_aoc", "inputs", &year.to_string(), &day_string].join(std::path::MAIN_SEPARATOR.encode_utf8(&mut buffer));
    let mut result = env::current_dir()?;

    result.push(PathBuf::from(file.strip_prefix(".").unwrap().strip_prefix(std::path::MAIN_SEPARATOR).unwrap()));
    Ok(result)
}

fn read_input_file(file_name: &PathBuf) -> Result<Vec<String>, Box<dyn Error>> {
    let file_content: String = match fs::read_to_string(&file_name) {
        Ok(result) => result,
        Err(..) => {
            println!("Failed to find {:?}", &file_name);
            process::exit(1);
        }
    };

    return Ok(file_content.lines().map(str::to_string).collect());
}
