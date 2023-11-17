use itertools::Itertools;

pub fn part_one(lines: &Vec<String>) -> String {
    let screen = build_screen(&lines);
    let lit: usize = screen.iter().map(|row| row.iter().filter(|r| **r).count()).sum();
    lit.to_string()
}

pub fn part_two(lines: &Vec<String>) -> String {
    let screen = build_screen(&lines);
    let mut result = String::from("\n");
    result += &screen.iter().map(|line| line.iter().map(|p| {
        if *p {
            '#'
        } else {
            ' '
        }
    }).collect::<String>()).join("\n");
    result
}

fn build_screen(lines: &Vec<String>) -> [[bool; 50]; 6] {
    let mut screen = [[false; 50]; 6];
    for line in lines {
        let split = line.split(" ").collect::<Vec<&str>>();
        match split[1] {
            "row" => rot_row(&mut screen, split[2][2..].parse().unwrap(), split[4].parse().unwrap()),
            "column" => rot_col(&mut screen, split[2][2..].parse().unwrap(), split[4].parse().unwrap()),
            _ => rect(&mut screen, split[1])
        }
    }

    screen
}

fn rect(screen: &mut [[bool; 50]; 6], dimensions: &str) {
    let s = dimensions.split("x").collect::<Vec<&str>>();
    rect_impl(screen, s[0].parse().unwrap(), s[1].parse().unwrap());
}

fn rect_impl(screen: &mut [[bool; 50]; 6], cols: usize, rows: usize) {
    for row in 0..rows {
        for col in 0..cols {
            screen[row][col] = true;
        }
    }
}

fn rot_row(screen: &mut [[bool; 50]; 6], row: usize, amount: usize) {
    let mut temp = [false; 50];
    for col in 0..50 {
        temp[col] = screen[row][(50 + col - amount) % 50];
    }
    screen[row] = temp;
}

fn rot_col(screen: &mut [[bool; 50]; 6], col: usize, amount: usize) {
    let mut temp = [false; 6];
    for row in 0..6 {
        temp[row] = screen[(6 + row - amount) % 6][col];
    }
    for row in 0..6 {
        screen[row][col] = temp[row];
    }
}
