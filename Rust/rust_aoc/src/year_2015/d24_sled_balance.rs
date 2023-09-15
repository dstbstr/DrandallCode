use crate::input_utils::parse_lines_as_numbers;

pub fn part_one(lines: &Vec<String>) -> String {
    //let weights = lines.iter().map(|s| s.parse::<usize>().unwrap()).collect::<Vec<usize>>();
    let weights = parse_lines_as_numbers::<usize>(&lines);
    solve(&weights, 3).to_string()
}

pub fn part_two(lines: &Vec<String>) -> String {
    //let weights = lines.iter().map(|s| s.parse::<usize>().unwrap()).collect::<Vec<usize>>();
    let weights = parse_lines_as_numbers::<usize>(&lines);
    solve(&weights, 4).to_string()
}

fn solve(weights: &Vec<usize>, spots: usize) -> usize {
    let mut result: Vec<usize> = Vec::new();
    let balance = find_balance_weight(&weights, spots);
    recurse(weights, 0, balance, &mut result, balance);

    find_entanglement(&result)
}

fn find_balance_weight(weights: &Vec<usize>, spots: usize) -> usize {
    weights.iter().sum::<usize>() / spots
}

fn find_entanglement(weights: &Vec<usize>) -> usize {
    weights.iter().product::<usize>()
}

fn recurse(weights: &Vec<usize>, index: usize, remaining_weight: usize, out_result: &mut Vec<usize>, initial_weight: usize) -> bool {
    if remaining_weight == 0 {
        return true;
    }
    if out_result.len() > 6 {
        return false;
    }

    let mut min_items = 999usize;
    let mut min_entanglement = 99_999usize;
    let mut found = false;
    let mut result: Vec<usize> = Vec::new();

    for i in index..weights.len() {
        let mut temp = out_result.clone();
        let weight = weights[i];
        if weight > remaining_weight {
            continue;
        }
        temp.push(weight);
        if recurse(weights, i + 1, remaining_weight - weight, &mut temp, initial_weight) {
            found = true;
            let entanglement = find_entanglement(&temp);
            if temp.len() < min_items {
                min_items = temp.len();
                min_entanglement = entanglement;
                result = temp.clone();
            } else if temp.len() == min_items && entanglement < min_entanglement {
                min_entanglement = entanglement;
                result = temp.clone();
            }
        }
    }

    if found {
        *out_result = result.clone();
    }

    found
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn find_balance_weight_tests() {
        let weights: Vec<usize> = vec![1, 2, 3, 4, 5, 7, 8, 9, 10, 11];
        assert_eq!(20, find_balance_weight(&weights, 3));
        assert_eq!(15, find_balance_weight(&weights, 4));
    }

    #[test]
    fn find_entanglement_tests() {
        assert_eq!(99, find_entanglement(&vec![11, 9]));
        assert_eq!(90, find_entanglement(&vec![10, 9, 1]));
    }
}