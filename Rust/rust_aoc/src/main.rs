//use std::io;
//use std::cmp::Ordering;
use std::env;
use std::fs;
use std::process;
use std::path::{Path, PathBuf};

//use rust_core;
//use rust_core::geometry::Coord as Coord;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() > 1 {
        run_from_command_line(args);
        return;
    }

    //run_all();
    //run_year(2015);
    run_one(2015, 1);

    // let result = rust_core::add(2, 3);
    // println!("Add: {result}");

    // let coord = Coord::new(4, 2);
    // println!("Coord {}", coord);

    // let coord2 = Coord::from_str("5,3");
    // println!("Coord2 {}", coord2);
}

fn run_from_command_line(args: Vec<String>) {
    if args[1] == "*" {
        //println!("Run All");
        run_all();
    } else {
        let year: i32 = args[1].parse().expect("Failed to parse year");
        if args.len() == 2 {
            //println!("Run Year {}", year);
            run_year(year);
        }
        else if args[2] == "*" {
            //println!("Run Year {}", year);
            run_year(year);
        } else {
            let day: i32 = args[2].parse().expect("Failed to parse day");
            //println!("Run One {} {}", year, day);
            run_one(year, day);
        }
    }
}

fn run_all() {
    //for year in years
    //run_year(year);
}

fn run_year(year: i32) {
    //for day in days[year]
    //run_one(year, day);
    run_one(year, 1);
}

fn run_one(year: i32, day: i32) {
    println!("### {} {} ###", year, day);
    let mut buffer = [0; 2];
    let day_string = String::from("d") + &day.to_string() + &String::from(".txt");
    let file = [".", "rust_aoc", "inputs", &year.to_string(), &day_string].join(std::path::MAIN_SEPARATOR.encode_utf8(&mut buffer));
    let path = Path::new(&file);

    let file_content: String = match fs::read_to_string(&path) {
        Err(..) => {
            let mut absolute = env::current_dir().expect("");
            absolute.push(PathBuf::from(file.strip_prefix(".\\").expect("")));
            println!("Failed to find {:?}", absolute);
            process::exit(1);
        },
        Ok(content) => content
    };
    let lines = file_content.lines();

    //let mut lines: Vec<String>;
    //read_input_file(year, day, &mut lines);
    //let lines = read_input_file(year, day);
    for line in lines {
        dbg!(&line);
    }
}

/*
fn read_input_file<>(year: i32, day: i32, out_lines: &mut Vec<String>){
    let file = format!("/inputs/{}/d{}.txt", year, day);
    
    //let result = std::str::Lines::new();
    for line in fs::read_to_string(file).expect("Failed to locate input file {file}").lines() {
        //result.push(line);
        out_lines.push(String::from_str(line));
    }
    //return result;
    
    //out_lines = fs::read_to_string(file).expect("Failed to locate input file {file}").lines();
}
*/
//fn read_input_file(year: i32, day: i32) -> String::Lines<String> {
//    let file = format!("/inputs/{}/d{}.txt", year, day);
//    fs::read_to_string(file).expect("Failed to locate input file {file}").lines().clone()
//}