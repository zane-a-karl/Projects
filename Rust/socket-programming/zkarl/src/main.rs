use sodiumoxide::crypto::secretbox::{self, Key};
use sodiumoxide::hex;
// Copy-pasted from the output of the commented-out main fn:
const KEY: &str = "f8956cf7a86f419ec4078da8b78650949e01b49b858f6f0bd8df484bbf3fb79d";
fn main() {
    let key = Key::from_slice(&hex::decode(KEY).unwrap()).unwrap();
    println!("{}", hex::encode(&key));
}
