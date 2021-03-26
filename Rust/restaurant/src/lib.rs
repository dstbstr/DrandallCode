mod back_of_house;
mod front_of_house;

use crate::front_of_house::hosting;

pub fn eat_at_restaurant() {
    let mut meal = back_of_house::Breakfast::summer("Rye");
    meal.toast = String::from("Wheat");

    let app1 = back_of_house::Appetizer::Salad;

    crate::front_of_house::hosting::add_to_waitlist();

    front_of_house::hosting::add_to_waitlist();

    hosting::add_to_waitlist();
}
