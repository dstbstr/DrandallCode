pub fn part_one(lines: &Vec<String>) -> String {
    let ings: Vec<Ing> = lines.iter().map(|l| Ing::parse(l)).collect();
    recurse(&ings, vec![0; 0].clone(), 0, false).to_string()
}

pub fn part_two(lines: &Vec<String>) -> String {
    let ings: Vec<Ing> = lines.iter().map(|l| Ing::parse(l)).collect();
    recurse(&ings, vec![0; 0].clone(), 0, true).to_string()
}

//Sugar: capacity 3, durability 0, flavor 0, texture -3, calories 2
struct Ing {
    capacity: i32,
    durability: i32,
    flavor: i32,
    texture: i32,
    calories: i32
}

impl Ing {
    fn parse(line: &str) -> Self {
        let parts: Vec<i32> = line.split(": ").last().unwrap().split(", ").map(|i| i.split(" ").last().unwrap().parse().unwrap()).collect();
        Self {
            capacity: parts[0],
            durability: parts[1],
            flavor: parts[2],
            texture: parts[3],
            calories: parts[4]
        }
    }
}


fn calc_score(ings: &Vec<(&Ing, &i32)>) -> i32 {
    let mut s = (0, 0, 0, 0);
    ings.iter().for_each(|p| {
        s.0 += p.0.capacity * p.1;
        s.1 += p.0.durability * p.1;
        s.2 += p.0.flavor * p.1;
        s.3 += p.0.texture * p.1;
    });

    if s.0 <= 0 || s.1 <= 0 || s.2 <= 0 || s.3 <= 0 {
        0
    } else {
        s.0 * s.1 * s.2 * s.3
    }
}

fn calc_calories(ings: &Vec<(&Ing, &i32)>) -> i32 {
    ings.iter().map(|p| p.0.calories * p.1).sum()
}

fn recurse(ings: &Vec<Ing>, mut amounts: Vec<i32>, total_so_far: i32, limit_calories: bool) -> i32 {
    if amounts.len() == ings.len() - 1 {
        amounts.push(100 - total_so_far);
        let pairs: Vec<(&Ing, &i32)> = ings.iter().zip(amounts.iter()).collect();
        if limit_calories {
            let calories = calc_calories(&pairs);
            if calories != 500 {
                return 0;
            }
        }
        return calc_score(&pairs);
    } else {
        amounts.push(1);
        let mut best: i32 = 0;
        for amount in 1..100 - total_so_far {
            amounts.pop();
            amounts.push(amount);
            best = std::cmp::max(best, recurse(ings, amounts.clone(), total_so_far + amount, limit_calories));
        }
        best
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn ing_parse_tests() {
        let ing = Ing::parse("Sugar: capacity 3, durability 0, flavor 0, texture -3, calories 2");
        assert_eq!(3, ing.capacity);
        assert_eq!(0, ing.durability);
        assert_eq!(0, ing.flavor);
        assert_eq!(-3, ing.texture);
        assert_eq!(2, ing.calories);
    }

    #[test]
    fn calc_score_tests() {
        let ings = vec![Ing::parse("Butterscotch: capacity -1, durability -2, flavor 6, texture 3, calories 8"),
        Ing::parse("Cinnamon: capacity 2, durability 3, flavor -2, texture -1, calories 3")];

        let pairs = ings.iter().zip([44, 56].iter()).collect();
        assert_eq!(62842880, calc_score(&pairs));
    }
}