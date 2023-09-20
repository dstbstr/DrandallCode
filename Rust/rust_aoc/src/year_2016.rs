use std::collections::HashMap;

pub mod d1_grid_search;
pub mod d2_bathroom_security;
pub mod d3_triangle_square;
pub mod d4_decoy_room;
pub mod d5_door_pass;
pub mod d6_repeat_signal;
pub mod d7_ipv7;
pub mod d8_tfa;
pub mod d9_decompress;
pub mod d10_balance_bots;
pub mod d11_generators;
pub mod d12_assembunny;
pub mod d13_cube_farm;
pub mod d14_one_time_pad;
pub mod d15_win_capsule;
pub mod d16_dragon_checksum;
pub mod d17_vault;
pub mod d18_traps;
pub mod d19_white_elephant;
pub mod d20_firewall;
pub mod d21_scrambler;
pub mod d22_grid_computing;
pub mod d23_safe_crack;
pub mod d24_air_ducts;
pub mod d25_clock_signal;

pub fn add_solutions(solutions: &mut HashMap<i32, HashMap<i32, HashMap<i32, fn(&Vec<String>) -> String>>>) {
    let parts = vec![
        d1_grid_search::part_one, d1_grid_search::part_two, 
        d2_bathroom_security::part_one, d2_bathroom_security::part_two,
        d3_triangle_square::part_one, d3_triangle_square::part_two, 
        d4_decoy_room::part_one, d4_decoy_room::part_two,
        d5_door_pass::part_one, d5_door_pass::part_two, 
        d6_repeat_signal::part_one, d6_repeat_signal::part_two, 
        d7_ipv7::part_one, d7_ipv7::part_two,
        d8_tfa::part_one, d8_tfa::part_two, 
        d9_decompress::part_one, d9_decompress::part_two,
        d10_balance_bots::part_one, d10_balance_bots::part_two, 
        d11_generators::part_one, d11_generators::part_two, 
        d12_assembunny::part_one, d12_assembunny::part_two,
        d13_cube_farm::part_one, d13_cube_farm::part_two, 
        d14_one_time_pad::part_one, d14_one_time_pad::part_two,
        d15_win_capsule::part_one, d15_win_capsule::part_two, 
        d16_dragon_checksum::part_one, d16_dragon_checksum::part_two,
        d17_vault::part_one, d17_vault::part_two, 
        d18_traps::part_one, d18_traps::part_two,
        d19_white_elephant::part_one, d19_white_elephant::part_two, 
        d20_firewall::part_one, d20_firewall::part_two,
        d21_scrambler::part_one, d21_scrambler::part_two, 
        d22_grid_computing::part_one, d22_grid_computing::part_two, 
        d23_safe_crack::part_one, d23_safe_crack::part_two,
        d24_air_ducts::part_one, d24_air_ducts::part_two, 
        d25_clock_signal::part_one];

    solutions.insert(2016, HashMap::new());

    for day in 1usize..=25 {
        let year = solutions.get_mut(&2016).unwrap();
        year.insert(day as i32, HashMap::new());
        let d = year.get_mut(&(day as i32)).unwrap();

        let index: usize = (day - 1) * 2;
        d.insert(1, parts[index]);
        if index + 1 < parts.len() {
            d.insert(2, parts[index + 1]);
        }
    }
}