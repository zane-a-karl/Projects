use std::io;

fn main() {

    println!("Please Enter which fibonacci # you'd like:");
    let mut choice = String::new();
    io::stdin()
	.read_line(&mut choice)
	.expect("Failed to read line");
    let choice: i32 = match choice.trim().parse() {
	Ok(num) => num,
	Err(_) => {
	    println!("The input must be a number");
	    -1
	}
    };
    println!("The {}th fibonacci # is {}", choice, get_nth_fibonacci(choice));
}

fn get_nth_fibonacci (n: i32) -> i32 {
    if n == 0 {
	return 0;
    } else if n == 1 {
	return 1;
    } else {
	return get_nth_fibonacci(n-1) + get_nth_fibonacci(n-2);
    }
}
