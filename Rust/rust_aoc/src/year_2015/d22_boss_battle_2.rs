use std::fmt;

pub fn part_one(lines: &Vec<String>) -> String {
    let boss = Boss::parse(lines);
    let player = Player{hp: 50, mana: 500, armor: 0};
    let initial_state = State{player: player, boss: boss, cast_spells: Vec::new(), mana_spent: 0};

    find_best_mana(initial_state, false).to_string()
}

pub fn part_two(lines: &Vec<String>) -> String {
    let boss = Boss::parse(lines);
    let player = Player{hp: 50, mana: 500, armor: 0};
    let initial_state = State{player: player, boss: boss, cast_spells: Vec::new(), mana_spent: 0};

    find_best_mana(initial_state, true).to_string()
}

#[derive(Clone, Copy, Eq)]
struct Spell<'a> {
    name: &'a str,
    cost: i32,
    damage: i32,
    heal: i32,
    armor: i32,
    restore_mana: i32,
    duration: i32
}

impl<'a> std::cmp::PartialEq for Spell<'a> {
    fn eq(&self, other: &Spell) -> bool {
       self.name == other.name
    }
}
impl<'a> fmt::Debug for Spell<'a> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.name)
    }
}

impl<'a> Spell<'a> {
    fn new() -> Self {
        Spell { name: "", cost: 0, damage: 0, heal: 0, armor: 0, restore_mana: 0, duration: 0}
    }
}

#[derive(Clone)]
struct Player { hp: i32, mana: i32, armor: i32 }
#[derive(Clone)]
struct Boss { hp: i32, atk: i32 }

impl Boss { 
    fn parse(lines: &Vec<String>) -> Self { 
        let hp = lines[0].split(" ").last().unwrap().parse().unwrap();
        let atk = lines[1].split(" ").last().unwrap().parse().unwrap();
        Self{hp, atk} 
    } 
}
#[derive(Clone)]
struct State {
    player: Player,
    boss: Boss,
    cast_spells: Vec<Spell<'static>>,
    mana_spent: i32
}

const ALL_SPELLS: [&'static Spell; 5] = [
    &Spell{name: "MagicMissle", cost: 53, damage: 4, heal: 0, armor: 0, restore_mana: 0, duration: 1},
    &Spell{name: "Drain", cost: 73, damage: 2, heal: 2, armor: 0, restore_mana: 0, duration: 1},
    &Spell{name: "Shield", cost: 113, damage: 0, heal: 0, armor: 7, restore_mana: 0, duration: 6},
    &Spell{name: "Poison", cost: 173, damage: 3, heal: 0, armor: 0, restore_mana: 0, duration: 6},
    &Spell{name: "Recharge", cost: 229, damage: 0, heal: 0, armor: 0, restore_mana: 101, duration: 5}
    ];

fn tick_spells(state: &mut State) {
    let total = state.cast_spells.iter().fold(Spell::new(), |mut res, e| {
        res.damage += e.damage;
        res.heal += e.heal;
        res.armor += e.armor;
        res.restore_mana += e.restore_mana;
        res
    });
    state.player.hp += total.heal;
    state.player.armor = total.armor;
    state.player.mana += total.restore_mana;
    state.boss.hp -= total.damage;

    state.cast_spells = state.cast_spells.iter().map(|s| Spell{duration: s.duration - 1, ..*s}).filter(|s| s.duration > 0).collect();
}

fn check_match_over(state: &State, best: &mut i32) -> bool {
    if state.boss.hp <= 0 {
        if state.mana_spent < *best {
            *best = state.mana_spent;
        }
        return true;
    }
    if state.player.hp <= 0 {
        return true;
    }
    false
}

fn get_next_spells (state: State) -> Vec<Spell<'static>> {
    ALL_SPELLS.iter().filter(|s| !state.cast_spells.contains(s) && s.cost <= state.player.mana).map(|s| **s).collect()
}

