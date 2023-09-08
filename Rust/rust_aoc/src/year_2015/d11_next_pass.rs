pub fn part_one(lines: &Vec<String>) -> String {
    next_pass(&lines[0])
}

pub fn part_two(lines: &Vec<String>) -> String {
    next_pass(&next_pass(&lines[0]))
}

fn next_pass(previous: &str) -> String {
    let mut result = String::from(previous);
    loop {
        increment(&mut result);
        replace_invalid(&mut result);
        if is_valid(&result) {
            break;
        }
    }

    result
}

fn increment(input: &mut String) {
    unsafe {
        for c in input.as_bytes_mut().iter_mut().rev() {
            if *c == b'z' {
                *c = b'a';
            } else {
                *c += 1;
                break;
            }
        }
    }
}

fn replace_invalid(input: &mut String) {
    let mut clear_rest = false;

    unsafe {
        for c in input.as_bytes_mut().iter_mut() {
            if clear_rest {
                *c = b'a';
            }
            if *c == b'i' || *c == b'o' || *c == b'l' {
                *c += 1;
                clear_rest = true;
            }
        }
    }
}

fn is_valid(input: &str) -> bool {
    contains_straight(input) && contains_two_pairs(input)
}

fn contains_straight(input: &str) -> bool {
    input.as_bytes().windows(3).any(|a| a[0] == (a[1] - 1) && a[0] == (a[2] - 2))
}

fn contains_two_pairs(input: &str) -> bool {
    input.as_bytes().windows(5).any(|a| a[0] == a[1] && a[0] != a[2] && a[2] != a[3] && a[3] == a[4])
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn increment_tests() {
        let mut val = String::from("a");
        increment(&mut val);
        assert_eq!(val, "b");
        
        val = String::from("z");
        increment(&mut val);
        assert_eq!(val, "a");

        val = String::from("azzz");
        increment(&mut val);
        assert_eq!(val, "baaa");

        val = String::from("xyz");
        increment(&mut val);
        assert_eq!(val, "xza");
    }

    #[test]
    fn replace_invalid_tests() {
        let mut val = String::from("oil");
        replace_invalid(&mut val);
        assert_eq!(val, "paa");

        val = String::from("happy");
        replace_invalid(&mut val);
        assert_eq!(val, "happy");
    }

    #[test]
    fn contains_straight_tests() {
        assert!(contains_straight("abc"));
        assert!(!contains_straight("aabbc"));
        assert!(contains_straight("aabcc"));
        assert!(!contains_straight("cba"));
        assert!(!contains_straight("aaa"));
    }

    #[test]
    fn contains_two_pairs_tests() {
        assert!(contains_two_pairs("aacbb"));
        assert!(!contains_two_pairs("abcde"));
        assert!(!contains_two_pairs("aaa"));
        assert!(contains_two_pairs("aabaa"));
    }

    #[test]
    fn is_valid_tests() {
        assert!(!is_valid("abbceffg"));
        assert!(is_valid("abcffdaa"));
        assert!(is_valid("ghjaabcc"));
    }

    #[test]
    fn next_pass_tests() {
        assert_eq!(next_pass("ghijklmn"), "ghjaabcc");
        assert_eq!(next_pass("vzbxkghb"), "vzbxxyzz");
        assert_eq!(next_pass("vzbxxyzz"), "vzcaabcc");
    }
}