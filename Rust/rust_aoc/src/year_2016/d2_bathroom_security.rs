use rust_core::geometry::UCoord;
use rust_core::facing::Facing;

type Keypad = Vec<String>;

pub fn part_one(lines: &Vec<String>) -> String {
    let keypad = vec!["     ", " 123 ", " 456 ", " 789 ", "     "].iter().map(|s| s.to_string()).collect();
    solve(lines, keypad, UCoord::new(2, 2))
}
pub fn part_two(lines: &Vec<String>) -> String {
    let keypad = vec!["       ", "   1   ", "  234  ", " 56789 ", "  ABC  ", "   D   ", "       "].iter().map(|s| s.to_string()).collect();
    solve(lines, keypad, UCoord::new(1, 3))
}

fn solve(lines: &Vec<String>, keypad: Keypad, mut pos: UCoord) -> String {
    let mut result = String::new();
    for line in lines {
        follow_path(&line, &keypad, &mut pos);
        result.push(char_at(&keypad, &pos));
    }

    result
}

fn follow_path(line: &str, keypad: &Keypad, pos: &mut UCoord) {
    for c in line.chars() {
        let mut next_pos = pos.clone();
        Facing::parse(c).move_ucoord(&mut next_pos);
        if char_at(&keypad, &next_pos) != ' ' {
            *pos = next_pos;
        }
    }
}

fn char_at(keypad: &Keypad, pos: &UCoord) -> char {
    keypad[pos.y].as_bytes()[pos.x] as char
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn first_example() {
        let lines = vec!["ULL", "RRDDD", "LURDL", "UUUUD"].iter().map(|s| s.to_string()).collect();
        let keypad = vec!["     ", " 123 ", " 456 ", " 789 ", "     "].iter().map(|s| s.to_string()).collect();
        assert_eq!("1985", solve(&lines, keypad, UCoord::new(2, 2)));
    }
}