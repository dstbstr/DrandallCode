use rust_core::facing::Facing;
use rust_core::geometry::Coord;
use std::collections::HashSet;

pub fn part_one(lines: &Vec<String>) -> String {
    solve(&lines[0], |_| false).to_string()
}
pub fn part_two(lines: &Vec<String>) -> String {
    use std::mem;
    let mut seen: HashSet<Coord> = HashSet::new().to_owned();
    let result = solve(&lines[0], |pos| !seen.insert(*pos)).to_string();
    mem::drop(seen);
    result
}

fn solve<F: FnMut(&Coord) -> bool>(line: &str, mut early_out: F) -> isize {
    let mut facing = Facing::Up;
    let mut pos = Coord::new(0, 0);
    let mut done = false;

    for step in line.split(", ") {
        match step.as_bytes()[0] {
            b'L' => facing = facing.turn_left(),
            b'R' => facing = facing.turn_right(),
            _ => panic!("Unrecognized turn")
        };
        let distance = step[1..].parse().unwrap();
        for _ in 0..distance {
            facing.move_coord(&mut pos);
            if early_out(&pos) {
                done = true;
                break;
            }
        }

        if done {
            break;
        }
    }
    pos.mdistance_origin()
}