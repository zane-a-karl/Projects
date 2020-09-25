fn main() {

    enum SpreadsheetCell {
        Int(i32),
        Float(f64),
        Text(String),
    }

    let row = vec![
        SpreadsheetCell::Int(3),
        SpreadsheetCell::Text(String::from("blue")),
        SpreadsheetCell::Float(10.12),
    ];

    for i in &row {
	match i {
	    SpreadsheetCell::Int(t) => println!("{}", t),
	    SpreadsheetCell::Text(s) => println!("{}", s),
	    SpreadsheetCell::Float(f) => println!("{}", f),
	}
    }

    let s = String::from("tic");
    let s1 = "tac".to_string();
    let s2 = format!("{}-{}-{}", s, s1, "toe");
    println!("{}", s2);

    let hello = "Здравствуйте";
    let answer = &hello[0..4];
    println!("{}", answer);

    use std::collections::HashMap;

    let mut scores = HashMap::new();
    scores.insert(String::from("Blue Team"), 10);
    scores.insert("Yellow Team".to_string(), 50);
    for (x, y) in scores {
	println!("{}: {}", x, y);
    }
}
