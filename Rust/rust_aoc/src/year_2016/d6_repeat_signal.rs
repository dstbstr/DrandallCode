use itertools::Itertools;

pub fn part_one(lines: &Vec<String>) -> String {
    decode(lines, false)
}
pub fn part_two(lines: &Vec<String>) -> String {
    decode(lines, true)
}

fn decode(lines: &Vec<String>, use_min: bool) -> String {
    let first_letters: Vec<String> = (0..lines[0].len())
        .map(|i| lines.iter().map(|line| line.as_bytes()[i] as char).collect())
        .collect();

    first_letters.iter().map(|letters| {
        let letter_counts = letters.chars().into_group_map_by(|&x| x)
        .into_iter().map(|(k, v)| (k, v.len()));
        let (letter, _count) = match use_min {
            true => letter_counts.min_by(|(_, lhs), (_, rhs)| lhs.cmp(rhs)).unwrap(),
            false => letter_counts.max_by(|(_, lhs), (_, rhs)| lhs.cmp(rhs)).unwrap()
        };
        letter
        }
    ).collect()
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn first_example() {
        let entries = vec!["eedadn", "drvtee", "eandsr", "raavrd", "atevrs", "tsrnev", "sdttsa", "rasrtv", "nssdts", "ntnada", "svetve", "tesnvt", "vntsnd", "vrdear", "dvrsen", "enarar"].into_iter().map(|s| String::from(s)).collect();
        assert_eq!("easter", decode(&entries, false));
        assert_eq!("advent", decode(&entries, true));

    }
}
