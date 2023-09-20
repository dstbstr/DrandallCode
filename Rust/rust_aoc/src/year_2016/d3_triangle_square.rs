use crate::input_utils::parse_line_as_numbers;

pub fn part_one(lines: &Vec<String>) -> String {
    lines.iter().map(|line| parse_line_as_numbers(line, Some(" ")))
    .filter(|v| is_valid(v[0], v[1], v[2]))
    .count().to_string()
}
pub fn part_two(lines: &Vec<String>) -> String {
    lines.chunks(3).flat_map(|three| {
        let s1 = parse_line_as_numbers(&three[0], Some(" "));
        let s2 = parse_line_as_numbers(&three[1], Some(" "));
        let s3 = parse_line_as_numbers(&three[2], Some(" "));

        vec![
            vec![s1[0], s2[0], s3[0]],
            vec![s1[1], s2[1], s3[1]],
            vec![s1[2], s2[2], s3[2]]
        ]
    })
    .filter(|v| is_valid(v[0], v[1], v[2]))
    .count().to_string()
}

fn is_valid(a: u32, b: u32, c: u32) -> bool {
    if a > b && a > c {
        return b + c > a;
    } else if b > a && b > c {
        return a + c > b;
    } else {
        return a + b > c;
    }
}

