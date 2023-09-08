pub fn part_one(lines: &Vec<String>) -> String {
    lines.iter().map(|l| calc_delta(l)).sum::<usize>().to_string()
}

pub fn part_two(lines: &Vec<String>) -> String {
    lines.iter().map(|l| calc_growth(l)).sum::<usize>().to_string()
}

fn count_code_chars(input: &str) -> usize {
    input.len()
}
fn count_memory_chars(input: &str) -> usize {
    let mut result: usize = 0;
    let mut is_escaping = false;
    for c in input.chars() {
        if is_escaping {
            match c {
                'x' => result -= 1,
                _ => result += 1
            }
            is_escaping = false
        } else {
            match c {
                '\\' => is_escaping = true,
                _ => result += 1
            }
        }
    }

    result - 2
}

fn calc_delta(input: &str) -> usize {
    count_code_chars(input) - count_memory_chars(input)
}

fn count_reencoded_chars(input: &str) -> usize {
    input.len() + 2 + input.chars().filter(|c| *c == '\\' || *c == '"').count()
}

fn calc_growth(input: &str) -> usize {
    count_reencoded_chars(input) - count_code_chars(input)
}

#[cfg(test)]
mod tests {
    use super::*;

    const EMPTY_STRING: &str = r#""""#;
    const QUOTED_STRING: &str = r#""abc""#;
    const ESCAPED_QUOTE: &str = r#""aaa\"aaa""#;
    const HEX_STRING: &str = r#""\x27""#;

    #[test]
    fn count_code_chars_tests() {
        assert_eq!(2, count_code_chars(EMPTY_STRING));
        assert_eq!(5, count_code_chars(QUOTED_STRING));
        assert_eq!(10, count_code_chars(ESCAPED_QUOTE));
        assert_eq!(6, count_code_chars(HEX_STRING));
    }

    #[test]
    fn count_memory_chars_tests() {
        assert_eq!(0, count_memory_chars(EMPTY_STRING));
        assert_eq!(3, count_memory_chars(QUOTED_STRING));
        assert_eq!(7, count_memory_chars(ESCAPED_QUOTE));
        assert_eq!(1, count_memory_chars(HEX_STRING));
    }

    #[test]
    fn calc_delta_tests() {
        assert_eq!(2, calc_delta(EMPTY_STRING));
        assert_eq!(2, calc_delta(QUOTED_STRING));
        assert_eq!(3, calc_delta(ESCAPED_QUOTE));
        assert_eq!(5, calc_delta(HEX_STRING));
    }

    #[test]
    fn count_reencoded_tests() {
        assert_eq!(6, count_reencoded_chars(EMPTY_STRING));
        assert_eq!(9, count_reencoded_chars(QUOTED_STRING));
        assert_eq!(16, count_reencoded_chars(ESCAPED_QUOTE));
        assert_eq!(11, count_reencoded_chars(HEX_STRING));
    }

    #[test]
    fn calc_growth_tests() {
        assert_eq!(4, calc_growth(EMPTY_STRING));
        assert_eq!(4, calc_growth(QUOTED_STRING));
        assert_eq!(6, calc_growth(ESCAPED_QUOTE));
        assert_eq!(5, calc_growth(HEX_STRING));
    }
}
