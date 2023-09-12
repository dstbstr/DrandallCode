pub fn part_one(lines: &Vec<String>) -> String {
    lines.iter().map(|l| Reindeer::parse(l)).map(|r| find_distance(&r, 2503)).max().unwrap().to_string()
}

pub fn part_two(lines: &Vec<String>) -> String {
    let racers = lines.iter().map(|l| Reindeer::parse(l)).collect();
    get_points(&racers, 2503).to_string()
}

struct Reindeer {
    speed: usize,
    run_time: usize,
    rest_time: usize
}

impl Reindeer {
    fn parse(line: &str) -> Self {
        let s: Vec<&str> = line.split(" ").collect();
        Self {
            speed: s[3].parse().unwrap(),
            run_time: s[6].parse().unwrap(),
            rest_time: s[13].parse().unwrap()
        }
    }
}

fn find_distance(who: &Reindeer, seconds: usize) -> usize {
    let cycle_time = who.run_time + who.rest_time;
    let cycles = seconds / cycle_time;
    let remainder = seconds % cycle_time;
    let mut distance = who.speed * cycles * who.run_time;

    if remainder <= who.run_time {
        distance += who.speed * remainder;
    } else {
        distance += who.speed * who.run_time;
    }

    distance
}

fn get_points(racers: &Vec<Reindeer>, seconds: usize) -> usize {
    let mut bests = vec![0; seconds + 1];
    for second in 1..=seconds {
        bests[second] = racers.iter().map(|r| find_distance(r, second)).max().unwrap();
    }

    racers.iter().map(|r| (1usize..=seconds)
        .map(|second| {
            if find_distance(r, second) == bests[second] {
                1usize
            } else {
                0usize
            }
        }
    ).sum::<usize>()
    ).max().unwrap()
}