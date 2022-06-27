///  Here’s how it works: the program will generate a random
/// integer between 1 and 100. It will then prompt the player
/// to enter a guess. After a guess is entered, the program
/// will indicate whether the guess is too low or too high.
/// If the guess is correct, the game will print a
/// congratulatory message and exit.
use rand::prelude::*;
use std::io::stdin;

fn main() {
    let mut thread_rng = ThreadRng::default();
    let mut rn: i32 = thread_rng.gen::<i32>() % 99 + 1;
    println!("{rn}");
    print!("Please enter a guess: ");
    let mut guess_buf = String::new();
    if let Ok(n) = stdin().read_line(&mut guess_buf) {
        print!("{guess_buf}");
        println!("read {n} bytes");
        println!(
            "you were off by {}!",
            rn - guess_buf.trim().parse::<i32>().unwrap()
        );
        println!("{rn}");
    }
}
