pub fn part_one(lines: &Vec<String>) -> String {
    let containers: Vec<i32> = lines.iter().map(|l| l.parse::<i32>().unwrap()).collect();
    count_combinations_initial(containers, 150).to_string()
}

pub fn part_two(lines: &Vec<String>) -> String {
    let containers: Vec<i32> = lines.iter().map(|l| l.parse::<i32>().unwrap()).collect();
    count_small_containers(containers, 150).to_string()
}

fn count_combinations(containers: &Vec<i32>, index: usize, remaining: i32) -> i32 {
    match remaining - containers[index] {
        r if r < 0 => 0,
        r if r == 0 => 1,
        r => (index + 1..containers.len()).map(|i| count_combinations(containers, i, r)).sum()
    }
}

fn count_combinations_initial(mut containers: Vec<i32>, initial: i32) -> i32 {
    containers.push(0);
    containers.sort();
    count_combinations(&containers, 0, initial)
}

fn get_min_size(containers: &Vec<i32>, index: usize, remaining: i32, current_length: i32) -> i32 {
    match remaining - containers[index] {
        r if r < 0 => 0,
        r if r == 0 => current_length + 1,
        r => (index + 1..containers.len()).map(|i| get_min_size(containers, i, r, current_length + 1)).filter(|v| *v > 0).min().unwrap_or(0)
    }
}

fn count_combos_with_max_containers(containers: &Vec<i32>, index: usize, remaining: i32, current_length: i32, max_size: i32) -> i32 {
    if current_length >= max_size {
        return 0;
    }
    match remaining - containers[index] {
        r if r < 0 => 0,
        r if r == 0 => 1,
        r => (index + 1..containers.len()).map(|i| count_combos_with_max_containers(containers, i, r, current_length + 1, max_size)).sum()
    }
}

fn count_small_containers(mut containers: Vec<i32>, initial: i32) -> i32 {
    containers.push(0);
    containers.sort();
    let min_size = get_min_size(&containers, 0, initial, 0);
    count_combos_with_max_containers(&containers, 0, initial, 0, min_size)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn count_combinations_test() {
        let mut containers = vec![20, 5, 10, 5, 15];
        containers.sort();

        assert_eq!(4, count_combinations_initial(containers, 25));
    }

    #[test]
    fn get_min_size_test() {
        let mut containers = vec![20, 5, 10, 5, 15];
        containers.sort();

        assert_eq!(2, get_min_size(&containers, 0, 25, 0));
    }

    #[test]
    fn count_small_containers_tests() {
        let mut containers = vec![20, 5, 10, 5, 15];
        containers.sort();

        assert_eq!(3, count_small_containers(containers, 25));
    }
}