use std::collections::{HashMap, HashSet};
use crate::input_utils::split_input_into_groups;

type Transforms<'a> = HashMap<&'a str, Vec<&'a str>>;
type RTransforms<'a> = HashMap<&'a str, &'a str>;

pub fn part_one(lines: &Vec<String>) -> String {
    let groups = split_input_into_groups(&lines);
    let (transforms, _) = parse_transforms(&groups[0]);

    count_molecules(&groups[1][0], &transforms).to_string()
}

pub fn part_two(lines: &Vec<String>) -> String {
    let groups = split_input_into_groups(&lines);
    let (_, reverse_transforms) = parse_transforms(&groups[0]);
    let mut steps = 0usize;
    let mut running = groups[1][0].clone();

    loop {
        if !collapse_molecule(&mut running, &reverse_transforms) {
            break;
        }
        steps += 1;
    }

    steps.to_string()
}

fn parse_transforms(lines: &Vec<String>) -> (Transforms, RTransforms) {
    let mut transforms = Transforms::new();
    let mut reverses = RTransforms::new();

    for line in lines {
        let s: Vec<&str> = line.split(" => ").collect();
        let from = s[0];
        let to = s[1];

        let values = transforms.entry(from).or_default();
        values.push(to);

        reverses.insert(to, from);
    }

    (transforms, reverses)
}

fn count_molecules(initial: &str, transforms: &Transforms) -> usize {
    let mut molecules: HashSet<String> = HashSet::new();

    for (key, values) in transforms {
        for m in initial.match_indices(key) {
            for v in values {
                let mut next = String::from(initial);
                next.replace_range(m.0 ..m.0 + m.1.len(), v);
                molecules.insert(next);
            }
        }
    }

    molecules.len()
}

fn collapse_molecule(molecule: &mut String, reverses: &RTransforms) -> bool {
    for (key, value) in reverses {
        if let Some(m) = molecule.find(key) {
            molecule.replace_range(m..m + key.len(), value);
            return true;
        }
    }
    false
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn part_one_test() {
        let lines: Vec<String> = vec!["H => HO", "H => OH", "O => HH", "", "HOH"].iter().map(|s| String::from(*s)).collect();
        assert_eq!("4", part_one(&lines));
    }
}