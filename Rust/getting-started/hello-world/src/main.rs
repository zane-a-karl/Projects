fn main () {
    println!("Hello World!");

    let ex_tup: (i32, f64, u8) = (500, 8.4, 0b0000_0011);
    let (x, _y, _z) = ex_tup;
    println!("The ex_tuple looks like ({}, {}, {})", ex_tup.0, ex_tup.1, ex_tup.2);
    let x = if x==500 {41} else {14};
    println!("output of add1 is {}", add1(x));

    let x = "zane";
    println!("{}", x);


    let s1 = String::from("hello");
    let s2 = s1.clone();

    println!("{} and {}", s1, s2);
}

fn add1 (x: i32) -> i32 {
    x + 1
}
