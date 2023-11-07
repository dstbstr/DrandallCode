pub fn part_one(lines: &Vec<String>) -> String {
    lines.iter()
        .filter(|line| supports_tls(line))
        .count()
        .to_string()
}
pub fn part_two(lines: &Vec<String>) -> String {
    lines.iter()
        .filter(|line| supports_ssl(line))
        .count()
        .to_string()
}

fn get_chunks(line: &str) -> (Vec<String>, Vec<String>) {
    line.split("[")
        .flat_map(|s| s.split("]"))
        .map(|s| String::from(s))
        .fold((Vec::new(), Vec::new()), |(mut outside, mut inside), next| {
            if outside.len() == inside.len() {
                outside.push(next.clone());
            } else {
                inside.push(next.clone());
            }
            (outside, inside)
        })
}

fn contains_abba(val: &str) -> bool {
    val.as_bytes().windows(4).any(|p| p[0] == p[3] && p[1] == p[2] && p[0] != p[1])
}

fn supports_tls(val: &str) -> bool {
    let (outside, inside) = get_chunks(val);
    outside.iter().any(|l| contains_abba(l)) && inside.iter().all(|l| !contains_abba(l))
}

fn get_threes(strs: &Vec<String>) -> Vec<&str> {
    strs.iter().map(|s| s.as_bytes().windows(3)).flatten()
        .filter(|w| w[0] == w[2] && w[0] != w[1])
        .map(|c| std::str::from_utf8(c).unwrap())
        .collect()
}

fn contains_key(line: &str, keys: &Vec<String>) -> bool {
    keys.iter().any(|key| line.contains(key))
}

fn supports_ssl(val: &str) -> bool {
    let (outside, inside) = get_chunks(val);
    let keys = get_threes(&outside).iter().map(|key| {
        let bytes = key.as_bytes();
        String::from_utf8(vec![bytes[1], bytes[0], bytes[1]]).unwrap()
    }).collect();
    inside.iter().any(|l| contains_key(l, &keys))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn get_chunks_test() {
        let val = "abba[mnop]qrst";
        let (outside, inside) = get_chunks(&val);
        assert_eq!("abba", outside[0]);
        assert_eq!("mnop", inside[0]);
    }

    #[test]
    fn contains_abba_test() {
        assert!(contains_abba("abba"));
        assert!(contains_abba("xyyx"));
        assert!(!contains_abba("abcd"));
        assert!(!contains_abba("aaaa"));
    }

    #[test]
    fn supports_tls_test() {
        assert!(supports_tls("abba[mnop]qrst"));
        assert!(supports_tls("ioxxoj[asdfgh]zxcvbn"));

        assert!(!supports_tls("abcd[bddb]xyyx"));
        assert!(!supports_tls("aaaa[qwer]tyui"));
    }

    #[test]
    fn supports_ssl_test() {
        assert!(supports_ssl("aba[bab]xyz"));
        assert!(supports_ssl("aaa[kek]eke"));
        assert!(supports_ssl("zazbz[bzb]cdb"));

        assert!(!supports_ssl("xyx[xyx]xyx"));
    }
}
