use std::collections::HashMap;

pub fn part_one(lines: &Vec<String>) -> String {
    let mut index = 0usize;
    count_all(&parse_json(&lines[0], &mut index), "").to_string()
}

pub fn part_two(lines: &Vec<String>) -> String {
    let mut index = 0usize;
    count_all(&parse_json(&lines[0], &mut index), "red").to_string()
}

struct Json {
    elements: Vec<Json>,
    objects: HashMap<String, Json>,
    string_value: String,
    numeric_value: isize,
    is_string: bool,
    is_number: bool
}

impl Json {
    fn new() -> Self {
        Self {
            elements: Vec::new(), 
            objects: HashMap::new(), 
            string_value: String::new(), 
            numeric_value: 0, 
            is_string: false, 
            is_number: false
        }
    }
    fn from_number(num: isize) -> Self {
        Self {
            elements: Vec::new(),
            objects: HashMap::new(),
            string_value: String::new(),
            numeric_value: num,
            is_string: false,
            is_number: true
        }
    }
    fn from_str(val: &str) -> Self {
        Self {
            elements: Vec::new(),
            objects: HashMap::new(),
            string_value: String::from(val),
            numeric_value: 0,
            is_string: true,
            is_number: false
        }
    }
}

fn parse_json(json: &str, index: &mut usize) -> Json {
    let chars = json.as_bytes();
    let mut result: Json;

    if chars[*index] == b'[' { //parse array
        *index += 1; //consume open brace
        result = Json::new();
        while chars[*index] != b']' {
            result.elements.push(parse_json(json, index));
        }

        *index += 1; //consume close brace
    } else if chars[*index] == b'{' { // parse object
        *index += 1; //consume open curly
        result = Json::new();
        while chars[*index] != b'}' {
            let key = parse_json(json, index).string_value;
            *index += 1;
            let value = parse_json(json, index);
            result.objects.insert(key, value);
        }
        *index += 1; //consume close curly
    } else if chars[*index] == b'"' { //parse string
        
        let close_quote = json[*index + 1..].find("\"").unwrap() + 1;
        result = Json::from_str(std::str::from_utf8(&chars[*index + 1..close_quote + *index]).unwrap());
        *index += close_quote + 1;
    } else { //parse number
        let substr = &json[*index..];
        let closes = [substr.find(","), substr.find("]"), substr.find("}")];
        let close = closes.iter().filter_map(|f| *f).min().unwrap();

        result = Json::from_number(std::str::from_utf8(&chars[*index..close + *index]).unwrap().parse().unwrap());
        *index += close;
    }

    if *index < chars.len() && chars[*index] == b',' {
        *index += 1;
    }
    result
}

fn count_all(json: &Json, exclude: &str) -> isize {
    if json.is_number {
        json.numeric_value
    }
    else if json.is_string {
        0
    }
    else if !json.elements.is_empty() {
        json.elements.iter().map(|j| count_all(j, exclude)).sum()
    } else if !json.objects.is_empty() {
        if json.objects.iter().any(|kvp| kvp.1.is_string && kvp.1.string_value == exclude) {
            0
        } else {
            json.objects.iter().map(|kvp| count_all(kvp.1, exclude)).sum()
        }
    } else {
        0
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn parse_numeric_array() {
        let mut index = 0usize;
        let root = parse_json("[1,2,3]", &mut index);
        assert_eq!(3, root.elements.len());
        assert!(root.elements[0].is_number);
        assert_eq!(1, root.elements[0].numeric_value);
    }

    #[test]
    fn parse_nested_array() {
        let mut index = 0usize;
        let root = parse_json("[[[3]]]", &mut index);
        assert_eq!(1, root.elements.len());
        assert_eq!(3, root.elements[0].elements[0].elements[0].numeric_value);
    }

    #[test]
    fn parse_string_array() {
        let mut index = 0usize;
        let root = parse_json(r#"["abc","def","ghi"]"#, &mut index);
        assert_eq!(3, root.elements.len());
        assert!(root.elements[0].is_string);
        assert_eq!("abc", root.elements[0].string_value);
    }

    #[test]
    fn parse_array_of_arrays() {
        let mut index = 0usize;
        let root = parse_json(r#"[["abc","def"],[123,345]]"#, &mut index);
        assert_eq!(2, root.elements.len());
        assert_eq!(2, root.elements[0].elements.len());
        assert_eq!("abc", root.elements[0].elements[0].string_value);
        assert_eq!(2, root.elements[1].elements.len());
        assert_eq!(123, root.elements[1].elements[0].numeric_value);
    }

    #[test]
    fn parse_objects() {
        let mut index = 0usize;
        let root = parse_json(r#"{"a":2,"b":4}"#, &mut index);
        assert_eq!(2, root.objects.len());
        assert_eq!(2, root.objects.get("a").unwrap().numeric_value);
        assert_eq!(4, root.objects.get("b").unwrap().numeric_value);
    }

    #[test]
    fn parse_nested_objects() {
        let mut index = 0usize;
        let root = parse_json(r#"{"a":{"b":4},"c":-1}"#, &mut index);
        assert_eq!(2, root.objects.len());
        assert_eq!(1, root.objects.get("a").unwrap().objects.len());
        assert_eq!(-1, root.objects.get("c").unwrap().numeric_value);
    }

    #[test]
    fn parse_empty_array() {
        let mut index = 0usize;
        let root = parse_json("[]", &mut index);
        assert!(!root.is_number);
        assert!(!root.is_string);
        assert!(root.elements.is_empty());
        assert!(root.objects.is_empty());
    }

    #[test]
    fn parse_empty_object() {
        let mut index = 0usize;
        let root = parse_json("{}", &mut index);
        assert!(!root.is_number);
        assert!(!root.is_string);
        assert!(root.elements.is_empty());
        assert!(root.objects.is_empty());
    }

    #[test]
    fn count_all_without_exclude() {
        let mut index = 0usize;
        let root = parse_json(r#"[1,{"c":"red","b":2},3]"#, &mut index);
        assert_eq!(6, count_all(&root, ""));
    }

    #[test]
    fn count_all_with_exclude() {
        let mut index = 0usize;
        let root = parse_json(r#"[1,{"c":"red","b":2},3]"#, &mut index);
        assert_eq!(4, count_all(&root, "red"));
    }
}