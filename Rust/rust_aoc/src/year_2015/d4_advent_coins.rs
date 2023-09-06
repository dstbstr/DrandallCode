use md5;

pub fn part_one(lines: &Vec<String>) -> String {
    solve(&lines[0], "00000", Some(100_000)).to_string()
}

pub fn part_two(lines: &Vec<String>) -> String {
    solve(&lines[0], "000000", Some(3_900_000)).to_string()
}

fn solve(line: &str, prefix: &str, hint: Option<usize>) -> usize {
    let mut result = hint.unwrap_or(0);

    loop {
        let key = String::from(line) + &result.to_string();
        let hash = format!("{:x}", md5::compute(key));
        if hash.starts_with(prefix) {
            break;
        }
        result += 1;
    }

    result
}