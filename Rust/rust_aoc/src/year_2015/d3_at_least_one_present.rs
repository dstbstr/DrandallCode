use std::collections::HashSet;
use rust_core::geometry::Coord;

pub fn part_one(lines: &Vec<String>) -> String {
    solve(&lines[0], 1).to_string()
}

pub fn part_two(lines: &Vec<String>) -> String {
    solve(&lines[0], 2).to_string()
}

fn solve(line: &str, movers: usize) -> usize {
    let mut seen: HashSet<Coord> = HashSet::new();
    let mut positions: Vec<Coord> = Vec::new();
    let mut current_mover: usize = 0;

    for _ in 0..movers {
        positions.push(Coord::new(0, 0));
    }

    for c in line.chars() {
        do_move(&mut positions[current_mover], &c);
        seen.insert(positions[current_mover]);
        current_mover = (current_mover + 1) % movers;
    }

    seen.len()
}

fn do_move(pos: &mut Coord, c: &char) {
    match c {
        '>' => pos.x += 1,
        '<' => pos.x -= 1,
        '^' => pos.y -= 1,
        'v' => pos.y += 1,
        _ => panic!("Unrecognized direction")
    }
}