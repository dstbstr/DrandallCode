pub fn part_one(lines: &Vec<String>) -> String {
    decompress(&lines[0], false).to_string()
}
pub fn part_two(lines: &Vec<String>) -> String {
    decompress(&lines[0], true).to_string()
}

fn decompress(line: &str, recurse: bool) -> usize {
    let mut result: usize = 0;
    let mut index: usize = 0;

    let bytes = line.as_bytes();
    while index < bytes.len() {
        if bytes[index] != b'(' {
            result += 1;
            index += 1;
        } else {
            let close_distance = line[index..].find(")").unwrap();
            let close_index = index + close_distance;
            let expr = std::str::from_utf8(&bytes[index + 1..close_index]).unwrap();
            let s = expr.split("x").collect::<Vec<&str>>();
            let run_length = s[0].parse::<usize>().unwrap();
            let repeat_count = s[1].parse::<usize>().unwrap();

            let next_expr_index = close_index + 1;
            if recurse {
                result += repeat_count * decompress(std::str::from_utf8(&bytes[next_expr_index..next_expr_index + run_length]).unwrap(), recurse);
            } else {
                result += repeat_count * run_length;
            }
            index = next_expr_index + run_length;
        }
    }

    result
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn part_one_examples() {
        assert_eq!(6, decompress("ADVENT", false));
        assert_eq!(7, decompress("A(1x5)BC", false));
        assert_eq!(9, decompress("(3x3)XYZ", false));
        assert_eq!(11, decompress("A(2x2)BCD(2x2)EFG", false));
        assert_eq!(6, decompress("(6x1)(1x3)", false));
        assert_eq!(18, decompress("X(8x2)(3x3)ABCY", false));
    }

    #[test]
    fn part_two_examples() {
        assert_eq!(241920, decompress("(27x12)(20x12)(13x14)(7x10)(1x12)A", true));
        assert_eq!(445, decompress("(25x3)(3x3)ABC(2x3)XY(5x2)PQRSTX(18x9)(3x2)TWO(5x7)SEVEN", true));
    }
}