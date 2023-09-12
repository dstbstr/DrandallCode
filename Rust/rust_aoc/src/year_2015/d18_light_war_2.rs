use rust_core::geometry::Coord;
use std::collections::HashSet;

pub fn part_one(lines: &Vec<String>) -> String {
    let mut grid = build_grid(&lines);
    for _ in 0..100 {
        grid = next(&grid);
    }

    grid.len().to_string()
}

pub fn part_two(lines: &Vec<String>) -> String {
    let mut grid = build_grid(&lines);
    set_corners(&mut grid);
    for _ in 0..100 {
        grid = next(&grid);
        set_corners(&mut grid);
    }

    grid.len().to_string()
}

type Grid = HashSet<Coord>;

fn build_grid(input: &Vec<String>) -> Grid {
    let mut result = Grid::new();
    for x in 0..input.len() {
        for y in 0..input[x].len() {
            if input[x].as_bytes()[y] == b'#' {
                result.insert(Coord::new(usize::try_into(x).unwrap(), usize::try_into(y).unwrap()));
            }
        }
    }
    
    result
}

fn count_lit_neighbors(grid: &Grid, pos: Coord) -> usize {
    pos.get_all_neighbors().iter().filter(|p| grid.contains(p)).count()
}

fn next(grid: &Grid) -> Grid {
    let mut result = Grid::new();
    for x in 0isize..100 {
        for y in 0isize..100 {
            let pos = Coord::new(x, y);
            let lit = count_lit_neighbors(grid, pos);
            if (grid.contains(&pos) && lit == 2 || lit == 3) || (!grid.contains(&pos) && lit == 3) {
                result.insert(pos);
            }
        }
    }
    result
}

fn set_corners(grid: &mut Grid) {
    grid.insert(Coord::new(0, 0));
    grid.insert(Coord::new(0, 99));
    grid.insert(Coord::new(99, 0));
    grid.insert(Coord::new(99, 99));
}