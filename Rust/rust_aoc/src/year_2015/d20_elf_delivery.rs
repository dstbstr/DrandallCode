use rust_core::math;

pub fn part_one(lines: &Vec<String>) -> String {
    solve(&lines[0], count_presents, 700_000).to_string()
}

pub fn part_two(lines: &Vec<String>) -> String {
    solve(&lines[0], count_lazy_presents, 800_000).to_string()
}

fn count_presents(house_number: u32) -> u32 {
    math::get_divisors(house_number).sum::<u32>() * 10
}

fn count_lazy_presents(house_number: u32) -> u32 {
    let threshold = house_number / 50;
    math::get_divisors(house_number)
    .filter(|v| v > &threshold)
    .sum::<u32>() * 11
}

fn solve(line: &str, func: fn(u32) -> u32, hint: u32) -> u32 {
    let mut current = hint;
    let target: u32 = line.parse().unwrap();
    while func(current) < target {
        current += 1;
    }

    current
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn count_presents_tests() {
        assert_eq!(10, count_presents(1));
        assert_eq!(30, count_presents(2));
        assert!(count_presents(786240) > 34_000_000);
    }

    #[test]
    fn count_lazy_presents_tests() {
        assert_eq!(11, count_lazy_presents(1));
        assert_eq!(33, count_lazy_presents(2));
    }
}