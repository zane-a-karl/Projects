use std::io;

fn main() {
    println!("Please Enter a Float:");
    let mut temp = String::new();
    io::stdin()
        .read_line(&mut temp)
        .expect("Failed to read temp");
    let temp: f32 = temp.trim().parse::<f32>().expect("Failed to parse temp");
    println!("Was it in F or in C? (only F/C)");
    let mut sys = String::new();
    io::stdin().read_line(&mut sys)
        .expect("Failed to read sys");
    if sys.trim() == "F" {
        let _temp = f_to_c(temp);
    } else {
        let _temp = c_to_f(temp);
    }
}

fn f_to_c(f: f32) -> f32 {
    println!("F to C");
    let c: f32 = (5.0/9.0)*(f - 32.0);
    println!("Your temp in Celcius is: {}", c);
    return c
}

fn c_to_f(c: f32) -> f32 {
    println!("C to F");
    let f: f32 = (9.0/5.0)*c + 32.0;
    println!("Your temp in Fahrenheit is: {}", f);
    return f
}
