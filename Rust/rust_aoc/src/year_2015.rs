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
    let year = solutions.get_mut(&2015).unwrap();

    year.insert(1, HashMap::new());
    year.get_mut(&1).unwrap().insert(1, d1_santa_elevator::part_one);
    year.get_mut(&1).unwrap().insert(2, d1_santa_elevator::part_two);

    year.insert(2, HashMap::new());
    year.get_mut(&2).unwrap().insert(1, d2_elf_wrapping_paper::part_one);
    year.get_mut(&2).unwrap().insert(2, d2_elf_wrapping_paper::part_two);

    year.insert(3, HashMap::new());
    year.get_mut(&3).unwrap().insert(1, d3_at_least_one_present::part_one);
    year.get_mut(&3).unwrap().insert(2, d3_at_least_one_present::part_two);

    year.insert(4, HashMap::new());
    year.get_mut(&4).unwrap().insert(1, d4_advent_coins::part_one);
    year.get_mut(&4).unwrap().insert(2, d4_advent_coins::part_two);

    year.insert(5, HashMap::new());
    year.get_mut(&5).unwrap().insert(1, d5_nice_string::part_one);
    year.get_mut(&5).unwrap().insert(2, d5_nice_string::part_two);

    year.insert(6, HashMap::new());
    year.get_mut(&6).unwrap().insert(1, d6_light_war::part_one);
    year.get_mut(&6).unwrap().insert(2, d6_light_war::part_two);

    year.insert(7, HashMap::new());
    year.get_mut(&7).unwrap().insert(1, d7_circuit::part_one);
    year.get_mut(&7).unwrap().insert(2, d7_circuit::part_two);

    year.insert(8, HashMap::new());
    year.get_mut(&8).unwrap().insert(1, d8_escaped_strings::part_one);
    year.get_mut(&8).unwrap().insert(2, d8_escaped_strings::part_two);

    year.insert(9, HashMap::new());
    year.get_mut(&9).unwrap().insert(1, d9_single_night::part_one);
    year.get_mut(&9).unwrap().insert(2, d9_single_night::part_two);

    year.insert(10, HashMap::new());
    year.get_mut(&10).unwrap().insert(1, d10_see_and_say::part_one);
    year.get_mut(&10).unwrap().insert(2, d10_see_and_say::part_two);

    year.insert(11, HashMap::new());
    year.get_mut(&11).unwrap().insert(1, d11_next_pass::part_one);
    year.get_mut(&11).unwrap().insert(2, d11_next_pass::part_two);

    year.insert(12, HashMap::new());
    year.get_mut(&12).unwrap().insert(1, d12_js_abacus::part_one);
    year.get_mut(&12).unwrap().insert(2, d12_js_abacus::part_two);

    year.insert(13, HashMap::new());
    year.get_mut(&13).unwrap().insert(1, d13_knights_of_dinner::part_one);
    year.get_mut(&13).unwrap().insert(2, d13_knights_of_dinner::part_two);

    year.insert(14, HashMap::new());
    year.get_mut(&14).unwrap().insert(1, d14_reindeer_races::part_one);
    year.get_mut(&14).unwrap().insert(2, d14_reindeer_races::part_two);

    year.insert(15, HashMap::new());
    year.get_mut(&15).unwrap().insert(1, d15_perfect_cookie::part_one);
    year.get_mut(&15).unwrap().insert(2, d15_perfect_cookie::part_two);

    year.insert(16, HashMap::new());
    year.get_mut(&16).unwrap().insert(1, d16_aunt_sue::part_one);
    year.get_mut(&16).unwrap().insert(2, d16_aunt_sue::part_two);

    year.insert(17, HashMap::new());
    year.get_mut(&17).unwrap().insert(1, d17_eggnog_storage::part_one);
    year.get_mut(&17).unwrap().insert(2, d17_eggnog_storage::part_two);

    year.insert(18, HashMap::new());
    year.get_mut(&18).unwrap().insert(1, d18_light_war_2::part_one);
    year.get_mut(&18).unwrap().insert(2, d18_light_war_2::part_two);

    year.insert(19, HashMap::new());
    year.get_mut(&19).unwrap().insert(1, d19_rudolf_reactor::part_one);
    year.get_mut(&19).unwrap().insert(2, d19_rudolf_reactor::part_two);

    year.insert(20, HashMap::new());
    year.get_mut(&20).unwrap().insert(1, d20_elf_delivery::part_one);
    year.get_mut(&20).unwrap().insert(2, d20_elf_delivery::part_two);

    year.insert(21, HashMap::new());
    year.get_mut(&21).unwrap().insert(1, d21_boss_battle::part_one);
    year.get_mut(&21).unwrap().insert(2, d21_boss_battle::part_two);

    year.insert(22, HashMap::new());
    year.get_mut(&22).unwrap().insert(1, d22_boss_battle_2::part_one);
    year.get_mut(&22).unwrap().insert(2, d22_boss_battle_2::part_two);

    year.insert(23, HashMap::new());
    year.get_mut(&23).unwrap().insert(1, d23_turing_lock::part_one);
    year.get_mut(&23).unwrap().insert(2, d23_turing_lock::part_two);

    year.insert(24, HashMap::new());
    year.get_mut(&24).unwrap().insert(1, d24_sled_balance::part_one);
    year.get_mut(&24).unwrap().insert(2, d24_sled_balance::part_two);

    year.insert(25, HashMap::new());
    year.get_mut(&25).unwrap().insert(1, d25_secret_code::part_one);
}