// Convert strings to pig latin. The first consonant of each word is moved to the end of the word and “ay” is added, so “first” becomes “irst-fay.” Words that start with a vowel have “hay” added to the end instead (“apple” becomes “apple-hay”). Keep in mind the details about UTF-8 encoding!

use std::io;

fn to_pig_latin (s: &str) -> String {

    let mut pig_latin = String::new();
    for word in s.split_whitespace() {
	pig_latin.push_str(
	    &format!("{}{} ",
		    &word[1..],
		    match &word[0..1] {
			"a" => "-hay".to_string(),
			"e" => "-hay".to_string(),
			"i" => "-hay".to_string(),
			"o" => "-hay".to_string(),
			"u" => "-hay".to_string(),
			c  => format!("{}{}{}", "-", c, "ay"),
		    }
	    )
	);
    }
    pig_latin
}

fn main() {

    println!("Please enter the message you'd like translated to Pig latin.");
    let mut msg = String::new();
    io::stdin()
	.read_line(&mut msg)
	.expect("Failed to read msg");
    println!("{}", to_pig_latin(&msg) );
}
