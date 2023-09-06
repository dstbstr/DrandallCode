
pub fn part_one(lines: &Vec<String>) -> String {
    solve(lines, calc_paper).to_string()
}

pub fn part_two(lines: &Vec<String>) -> String {
    solve(lines, calc_ribbon).to_string()
}

fn solve(lines: &Vec<String>, func: fn(u32, u32, u32)->u32) -> u32 {
    let mut result: u32 = 0;
    for line in lines {
        let parts: Vec<u32> = line.split("x").map(|d| d.parse().unwrap()).collect();
        let (width, height, depth) = (parts[0], parts[1], parts[2]);
        result += func(width, height, depth)
    }

    result
}

fn calc_paper(width: u32, height: u32, depth: u32) -> u32 {
    let d1 = width * height;
    let d2 = width * depth;
    let d3 = height * depth;

    (d1 * 2) + (d2 * 2) + (d3 * 2) + std::cmp::min(d1, std::cmp::min(d2, d3))
}

fn calc_ribbon(width: u32, height: u32, depth: u32) -> u32 {
    let area = width * height * depth;
    (area + (width * 2) + (height * 2) + (depth * 2)) - (std::cmp::max(width, std::cmp::max(height, depth)) * 2)
}