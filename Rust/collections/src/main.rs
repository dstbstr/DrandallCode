mod math_problems;

fn main() {
    let numbers = vec![1, 2, 3, 3, 3, 4, 5, 6, 7, 7, 8, 9, 11];

    println!("Mean: {}", math_problems::mean(&numbers));
    println!("Median: {}", math_problems::median(&numbers));
    println!("Mode: {}", math_problems::mode(&numbers));
}
