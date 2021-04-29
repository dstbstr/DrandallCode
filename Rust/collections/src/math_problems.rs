use std::collections::HashMap;
use Vec;

pub fn mean(numbers: &Vec<i32>) -> i32 {
    if numbers.is_empty() {
        return 0;
    }

    let mut total = 0;
    let mut count = 0;
    for number in numbers {
        total += number;
        count += 1;
    }

    total / count
}

pub fn median(numbers: &Vec<i32>) -> i32 {
    if numbers.is_empty() {
        return 0;
    }

    let mut numbers = numbers.clone();
    numbers.sort();
    let half_way = numbers.len() >> 1;
    numbers[half_way]
}

pub fn mode(numbers: &Vec<i32>) -> i32 {
    if numbers.is_empty() {
        return 0;
    }

    let mut map = HashMap::new();
    for number in numbers {
        let count = map.entry(number).or_insert(0);
        *count += 1;
    }

    let mut max_key = 0;
    let mut max_value = 0;
    for (key, value) in &map {
        if value > &max_value {
            max_key = **key;
            max_value = *value;
        }
    }

    return max_key;
}
