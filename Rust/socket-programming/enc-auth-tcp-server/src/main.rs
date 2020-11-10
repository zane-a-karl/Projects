use sodiumoxide::crypto::box_;
use std::io::{Read, Write};
use std::net::{Shutdown, TcpListener, TcpStream};
use std::thread;

fn handle_client(mut stream: TcpStream) {
    match sodiumoxide::init() {
        Ok(()) => println!("sodium is available for use"),
        Err(_) => panic!("Init failed"),
    };
    let (server_pk, server_sk) = box_::gen_keypair();
    stream.write(server_pk.as_ref()).unwrap();

    let mut client_pk = [0 as u8; 32];
    stream.read(&mut client_pk).unwrap();
    
    let mut data = [0 as u8; 50]; // i.e. 50 bytes of 0s
    while match stream.read(&mut data) {
        Ok(size) => {
            // echo everything!
            let nonce = [7 as u8; 24]; //box_::gen_nonce();
            let data = box_::open(
                &data,
                &box_::curve25519xsalsa20poly1305::Nonce::from_slice(&nonce).unwrap(),
                &box_::curve25519xsalsa20poly1305::PublicKey::from_slice(&client_pk).unwrap(),
                &server_sk
            )
            .unwrap();

            stream.write(&data[0..size]).unwrap();
            true
        }
        Err(_) => {
            println!(
                "An error occurred, terminating connection with {}",
                stream.peer_addr().unwrap()
            );
            stream.shutdown(Shutdown::Both).unwrap();
            false
        }
    } {}
}

fn main() {
    let listener = TcpListener::bind("0.0.0.0:3333").unwrap();
    //accept connections and process them, spawning a new thread for each one
    println!("Server listening on port 3333");
    for stream in listener.incoming() {
        match stream {
            Ok(stream) => {
                println!("New connection {}", stream.peer_addr().unwrap());
                thread::spawn(move || {
                    // connection succeeded
                    handle_client(stream)
                });
            }
            Err(e) => {
                // connection failed
                println!("Error {}", e);
            }
        }
    }
    // close the socket server
    drop(listener);
}
