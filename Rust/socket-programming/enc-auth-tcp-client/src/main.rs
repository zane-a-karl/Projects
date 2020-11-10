use sodiumoxide::crypto::box_;
use std::io::{Read, Write};
use std::net::TcpStream;
use std::str::from_utf8;

fn main() {
    match sodiumoxide::init() {
        Ok(()) => println!("sodium is available for use"),
        Err(_) => panic!("Init failed"),
    };
    let (client_pk, client_sk) = box_::gen_keypair();
    match TcpStream::connect("localhost:3333") {
        Ok(mut stream) => {
            println!("Successfully connected to server in port 3333");
            let mut server_pk = [0 as u8; 32];
            stream.read(&mut server_pk).unwrap();

            stream.write(&client_pk.as_ref()).unwrap();

            let nonce = [7 as u8; 24]; //box_::gen_nonce();
            let msg = b"Hello!";
            let ciphertext = box_::seal(
                msg,
                &box_::curve25519xsalsa20poly1305::Nonce::from_slice(&nonce).unwrap(),
		&box_::curve25519xsalsa20poly1305::PublicKey::from_slice(&server_pk).unwrap(),
                &client_sk,
            );

            stream.write(&ciphertext).unwrap();
            println!("Sent Message, awaiting reply...");
            let mut data = [0 as u8; 6]; // using 6 byte buffer

            match stream.read_exact(&mut data) {
                Ok(_) => {
                    if &data[..] == msg {
                        println!("Reply is ok!");
                    } else {
                        let text = from_utf8(&data).unwrap();
                        println!("Unexpected reply: !{}!", text);
                    }
                }
                Err(e) => {
                    println!("Failed to receive data: {}", e);
                }
            }
        }
        Err(e) => {
            println!("Failed to connect: {}", e);
        }
    }
    println!("Terminated.");
}
