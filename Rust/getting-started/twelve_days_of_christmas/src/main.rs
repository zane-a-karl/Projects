fn main() {
    sing_twelve_days_of_christmas();
}

fn sing_twelve_days_of_christmas () {
    
    for i in 1..13 {
	println!("");
	print!("On the {}", i);
	for j in (1..(i+1)).rev() {
	    if j == 12 {
		if i == 12 {println!("th day of chrsitmas my true love gave to me");}
		println!("Twelve drummers drumming");
	    }
	    if j == 11 {
		if i == 11 {println!("th day of chrsitmas my true love gave to me");}
		println!("Eleven pipers piping");
	    }
	    if j == 10 {
		if i == 10 {println!("th day of chrsitmas my true love gave to me");}
		println!("Ten lords a leaping");
	    }
	    if j == 9 {
		if i == 9 {println!("th day of chrsitmas my true love gave to me");}
		println!("Nine ladies dancing");
	    }
	    if j == 8 {
		if i == 8 {println!("th day of chrsitmas my true love gave to me");}
		println!("Eight maids a milking");
	    }
	    if j == 7 {
		if i == 7 {println!("th day of chrsitmas my true love gave to me");}
		println!("Seven swans a swimming");
	    }
	    if j == 6 {
		if i == 6 {println!("th day of chrsitmas my true love gave to me");}
		println!("Six geese a laying");
	    }
	    if j == 5 {
		if i == 5 {println!("th day of chrsitmas my true love gave to me");}
		println!("Five golden rings!");
	    }
	    if j == 4 {
		if i == 4 {println!("th day of chrsitmas my true love gave to me");}
		println!("Four calling birds");
	    }
	    if j == 3 {
		if i == 3 {println!("rd day of chrsitmas my true love gave to me");}
		println!("Three French Hens");
	    }
	    if j == 2 {
		if i == 2 {println!("nd day of chrsitmas my true love gave to me");}
		println!("Two turtle-doves");
	    }
	    if j == 1 {
		if i == 1 { println!("st day of chrsitmas my true love gave to me");}
		if i != 1 {println!("And a partridge in a pear tree")} else {println!("A partridge in a pear tree")};
	    }
	}
    }
}
