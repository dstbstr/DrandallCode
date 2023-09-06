pub fn part_one(lines: &Vec<String>) -> String {
    find_floor(&lines[0]).to_string()
}
pub fn part_two(lines: &Vec<String>) -> String {
    find_basement_step(&lines[0]).to_string()
}

fn find_floor(val: &str) -> isize {
    let ups = val.chars().filter(|c| c == &'(').count();
    let downs = val.chars().filter(|c| c == &')').count();
    ups as isize - downs as isize
}

fn find_basement_step(val: &str) -> i32 {
    let mut current_floor: i32 = 0;
    let mut index = 0;
    for c in val.chars() {
        index += 1;
        if c == '(' {
            current_floor += 1;
        } else {
            current_floor -= 1;
            if current_floor < 0 {
                return index;
            }
        }
    }

    0
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn find_floor_tests() {
        assert_eq!(find_floor(""), 0);
        assert_eq!(find_floor("(())"), 0);
        assert_eq!(find_floor("()()"), 0);
        assert_eq!(find_floor("((("), 3);
        assert_eq!(find_floor("(()(()("), 3);
        assert_eq!(find_floor("())"), -1);
        assert_eq!(find_floor("))("), -1);
        assert_eq!(find_floor(")))"), -3);
        assert_eq!(find_floor(")())())"), -3);
    }

    #[test]
    fn find_basement_step_tests() {
        assert_eq!(find_basement_step(")"), 1);
        assert_eq!(find_basement_step("()())"), 5);
    }

}
