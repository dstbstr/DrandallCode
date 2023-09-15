use std::str::FromStr;
use std::fmt::Debug;
use regex::Regex;

pub fn split_input_into_groups(lines: &Vec<String>) -> Vec<Vec<String>> {
    let mut result: Vec<Vec<String>> = Vec::new();
    let mut group: Vec<String> = Vec::new();

    for line in lines {
        if line.is_empty() {
            result.push(group.clone());
            group.clear();
        } else {
            group.push(line.clone());
        }
    }

    result.push(group.clone());
    result
}

pub fn parse_lines_as_numbers<T>(lines: &Vec<String>) -> Vec<T> 
where T: FromStr, <T as FromStr>::Err: Debug {
    lines.iter().map(|s| s.parse::<T>().expect("Failed to parse number")).collect::<Vec<T>>()
}

pub fn get_numbers_from_string<T>(line: &str) -> Vec<T> 
where T: FromStr, <T as FromStr>::Err: Debug {
    let re = Regex::new(r#"(-?\d+)"#).unwrap();

    re.captures_iter(line).map(|c| {
        let (_, [val]) = c.extract();
        val.parse::<T>().unwrap()
    }).collect::<Vec<T>>()
}