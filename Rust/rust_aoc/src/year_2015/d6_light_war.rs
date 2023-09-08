use regex::Regex;
use rust_core::geometry::UCoord;

pub fn part_one(lines: &Vec<String>) -> String {
    let mut grid: Vec<bool> = vec![false; 1_000_000];

    lines.iter()
    .map(|l| parse_command(l))
    .for_each(|cmd| apply_one(&cmd, &mut grid));
    grid.into_iter().map(|v| if v { 1 } else { 0 })
    .sum::<u32>()
    .to_string()
}

pub fn part_two(lines: &Vec<String>) -> String {
    let mut grid: Vec<u32> = vec![0; 1_000_000];

    lines.iter()
    .map(|l| parse_command(l))
    .for_each(|cmd| apply_two(&cmd, &mut grid));

    grid.into_iter().sum::<u32>()
    .to_string()
}

enum CommandType {
    On,
    Off,
    Toggle
}

struct Command {
    bottom_left: UCoord,
    top_right: UCoord,
    command_type: CommandType
}

fn parse_command(line: &str) -> Command {
    let re = Regex::new(r"(toggle|turn on|turn off)\s(\d+,\d+)\sthrough\s(\d+,\d+)").unwrap();
    let Some(caps) = re.captures(line) else { panic!("Failed to match regex")};
    let bl = UCoord::from_str(&caps[2]);
    let tr = UCoord::from_str(&caps[3]);

    return match &caps[1] {
        "toggle" => Command{bottom_left: bl, top_right: tr, command_type: CommandType::Toggle},
        "turn on" => Command{bottom_left: bl, top_right: tr, command_type: CommandType::On},
        "turn off" => Command{bottom_left: bl, top_right: tr, command_type: CommandType::Off},
        _ => panic!("Unrecognized command type")
    };
}

fn apply_one(cmd: &Command, grid: &mut Vec<bool>) {
    for x in cmd.bottom_left.x .. cmd.top_right.x + 1 {
        for y in cmd.bottom_left.y .. cmd.top_right.y + 1 {
            match cmd.command_type {
                CommandType::On => grid[x + y * 1000usize] = true,
                CommandType::Off => grid[x + y * 1000usize] = false,
                _ => grid[x + y * 1000usize] = !grid[x+ y * 1000usize]
            }
        }
    }

}

fn apply_two(cmd: &Command, grid: &mut Vec<u32>) {
    for x in cmd.bottom_left.x .. cmd.top_right.x + 1 {
        for y in cmd.bottom_left.y .. cmd.top_right.y + 1 {
            let light = &mut grid[x + y * 1000usize];

            match cmd.command_type {
                CommandType::On => *light += 1,
                CommandType::Off => if *light > 0 { *light -= 1; },
                CommandType::Toggle => *light += 2
            }
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn parse_command_tests() {
        let c1 = parse_command("turn on 0,0 through 999,999");
        match c1.command_type {
            CommandType::On => (),
            _ => assert!(false)
        }
        assert!(c1.bottom_left == UCoord::new(0, 0));
        assert!(c1.top_right == UCoord::new(999, 999));

        let c2 = parse_command("toggle 0,0 through 999,0");
        match c2.command_type {
            CommandType::Toggle => (),
            _ => assert!(false)
        }
        assert!(c2.bottom_left == UCoord::new(0,0));
        assert!(c2.top_right == UCoord::new(999,0));

        let c3 = parse_command("turn off 499,499 through 500,500");
        match c3.command_type {
            CommandType::Off => (),
            _ => assert!(false)
        }
        assert!(c3.bottom_left == UCoord::new(499,499));
        assert!(c3.top_right == UCoord::new(500,500));
    }

    #[test]
    fn part_one_tests() {
        let lines = vec!["turn on 0,0 through 1,1", "toggle 1,1 through 2,2"].iter().map(|s| String::from(*s)).collect();
        assert_eq!(part_one(&lines), "6");
    }
}