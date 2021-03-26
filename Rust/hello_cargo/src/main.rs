/*
use rand::Rng;
use std::cmp::Ordering;
use std::io;

fn main() {
    println!("Guess the number");

    let secret_number = rand::thread_rng().gen_range(1, 101);
    loop {
        println!("Please input your guess.");

        let mut guess = String::new();
        io::stdin()
            .read_line(&mut guess)
            .expect("Failed to read line");

        let guess: u32 = match guess.trim().parse() {
            Ok(num) => num,
            Err(_) => continue,
        };
        println!("You guessed: {}", guess);

        match guess.cmp(&secret_number) {
            Ordering::Less => println!("Too small!"),
            Ordering::Greater => println!("Too big!"),
            Ordering::Equal => {
                println!("You win!");
                break;
            }
        }
    }
}
*/

/*
#[derive(Debug)]
struct Rect {
    width: u32,
    height: u32,
}

impl Rect {
    fn area(&self) -> u32 {
        self.width * self.height
    }

    fn can_hold(&self, other: &Rect) -> bool {
        self.width > other.width && self.height > other.height
    }

    fn square(size: u32) -> Rect {
        Rect {
            height: size,
            width: size,
        }
    }
}

fn main() {
    let rect1 = Rect {
        width: 30,
        height: 50,
    };

    let rect2 = Rect::square(10);

    println!(
        "The area of the rectangle is {} square pixels",
        rect1.area()
    );

    println!("Rect1 {:?}", rect1);
    println!("Rect2 {:#?}", rect2);

    println!("Rect 1 fits in rect 2? {}", rect2.can_hold(&rect1));
}
*/

/*
#[derive(Debug)]
enum State {
    Washington,
    Indiana,
    California,
}

enum Coin {
    Penny,
    Nickel,
    Dime,
    Quarter(State),
}

fn main() {
    let penny = Coin::Penny;
    let quarter = Coin::Quarter(State::Washington);

    println!("Penny is worth {}", value_of_coin(&penny));
    println!("Quarter is worth {}", value_of_coin(&quarter));
}

fn value_of_coin(coin: &Coin) -> u8 {
    match coin {
        Coin::Penny => 1,
        Coin::Nickel => 5,
        Coin::Dime => 10,
        Coin::Quarter(state) => {
            println!("State quarter from {:?}!", state);
            25
        }
    }
}
*/

fn main() {
    println!("Hello world");
}
