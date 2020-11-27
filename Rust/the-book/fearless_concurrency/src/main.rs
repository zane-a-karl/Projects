use std::thread;
use std::time::Duration;

#[allow(unused)]
fn control_thread_completion() {
    let handle = thread::spawn(|| {
        for i in 1..10 {
            println!("Hi number {} from the spawned thread", i);
            thread::sleep(Duration::from_millis(1));
        }
    });
    // Un-comment to have the spawned thread run to completion before the main thread starts.
    // handle.join().unwrap();
    for i in 1..5 {
        println!("Hi number {} from the main thread", i);
        thread::sleep(Duration::from_millis(1));
    }

    // Allow the spawned thread to guarantee finishing even if the main thread completes early.
    handle.join().unwrap();
}

fn main() {
    let v = vec![1, 2, 3];
    let handle = thread::spawn(move || {
        println!("Here's a vector: {:?}", v);
    });
    handle.join().unwrap();
}
