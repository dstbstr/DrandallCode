use itertools::Itertools;
use std::collections::{HashMap, HashSet};

pub fn part_one(lines: &Vec<String>) -> String {
    solve(lines, true ).to_string()
}

pub fn part_two(lines: &Vec<String>) -> String {
    solve(lines, false).to_string()
}

fn solve(lines: &Vec<String>, use_min: bool) -> u32 {
    let mut dist: HashMap<&str, HashMap<&str, u32>> = HashMap::new();
    let mut destinations: HashSet<&str> = HashSet::new();

    lines.iter().map(|l| {
        let s: Vec<&str> = l.split(" ").collect();
        (s[0], s[2], s[4].parse().unwrap())
    }).for_each(|e| {
        if !dist.contains_key(e.0) {
            dist.insert(&e.0, HashMap::new());
        }
        if !dist.contains_key(e.1) {
            dist.insert(&e.1, HashMap::new());
        }
        dist.get_mut(&e.0).unwrap().insert(&e.1, e.2);
        dist.get_mut(&e.1).unwrap().insert(&e.0, e.2);
        destinations.insert(&e.0);
        destinations.insert(&e.1);
    });

    let results = destinations.iter().permutations(destinations.len())
        .map(|p| get_total_distance(&p, &dist));

    if use_min {
        results.min().unwrap()
    } else {
        results.max().unwrap()
    }

}
fn get_total_distance(destinations: &Vec<&&str>, distances: &HashMap<&str, HashMap<&str, u32>>) -> u32 {
    let mut result = 0u32;

    for i in 0..destinations.len() - 1 {
        result += distances.get(destinations[i]).unwrap().get(destinations[i + 1]).unwrap();
    }
    result
}