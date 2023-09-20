use std::collections::HashMap;

pub mod d1_santa_elevator;
pub mod d2_elf_wrapping_paper;
pub mod d3_at_least_one_present;
pub mod d4_advent_coins;
pub mod d5_nice_string;
pub mod d6_light_war;
pub mod d7_circuit;
pub mod d8_escaped_strings;
pub mod d9_single_night;
pub mod d10_see_and_say;
pub mod d11_next_pass;
pub mod d12_js_abacus;
pub mod d13_knights_of_dinner;
pub mod d14_reindeer_races;
pub mod d15_perfect_cookie;
pub mod d16_aunt_sue;
pub mod d17_eggnog_storage;
pub mod d18_light_war_2;
pub mod d19_rudolf_reactor;
pub mod d20_elf_delivery;
pub mod d21_boss_battle;
pub mod d22_boss_battle_2;
pub mod d23_turing_lock;
pub mod d24_sled_balance;
pub mod d25_secret_code;

pub fn add_solutions(solutions: &mut HashMap<i32, HashMap<i32, HashMap<i32, fn(&Vec<String>) -> String>>>) {
    solutions.insert(2015, HashMap::new());
    //let year = solutions.get_mut(&2015).unwrap();
    let parts = vec![d1_santa_elevator::part_one, d1_santa_elevator::part_two, d2_elf_wrapping_paper::part_one, d2_elf_wrapping_paper::part_two,
    d3_at_least_one_present::part_one, d3_at_least_one_present::part_two, d4_advent_coins::part_one, d4_advent_coins::part_two,
    d5_nice_string::part_one, d5_nice_string::part_two, d6_light_war::part_one, d6_light_war::part_two, d7_circuit::part_one, d7_circuit::part_two,
    d8_escaped_strings::part_one, d8_escaped_strings::part_two, d9_single_night::part_one, d9_single_night::part_two,
    d10_see_and_say::part_one, d10_see_and_say::part_two, d11_next_pass::part_one, d11_next_pass::part_two, d12_js_abacus::part_one, d12_js_abacus::part_two,
    d13_knights_of_dinner::part_one, d13_knights_of_dinner::part_two, d14_reindeer_races::part_one, d14_reindeer_races::part_two,
    d15_perfect_cookie::part_one, d15_perfect_cookie::part_two, d16_aunt_sue::part_one, d16_aunt_sue::part_two,
    d17_eggnog_storage::part_one, d17_eggnog_storage::part_two, d18_light_war_2::part_one, d18_light_war_2::part_two,
    d19_rudolf_reactor::part_one, d19_rudolf_reactor::part_two, d20_elf_delivery::part_one, d20_elf_delivery::part_two,
    d21_boss_battle::part_one, d21_boss_battle::part_two, d22_boss_battle_2::part_one, d22_boss_battle_2::part_two, d23_turing_lock::part_one, d23_turing_lock::part_two,
    d24_sled_balance::part_one, d24_sled_balance::part_two, d25_secret_code::part_one];
    for day in 1usize..=25 {
        let year = solutions.get_mut(&2015).unwrap();
        year.insert(day as i32, HashMap::new());
        let d = year.get_mut(&(day as i32)).unwrap();

        let index: usize = (day - 1) * 2;
        d.insert(1, parts[index]);
        if index + 1 < parts.len() {
            d.insert(2, parts[index + 1]);
        }
    }
}