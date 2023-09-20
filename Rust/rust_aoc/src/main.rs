use std::env;
use std::fs;
use std::path::PathBuf;
use std::process;
use std::time::Instant;

use std::collections::HashMap;

use std::thread;

mod input_utils;
mod year_2015;
mod year_2016;

type Solutions = HashMap<i32, HashMap<i32, HashMap<i32, fn(&Vec<String>) -> String>>>;

fn main() {
    let mut solutions: Solutions = Solutions::new();
    year_2015::add_solutions(&mut solutions);
    year_2016::add_solutions(&mut solutions);

    let args: Vec<String> = env::args().collect();
    let now = Instant::now();

    if args.len() > 1 {
        run_from_command_line(args, solutions);
    } else {
        //run_all();
        //run_year(2015);
        run_one(2016, 3, &solutions);
    }
    println!("");
    println!("Total Elapsed Time: {:?}", now.elapsed());
}

fn run_from_command_line(args: Vec<String>, solutions: Solutions) {
    match args.iter().map(|s| s.as_str()).collect::<Vec<&str>>() {
        _ if args[1] == "*" => run_all(solutions),
        _ if args.len() == 2 => run_year(args[1].parse().unwrap(), solutions),
        _ if args.len() == 3 => run_one(
            args[1].parse().unwrap(),
            args[2].parse().unwrap(),
            &solutions
        ),
        _ => panic!("Unrecognized arguments"),
    }
}

fn run_all(solutions: Solutions) {
    let mut pairs = vec![];
    for (year, days) in solutions {
        for (day, parts) in days {
            for (_part, to_call) in parts {
                pairs.push((read_input_file(get_file_name(year, day)), to_call));
            }
        }
    }
    let mut handles = vec![];
    for (input, to_call) in pairs {
        handles.push(thread::spawn(move || {
            to_call(&input);
        }))
    }
    for handle in handles {
        handle.join().unwrap();
    }
}

fn run_year(year: i32, solutions: Solutions) {
    let mut pairs = vec![];
    for (y, days) in solutions {
        if y != year {
            continue;
        }
        for (day, parts) in days {
            for (_part, to_call) in parts {
                pairs.push((read_input_file(get_file_name(year, day)), to_call));
            }
        }
    }

    let mut handles = vec![];
    for (input, to_call) in pairs {
        handles.push(thread::spawn(move || {
            to_call(&input);
        }))
    }
    for handle in handles {
        handle.join().unwrap();
    }
}

fn run_one(year: i32, day: i32, solutions: &Solutions) {
    let file = get_file_name(year, day);
    let lines = read_input_file(file);

    println!("### {} {} ###", year, day);

    for (part, func) in solutions.get(&year).unwrap().get(&day).unwrap() {
            let now = Instant::now();
            println!("Part {part}: {}", func(&lines));
            println!("Elapsed Time: {:?}", now.elapsed());
    }
}

fn get_file_name(year: i32, day: i32) -> PathBuf {
    let mut buffer = [0; 2];
    let day_string = String::from("d") + &day.to_string() + &String::from(".txt");
    let file = [".", "rust_aoc", "inputs", &year.to_string(), &day_string]
        .join(std::path::MAIN_SEPARATOR.encode_utf8(&mut buffer));
    let mut result = match env::current_dir() {
        Ok(r) => r,
        Err(..) => {
            panic!("Failed to find current_dir");
        }
    };

    result.push(PathBuf::from(
        file.strip_prefix(".")
            .unwrap()
            .strip_prefix(std::path::MAIN_SEPARATOR)
            .unwrap(),
    ));
    result
}

fn read_input_file(file_name: PathBuf) -> Vec<String> {
    let file_content: String = match fs::read_to_string(&file_name) {
        Ok(result) => result,
        Err(..) => {
            println!("Failed to find {:?}", &file_name);
            process::exit(1);
        }
    };

    file_content.lines().map(str::to_string).collect()
}
