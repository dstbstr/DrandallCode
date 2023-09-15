use itertools::iproduct;

pub fn part_one(lines: &Vec<String>) -> String {
    let boss = parse_boss(&lines);
    get_builds().iter()
    .filter(|b| count_rounds(b, &boss) <= count_rounds(&boss, b))
    .map(|b| b.total_cost)
    .min().unwrap_or(0).to_string()
}

pub fn part_two(lines: &Vec<String>) -> String {
    let boss = parse_boss(&lines);
    get_builds().iter()
    .filter(|b| count_rounds(&boss, b) < count_rounds(b, &boss))
    .map(|b| b.total_cost)
    .max().unwrap_or(0).to_string()
}

#[derive(Eq, PartialEq, Debug)]
struct Equipment { cost: i32, attack: i32, defense: i32 }
impl Equipment {
    fn new(c: i32, a: i32, d: i32) -> Self {
        Self{ cost: c, attack: a, defense: d}
    }
}

#[derive(Debug)]
struct Player { hp: i32, attack: i32, defense: i32, total_cost: i32 }
impl Player {
    fn new(h: i32, a: i32, d: i32) -> Self {
        Self { hp: h, attack: a, defense: d, total_cost: 0}
    }
    fn from_equipment(equipment: &Vec<&Equipment>) -> Self {
        let total = equipment.iter().fold(Equipment::new(0, 0, 0), |mut sum, e| {
            sum.cost += e.cost;
            sum.attack += e.attack;
            sum.defense += e.defense;
            sum
        });
        Self {hp: 100, attack: total.attack, defense: total.defense, total_cost: total.cost }
    }
}

fn count_rounds(attacker: &Player, defender: &Player) -> i32 {
    let dmg = match attacker.attack - defender.defense {
        r if r <= 0 => 1,
        r => r
    };

    match defender.hp % dmg {
        0 => defender.hp / dmg,
        _ => defender.hp / dmg + 1
    }
}

fn get_builds() -> Vec<Player> {
    let weapons: Vec<Equipment> = [(8, 4, 0), (10, 5, 0), (25, 6, 0), (40, 7, 0), (74, 8, 0)].iter().map(|v| Equipment::new(v.0, v.1, v.2)).collect();
    let armors: Vec<Equipment> = [(0, 0, 0), (13, 0, 1), (31, 0, 2), (53, 0, 3), (75, 0, 4), (102, 0, 5)].iter().map(|v| Equipment::new(v.0, v.1, v.2)).collect();
    let rings: Vec<Equipment> = [(0, 0, 0), (0, 0, 0), (25, 1, 0), (50, 2, 0), (100, 3, 0), (20, 0, 1), (40, 0, 2), (80, 0, 3)].iter().map(|v| Equipment::new(v.0, v.1, v.2)).collect();

    iproduct!(&weapons, &armors, &rings, &rings)
    .filter(|(_, _, r1, r2) | r1 != r2)
    .map(|(w, a, r1, r2)| Player::from_equipment(&vec![&w, &a, &r1, &r2]))
    .collect()
}

/*
Hit Points: 104
Damage: 8
Armor: 1
*/

fn parse_boss(lines: &Vec<String>) -> Player {
    Player::new(
        lines[0].split(" ").last().unwrap().parse().unwrap(), 
        lines[1].split(" ").last().unwrap().parse().unwrap(),
        lines[2].split(" ").last().unwrap().parse().unwrap()
    )
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn count_rounds_test() {
        let p1 = Player::new(8, 5, 5);
        let p2 = Player::new(12, 7, 2);

        assert_eq!(4, count_rounds(&p1, &p2));
        assert_eq!(4, count_rounds(&p2, &p1));
    }
}