pub fn split_input_into_groups(lines: &Vec<String>) -> Vec<Vec<String>> {
    let mut result: Vec<Vec<String>> = Vec::new();
    let mut group: Vec<String> = Vec::new();

    for line in lines {
        if line.is_empty() {
            result.push(group.clone());
            group.clear();
        } else {
            group.push(line.clone());
        }
    }

    result.push(group.clone());
    result
}