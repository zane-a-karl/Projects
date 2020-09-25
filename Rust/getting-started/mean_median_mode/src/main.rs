// Given a list of integers, use a vector and return the mean (the average value), median (when sorted, the value in the middle position), and mode (the value that occurs most often; a hash map will be helpful here) of the list.

use std::io;
use rand::Rng;
use std::collections::HashMap;

fn mean (vals: &Vec<i32>) -> i32 {
    let mut sum = 0;
    for v in vals {
	sum += v;
    }
    sum / (vals.len() as i32)
}

fn median (vals: &mut Vec<i32>) -> i32 {
    vals.sort();
    //    for i in 0..vals.len() {
    //	println!("{} ", &vals[i]);
    //}
    if vals.len()%2 == 1 {
	return vals[vals.len()/2]
    } else {
	return ( vals[vals.len()/2 - 1] + vals[vals.len()/2] ) / 2
    }
}

fn mode (vals: &Vec<i32>) -> i32 {

    let mut hmap = HashMap::new();
    for val in vals {
	let cnt = hmap.entry(val).or_insert(0);
	*cnt += 1;
    }
    let mut mode = 0;
    let mut max = 0;
    for (k, v) in hmap {
	if max < v {
	    mode = *k;
	    max = v;
	}
    }
    mode
}

fn main() {

    println!("Please enter the length of your list: ");
    let mut len = String::new();
    io::stdin().read_line(&mut len).expect("Failed to read length");
    let len: i32 = match len.trim().parse() {
	Ok(num) => num,
	Err(_) => {
	    println!("The length will default to 10");
	    10
	}
    };
    let mut vals: Vec<i32> = Vec::with_capacity(len as usize);
    for i in 0..len as usize {
	&mut vals.push( rand::thread_rng().gen_range(-256, 255) );
	//	&mut vals.push( i as i32 % 2  );
	print!("{} ", &vals[i]);
    }
    println!("\nMean: {}", mean(&vals) );
    println!("Median: {}", median( &mut vals ) );
    println!("Mode: {}", mode(&vals) );
}
