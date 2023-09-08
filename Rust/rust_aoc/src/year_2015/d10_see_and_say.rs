pub fn part_one(lines: &Vec<String>) -> String {
    apply_times(&lines[0], 40).len().to_string()
}

pub fn part_two(lines: &Vec<String>) -> String {
    apply_times(&lines[0], 50).len().to_string()
}

fn apply_times(start: &str, times: u32) -> String {
    let mut running = String::from(start);
    for _i in 0..times {
        running = next(&running);
    }
    running
}

fn next(input: &str) -> String {
    let mut result: String = String::new();
    let mut last_char = input.chars().next().unwrap();
    let mut count = 0u32;
    let mut buf = [0; 2];

    for c in input.chars() {
        if c == last_char {
            count += 1;
        } else {
            let to_add = count.to_string() + last_char.encode_utf8(&mut buf);
            result += &to_add;
            last_char = c;
            count = 1;
        }
    }

    let to_add = count.to_string() + last_char.encode_utf8(&mut buf);
    result += &to_add;

    result
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn next_tests() {
        assert_eq!(next("1"), "11");
        assert_eq!(next("11"), "21");
        assert_eq!(next("21"), "1211");
        assert_eq!(next("1211"), "111221");
        assert_eq!(next("111221"), "312211");
    }
}