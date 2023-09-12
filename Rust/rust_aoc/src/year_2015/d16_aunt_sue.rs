use std::collections::HashMap;
use regex::Regex;

pub fn part_one(lines: &Vec<String>) -> String {
    solve(lines, is_exact_match).to_string()
}

pub fn part_two(lines: &Vec<String>) -> String {
    solve(lines, is_range_match).to_string()
}

type Sue = HashMap<String, u32>;

fn get_target_sue() -> Sue {
    let mut result: Sue = HashMap::new();
    result.insert(String::from("children"), 3);
    result.insert(String::from("cats"), 7);
    result.insert(String::from("samoyeds"), 2);
    result.insert(String::from("pomeranians"), 3);
    result.insert(String::from("akitas"), 0);
    result.insert(String::from("vizslas"), 0);
    result.insert(String::from("goldfish"), 5);
    result.insert(String::from("trees"), 3);
    result.insert(String::from("cars"), 2);
    result.insert(String::from("perfumes"), 1);

    result
}

fn parse_sue(line: &str) -> Sue {
    //Sue 1: goldfish: 6, trees: 9, akitas: 0
    let re = Regex::new(r#"(\w+): (\d+)"#).unwrap();

    let mut result = Sue::new();

    re.captures_iter(line).map(|c| {
        let (_, [name, value]) = c.extract();
        (name, value)
    }).for_each(|(name, value)| {
        result.insert(String::from(name), value.parse::<u32>().unwrap());
    });

    result
}

fn is_exact_match(sue: &Sue, target: &Sue) -> bool {
    !target.iter().any(|t| sue.contains_key(t.0) && sue.get(t.0).unwrap() != t.1)
}

fn is_range_match(sue: &Sue, target: &Sue) -> bool {
    for (key, value) in target.iter() {
        if !sue.contains_key(key) {
            continue;
        }

        if  key == "cats" || key == "trees" {
            if sue.get(key).unwrap() <= value {
                return false;
            } 
        }
        else if key == "pomeranians" || key == "goldfish" {
            if sue.get(key).unwrap() >= value {
                 return false;
            }
        }
        else {
            if sue.get(key).unwrap() != value {
                return false;
            }
        }
    }

    true
}

fn solve(lines: &Vec<String>, matcher: fn(&Sue, &Sue)->bool) -> usize {
    let target = get_target_sue();
    for i in 0..lines.len() {
        let sue = parse_sue(&lines[i]);
        if matcher(&sue, &target) {
            return i + 1;
        }
    }
    0
}