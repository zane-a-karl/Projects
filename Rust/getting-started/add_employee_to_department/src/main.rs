// Using a hash map and vectors, create a text interface to allow a user to add employee names to a department in a company. For example, “Add Sally to Engineering” or “Add Amir to Sales.” Then let the user retrieve a list of all people in a department or all people in the company by department, sorted alphabetically.

use std::io;
use std::collections::HashMap;

fn get_employee (s: &str) -> String {
    let mut emp_name = String::new();
    let s_vec: Vec<&str> = s.split_whitespace().collect();
    let mut add_idx = 0;
    let mut to_idx = 0;
    for i in 0..s_vec.len() {
	match s_vec[i] {
	    "Add" => add_idx = i,
	    "to"  => to_idx = i,
	    _ => continue,
	};
    }
    for i in (add_idx+1)..to_idx {
	emp_name.push_str(&s_vec[i]);
	if i+1 == to_idx {
	    break;
	} else {
	    emp_name.push(' ');
	}
    }
    emp_name
}

fn get_department (s: &str) -> String {
    let mut dep_name = String::new();
    let s_vec: Vec<&str> = s.split_whitespace().collect();
    let mut to_idx = 0;
    for i in 0..s_vec.len() {
	if let "to" = s_vec[i] {
	    to_idx = i;
	}
    }
    for i in (to_idx+1)..s_vec.len() {
	dep_name.push_str(&s_vec[i]);
	if i+1 == s_vec.len() {
	    break;
	} else {
	    dep_name.push(' ');
	}
    }
    dep_name
}

fn add_employee_to_department (en_val: &str, dn_key: &str, sd_map: &mut HashMap<String, Vec<String>>) {
    sd_map.entry(dn_key.to_string()).or_insert(Vec::new()).push(en_val.to_string())
}

fn main() {

    println!("Add a new employee!");
    let mut staff_directory: HashMap<String, Vec<String>>  = HashMap::new();
    loop {
	println!("Please input \"Add <employee name> to <department name>\"");
	let mut add_str = String::new();
	io::stdin()
	    .read_line(&mut add_str)
	    .expect("Failed to read in the add_str");
	match add_str.trim() {
	    "break" => break,
	    "list" => {
		for (k, v) in &staff_directory {
		    print!("In the {} department there's: ", k);
		    for e in v {
			print!("{}, ", e);
			}
		    println!("");
		}
	    },
	    _ => (),
	};
	let emp_name = get_employee(&add_str);
	let dep_name = get_department(&add_str);
	if emp_name.trim() != "" {
	    println!("\tAdding {} to {}...", &emp_name, &dep_name);
	    add_employee_to_department(&emp_name, &dep_name, &mut staff_directory);
	}
    }
}