fn find_best_mana(initial_state: State, hard_mode: bool) -> i32 {
    let mut q: Vec<State> = vec![initial_state];

    let mut best = 99_999;
    while !q.is_empty() {
        let mut current = q.pop().unwrap();
        if hard_mode {
            current.player.hp -= 1;
            if check_match_over(&current, &mut best) {
                continue;
            }
        }
        tick_spells(&mut current);

        if check_match_over(&current, &mut best) {
            continue;
        }
        
        for next_spell in get_next_spells(current.clone()).iter() {
            let mut next = current.clone();
            next.cast_spells.push(*next_spell);
            next.player.mana -= next_spell.cost;
            next.mana_spent += next_spell.cost;
            if next.mana_spent >= best {
                continue;
            }

            //Boss Turn
            tick_spells(&mut next);
            if check_match_over(&next, &mut best) {
                continue;
            }

            next.player.hp -= match next.boss.atk - next.player.armor {
                dmg if dmg < 1 => 1,
                dmg => dmg
            };
            if check_match_over(&next, &mut best) {
                continue;
            }

            q.push(next);
        }
    }

    best
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn example_one() {
        let player = Player{hp: 10, mana: 250, armor: 0};
        let boss = Boss{hp: 13, atk: 8};
        let initial_state = State{player: player, boss: boss, cast_spells: Vec::new(), mana_spent: 0};

        assert_eq!(226, find_best_mana(initial_state, false));
    }

    #[test]
    fn example_two() {
        let player = Player{hp: 10, mana: 250, armor: 0};
        let boss = Boss{hp: 14, atk: 8};
        let initial_state = State{player: player, boss: boss, cast_spells: Vec::new(), mana_spent: 0};

        assert_eq!(641, find_best_mana(initial_state, false));
    }

    //MagicMissle, Drain, Shield, Poison, Recharge
    //    0          1       2       3        4

    #[test]
    fn can_cast_tests() {
        let mut state = State{ player: Player{hp: 10, mana: 1000, armor: 0}, boss: Boss{hp: 14, atk: 8}, cast_spells: Vec::new(), mana_spent: 0 };

        let castable = get_next_spells(state.clone());
        dbg!(&castable);
        assert!(castable.contains(ALL_SPELLS[2]));

        state.cast_spells.push(*ALL_SPELLS[2]); //Shield
        tick_spells(&mut state); //5
        tick_spells(&mut state); //4

        let castable = get_next_spells(state.clone());
        dbg!(&castable);
        assert!(!castable.contains(ALL_SPELLS[2]));
        tick_spells(&mut state); //3
        tick_spells(&mut state); //2

        let castable = get_next_spells(state.clone());
        assert!(!castable.contains(ALL_SPELLS[2]));
        tick_spells(&mut state); //1
        tick_spells(&mut state); //0

        let castable = get_next_spells(state.clone());
        assert!(castable.contains(ALL_SPELLS[2]));
    }

    #[test]
    fn tick_spells_tests() {
        let mut state = State{ player: Player{hp: 10, mana: 0, armor: 0}, boss: Boss{hp: 14, atk: 8}, cast_spells: Vec::new(), mana_spent: 0 };

        // Turn 1
        state.cast_spells.push(*ALL_SPELLS[4]); //Recharge
        tick_spells(&mut state);
        assert_eq!(101, state.player.mana);

        tick_spells(&mut state);
        assert_eq!(202, state.player.mana);

        // Turn 2
        state.cast_spells.push(*ALL_SPELLS[2]); //Shield
        tick_spells(&mut state);
        assert_eq!(303, state.player.mana);
        assert_eq!(7, state.player.armor);

        tick_spells(&mut state);
        assert_eq!(404, state.player.mana);
        assert_eq!(7, state.player.armor);

        // Turn 3
        state.cast_spells.push(*ALL_SPELLS[1]); //Drain
        tick_spells(&mut state);
        assert_eq!(505, state.player.mana);
        assert_eq!(7, state.player.armor);
        assert_eq!(12, state.player.hp);

        tick_spells(&mut state);
        assert_eq!(505, state.player.mana);
        assert_eq!(7, state.player.armor);
        assert_eq!(12, state.player.hp);

        // Turn 4
        state.cast_spells.push(*ALL_SPELLS[3]); //Poison
        tick_spells(&mut state);
        assert_eq!(7, state.player.armor);

        tick_spells(&mut state);
        assert_eq!(7, state.player.armor);

        // Turn 5
        state.cast_spells.push(*ALL_SPELLS[0]); //Magic Missle
        tick_spells(&mut state);
        assert_eq!(0, state.player.armor);
    }
}
