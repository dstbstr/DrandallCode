use std::env;
use std::fs;
use std::process;
use std::path::PathBuf;
use std::time::Instant;

use std::collections::HashMap;

//use std::thread;
//use std::thread::JoinHandle;
//use std::sync::{Arc, Mutex};

mod year_2015;
mod input_utils;

type Solutions = HashMap<i32, HashMap<i32, HashMap<i32, fn(&Vec<String>)->String>>>;

fn main() {
    let mut solutions: Solutions = Solutions::new();
    year_2015::add_solutions(&mut solutions);

    let args: Vec<String> = env::args().collect();
    let now = Instant::now();

    if args.len() > 1 {
        run_from_command_line(args, solutions);
    } else {
        //run_all();
        //run_year(2015);
        run_one(2015, 23, &solutions, true);
    }
    println!("");
    println!("Total Elapsed Time: {:?}", now.elapsed());
}

fn run_from_command_line(args: Vec<String>, solutions: Solutions) {
    match args.iter().map(|s| s.as_str()).collect::<Vec<&str>>() {
        _ if args[1] == "*" => run_all(solutions),
        _ if args.len() == 2 => run_year(args[1].parse().unwrap(), solutions),
        _ if args.len() == 3 => run_one(args[1].parse().unwrap(), args[2].parse().unwrap(), &solutions, false),
        _ => panic!("Unrecognized arguments")
    }
}

fn run_all(solutions: Solutions) {
    for (year, days) in &solutions {
        for (day, _) in days {
            run_one(*year, *day, &solutions, false);
        }
    }
    /*
    let shared = Arc::new(Mutex::new(solutions));
    let temp = Arc::clone(&shared);
    let temp2 = temp.lock().unwrap();
    let mut handles = vec![];

    let years = temp2.keys().map(|y| *y);
    for year in years {
        let days = temp2.get(&year).unwrap().keys().map(|d| *d);
        for day in days {
            let s = Arc::clone(&shared);
            handles.push(thread::spawn(move || {
                let runner = s.lock().unwrap();
                run_one(year, day, &runner, false);
            }));
        }
    }

    for handle in handles {
        handle.join().unwrap();
    }
    */
}

fn run_year(year: i32, solutions: Solutions) {
    /*
    let s = Arc::new(Mutex::new(solutions));
    let temp = Arc::clone(&s);
    let temp2 = temp.lock().unwrap();
    let days = temp2.get(&year).unwrap().keys().map(|d| *d);

    let mut handles = vec![];
    for day in days {
        let s = Arc::clone(&s);
        handles.push(thread::spawn(move || { 
            let runner = s.lock().unwrap();
            run_one(year, day, &runner, false);
        }));
    }

    for handle in handles {
        handle.join().unwrap();
    }
    */
    
    for(day, _) in solutions.get(&year).unwrap() {
        run_one(year, *day, &solutions, false);
    }
}

fn run_one(year: i32, day: i32, solutions: &Solutions, show_output: bool) {
    let file = get_file_name(year, day);
    let lines = read_input_file(&file);

    if show_output {
        println!("### {} {} ###", year, day);
    }

    for(part, func) in solutions.get(&year).unwrap().get(&day).unwrap() {
        if show_output {
            let now = Instant::now();
            println!("Part {part}: {}", func(&lines));
            println!("Elapsed Time: {:?}", now.elapsed());
        }
        else {
            func(&lines);
        }
    }
}

fn get_file_name(year: i32, day: i32) -> PathBuf {
    let mut buffer = [0; 2];
    let day_string = String::from("d") + &day.to_string() + &String::from(".txt");
    let file = [".", "rust_aoc", "inputs", &year.to_string(), &day_string].join(std::path::MAIN_SEPARATOR.encode_utf8(&mut buffer));
    let mut result = match env::current_dir() {
        Ok(r) => r,
        Err(..) => {
            panic!("Failed to find current_dir");
        }
    };

    result.push(PathBuf::from(file.strip_prefix(".").unwrap().strip_prefix(std::path::MAIN_SEPARATOR).unwrap()));
    result
}

fn read_input_file(file_name: &PathBuf) -> Vec<String> {
    let file_content: String = match fs::read_to_string(&file_name) {
        Ok(result) => result,
        Err(..) => {
            println!("Failed to find {:?}", &file_name);
            process::exit(1);
        }
    };

    file_content.lines().map(str::to_string).collect()
}
