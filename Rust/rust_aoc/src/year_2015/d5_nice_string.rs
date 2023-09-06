
pub fn part_one(lines: &Vec<String>) -> String {
    solve(lines, nice_string).to_string()
}

pub fn part_two(lines: &Vec<String>) -> String {
    solve(lines, really_nice).to_string()
}

fn solve(lines: &Vec<String>, pred: fn(&str) -> bool) -> usize {
    lines.iter().filter(|l| pred(l)).count()
}

const VOWELS: &'static [char] = &['a', 'e', 'i', 'o', 'u'];
const BAD_PAIRS: &'static [&'static str] = &["ab", "cd", "pq", "xy"];

fn nice_string(line: &str) -> bool {
    (line.split(VOWELS).count() > 3) &&
    !BAD_PAIRS.iter().any(|p| line.contains(p)) &&
    line.as_bytes().windows(2).any(|p| p[0] == p[1])
}

fn really_nice(line: &str) -> bool {
    let bytes = line.as_bytes();

    bytes.windows(3).any(|p| p[0] == p[2] && {
        bytes.windows(2).enumerate().any(|(i, pair)| line.rfind(std::str::from_utf8(pair).unwrap()).map(|index| index > i + 1).unwrap_or(false))
    })
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn nice_string_tests() {
        assert!(nice_string("ugknbfddgicrmopn"));
        assert!(nice_string("aaa"));

        assert!(!nice_string("jchzalrnumimnmhp"));
        assert!(!nice_string("haegwjzuvuyypxyu"));
        assert!(!nice_string("dvszwmarrgswjxmb"));
        assert!(!nice_string("zztdcqzqddaazdjp"));
    }

    #[test]
    fn really_nice_tests() {
        assert!(really_nice("qjhvhtzxzqqjkmpb"));
        assert!(really_nice("xxyxx"));

        assert!(!really_nice("uurcxstgmygtbstg"));
        assert!(!really_nice("ieodomkazucvgmuy"));
    }

}
