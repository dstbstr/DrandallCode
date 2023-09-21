use md5;

const PART_ONE_HINTS: &'static[i32; 8] = &[1739529, 1910966, 1997199, 2854555, 2963716, 3237361, 4063427, 7777889];

pub fn part_one(lines: &Vec<String>) -> String {
    solve_part_one(lines, Some(PART_ONE_HINTS))
}

fn solve_part_one(lines: &Vec<String>, hints: Option<&[i32; 8]>) -> String {
    let prefix = lines[0].clone();
    match hints {
        Some(hints) => {
            hints.iter().map(|hint| prefix.clone() + &hint.to_string())
            .map(|attempt| format!("{:x}", md5::compute(attempt)))
            .map(|hex| hex.as_bytes()[5] as char)
            .collect()    
        },
        None => {
            let mut index = 0;
            let mut result = String::new();
            while result.len() < 8 {
                let attempt = prefix.clone() + &index.to_string();
                let hex = format!("{:x}", md5::compute(attempt));
                if hex.starts_with("00000") {
                    result.push(hex.as_bytes()[5] as char);
                }
                index += 1;
            }
            result                
        }
    }
}
const PART_TWO_HINTS: &'static[i32; 8] = &[1910966, 1997199, 2963716, 3237361, 7777889, 12850790, 12942170, 25651067];

pub fn part_two(lines: &Vec<String>) -> String {
    solve_part_two(lines, Some(PART_TWO_HINTS))
}

fn solve_part_two(lines: &Vec<String>, hints: Option<&[i32; 8]>) -> String {
    let prefix = lines[0].clone();

    match hints {
        Some(hints) => {
            let mut pairs = hints.map(|hint| prefix.clone() + &hint.to_string())
            .map(|attempt| format!("{:x}", md5::compute(attempt)))
            .map(|hex| (hex.as_bytes()[5] - b'0', hex.as_bytes()[6]))
            .into_iter()
            .collect::<Vec<_>>();
            pairs.sort_by(|(lhs, _), (rhs, _)| lhs.cmp(rhs));
            pairs.iter().map(|(_, c)| *c as char)
            .collect::<String>()
        },
        None => {
            let mut result = String::from("________");
            let mut index = 0;
            let mut found_count = 0;
        
            while found_count < 8 {
                let attempt = prefix.clone() + &index.to_string();
                let hex = format!("{:x}", md5::compute(attempt));
                if hex.starts_with("00000")  && hex.as_bytes()[5] < b'8' {
                    let pos = hex.as_bytes()[5] - b'0';
                    if result.as_bytes()[pos as usize] == b'_' {
                        unsafe {
                            result.as_bytes_mut()[pos as usize] = hex.as_bytes()[6];
                        }
                        found_count += 1;
                    }
                }
                index += 1;
            }
            return result;
        }
    }

}
