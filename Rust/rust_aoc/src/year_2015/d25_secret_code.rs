use crate::input_utils::get_numbers_from_string;

pub fn part_one(lines: &Vec<String>) -> String {
    let values = get_numbers_from_string(&lines[0]);
    calc_code(calc_index(values[0], values[1])).to_string()
}

fn calc_index(row: usize, col: usize) -> usize {
    let mut top_row = row + col - 1;
    top_row = (top_row * (top_row + 1)) / 2;
    top_row - row + 1
}

fn calc_code(index: usize) -> usize {
    let mut running = 20151125usize;
    for _ in 1..index {
        running *= 252533;
        running %= 33554393;
    }

    running
}
