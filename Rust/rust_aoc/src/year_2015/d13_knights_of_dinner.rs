use itertools::Itertools;
use std::collections::{HashMap, HashSet};

pub fn part_one(lines: &Vec<String>) -> String {
    find_best_seat(&lines, false).to_string()
}

pub fn part_two(lines: &Vec<String>) -> String {
    find_best_seat(&lines, true).to_string()
}

fn parse_person<'a>(line: &'a str, preferences: &mut HashMap<&'a str, HashMap<&'a str, i32>>, all_people: &mut HashSet<&'a str>) {
    let s: Vec<&str> = line.split(" ").collect();
    let lhs = s[0];
    let rhs = s.last().unwrap().strip_suffix(".").unwrap();

    let mut quality = s[3].parse().unwrap();
    if s[2] != "gain" {
        quality *= -1;
    }

    all_people.insert(lhs);
    all_people.insert(rhs);
    if !preferences.contains_key(lhs) {
        preferences.insert(lhs, HashMap::new());
    }
    preferences.get_mut(lhs).unwrap().insert(rhs, quality);
}

fn add_self<'a>(preferences: &mut HashMap<&'a str, HashMap<&'a str, i32>>, all_people: &mut HashSet<&'a str>) {
    let you = "You";
    preferences.insert(you, HashMap::new());
    all_people.insert(you);

    for person in all_people.iter() {
        preferences.get_mut(you).unwrap().insert(person, 0);
        preferences.get_mut(person).unwrap().insert(you, 0);
    }
}

fn calc_happiness(people: &Vec<&&str>, preferences: &HashMap<&str, HashMap<&str, i32>>) -> i32 {
    people.windows(2).map(|p| preferences.get(p[0]).unwrap().get(p[1]).unwrap() + preferences.get(p[1]).unwrap().get(p[0]).unwrap()).sum::<i32>()
    + preferences.get(people[0]).unwrap().get(*people.last().unwrap()).unwrap() 
    + preferences.get(*people.last().unwrap()).unwrap().get(people[0]).unwrap()
}

fn find_best_seat(lines: &Vec<String>, include_self: bool) -> i32 {
    let mut preferences: HashMap<&str, HashMap<&str, i32>> = HashMap::new();
    let mut all_people: HashSet<&str> = HashSet::new();
    for line in lines {
        parse_person(line, &mut preferences, &mut all_people);
    }
    if include_self {
        add_self(&mut preferences, &mut all_people);
    }

    all_people.iter().permutations(all_people.len()).map(|v| calc_happiness(&v, &preferences)).max().unwrap()
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn part_one_test() {
        let lines = vec![
            "Alice would gain 54 happiness units by sitting next to Bob.",
            "Alice would lose 79 happiness units by sitting next to Carol.",
            "Alice would lose 2 happiness units by sitting next to David.",
            "Bob would gain 83 happiness units by sitting next to Alice.",
            "Bob would lose 7 happiness units by sitting next to Carol.",
            "Bob would lose 63 happiness units by sitting next to David.",
            "Carol would lose 62 happiness units by sitting next to Alice.",
            "Carol would gain 60 happiness units by sitting next to Bob.",
            "Carol would gain 55 happiness units by sitting next to David.",
            "David would gain 46 happiness units by sitting next to Alice.",
            "David would lose 7 happiness units by sitting next to Bob.",
            "David would gain 41 happiness units by sitting next to Carol."
        ].iter().map(|s| String::from(*s)).collect();

        assert_eq!("330", part_one(&lines));
    }
}
