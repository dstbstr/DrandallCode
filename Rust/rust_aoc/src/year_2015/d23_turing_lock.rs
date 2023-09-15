pub fn part_one(lines: &Vec<String>) -> String {
    solve(lines, 0).to_string()
}

pub fn part_two(lines: &Vec<String>) -> String {
    solve(lines, 1).to_string()
}

fn solve(lines: &Vec<String>, initial_a: usize) -> usize {
    let instructions: Vec<_> = lines.iter().map(|s| generate_func(s)).collect();
    let mut a = initial_a;
    let mut b = 0usize;
    let mut ip = 0usize;

    while ip < instructions.len() {
        instructions[ip](&mut ip, &mut a, &mut b);
    }

    b
}

#[derive(Copy, Clone)]
enum Cmd {Hlf, Tpl, Inc, Jmp, Jie, Jio}

fn generate_func(line: &str) -> impl Fn(&mut usize, &mut usize, &mut usize) {
    let s: Vec<String> = line.split(" ").map(|s| String::from(s)).collect();
    let is_reg_a = s[1].as_bytes()[0] == b'a';
    let cmd = match s[0].as_str() {
        "hlf" => Cmd::Hlf,
        "tpl" => Cmd::Tpl,
        "inc" => Cmd::Inc,
        "jmp" => Cmd::Jmp,
        "jie" => Cmd::Jie,
        "jio" => Cmd::Jio,
        _ => panic!("Unrecognized instruction")
    };

    move |ip: &mut usize, a: &mut usize, b: &mut usize| {
        match (cmd, is_reg_a) {
            (Cmd::Hlf, true) => *a /= 2,
            (Cmd::Hlf, false) => *b /= 2,
            (Cmd::Tpl, true) => *a *= 3,
            (Cmd::Tpl, false) => *b *= 3,
            (Cmd::Inc, true) => *a += 1,
            (Cmd::Inc, false) => *b += 1,
            (Cmd::Jie, true) => { if *a % 2 == 0 { *ip = (*ip as i32 + s[2].parse::<i32>().unwrap() - 1) as usize }},
            (Cmd::Jie, false) => { if *b % 2 == 0 { *ip = (*ip as i32 + s[2].parse::<i32>().unwrap() - 1) as usize }},
            (Cmd::Jio, true) => { if *a == 1 { *ip = (*ip as i32 + s[2].parse::<i32>().unwrap() - 1) as usize }},
            (Cmd::Jio, false) => { if *b == 1{ *ip = (*ip as i32 + s[2].parse::<i32>().unwrap() - 1) as usize }}
            (Cmd::Jmp, _) => *ip = (*ip as i32 + s[1].parse::<i32>().unwrap() - 1) as usize,
        };
        *ip += 1;
    }
}