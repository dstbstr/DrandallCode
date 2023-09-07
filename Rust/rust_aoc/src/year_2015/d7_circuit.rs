use std::collections::HashMap;

pub fn part_one(lines: &Vec<String>) -> String {
    solve(lines).to_string()
}

pub fn part_two(lines: &Vec<String>) -> String {
    let part_one_result = part_one(lines);
    let mut copy = lines.clone();

    for line in copy.iter_mut() {
        if line.ends_with("-> b") {
            *line = part_one_result.clone() + " -> b";
            break;
        }
    }
    solve(&copy).to_string()
}

pub fn solve(lines: &Vec<String>) -> u16 {
    let mut resolved: HashMap<&str, u16> = HashMap::new();
    let entries: Vec<Entry> = lines.iter().map(|l| parse_entry(l)).collect();

    while resolved.len() < lines.len() {
        let to_add: Vec<(&str, u16)> = entries.iter()
            .filter(|e| !resolved.contains_key(e.output))
            .filter_map(|e| try_resolve(e, &resolved)).collect();
        
        if to_add.len() == 0 {
            panic!("Failed to find new entries");
        }
        for e in to_add {
            resolved.insert(e.0, e.1);
        }
    }

    *resolved.get(&"a").unwrap()
}
struct Entry<'a> {
    parts: Vec<&'a str>,
    output: &'a str
}

fn try_resolve<'a, 'b>(entry: &'a Entry, resolved: &HashMap<&'b str, u16>) -> Option<(&'a str, u16)> {
    match entry.parts.len() {
        1 => match try_resolve_part(entry.parts[0], resolved) {
                Some(v) => Some((&entry.output, v)),
                None => None
        },
        2 => match try_resolve_part(entry.parts[1], resolved) {
                Some(v) => Some((&entry.output, !v)),
                None => None
        },
        3 => {
            let lhs = try_resolve_part(entry.parts[0], resolved);
            let rhs = try_resolve_part(entry.parts[2], resolved);
            match (lhs, entry.parts[1], rhs) {
                (Some(left), "AND", Some(right)) => Some((&entry.output, left & right)),
                (Some(left), "OR", Some(right)) => Some((&entry.output, left | right)),
                (Some(left), "LSHIFT", Some(right)) => Some((&entry.output, left << right)),
                (Some(left), "RSHIFT", Some(right)) => Some((&entry.output, left >> right)),
                _ => None
            }
        }
        _ => None
    }
}

fn try_resolve_part(part: &str, resolved: &HashMap<&str, u16>) -> Option<u16> {
    match resolved.get(part) {
        Some(v) => Some(*v),
        None => {
            match part.parse::<u16>() {
                Ok(res) => Some(res),
                Err(..) => None
            }
        }
    }
}
/*
hv OR hu -> hw
19138 -> b
NOT ft -> fu
*/

fn parse_entry(line: &str) -> Entry {
    let s: Vec<&str> = line.split(" -> ").collect();
    Entry{parts: s[0].split(" ").collect(), output: s[1]}
}
/*
fn parse_input(lines: &Vec<String>) -> HashMap<&str, String> {
    lines.iter().map(|l| { 
        let s: Vec<&str> = l.split(" -> ").collect();
        (s[0], s[1].to_string())
    }).collect()
}

fn get_value(in_str: &str, mapping: &mut HashMap<&str, String>) -> u16 {
    if mapping.contains_key(in_str) {
        resolve(in_str, mapping);
        return mapping.get(in_str).unwrap().parse::<u16>().unwrap();
    }
    in_str.parse::<u16>().unwrap()
}

fn resolve(key: &str, mapping: &mut HashMap<&str, String>) {
    let s: Vec<String> = mapping.get(key).unwrap().split(" ").map(|s| String::from(s)).collect();
    //let s: Vec<&str> = expr.split(" ").collect();
    match s.len() {
        1 => {
            let new_val = get_value(&s[0], mapping);
            mapping.insert(key, new_val.to_string());
        },
        2 => {
            let new_val = !get_value(&s[1], mapping);
            mapping.insert(key, new_val.to_string());
        },
        _ => panic!("Unexpected number of args")
    };
}
*/