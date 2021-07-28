use std::string::String;
use tokio::{
    io::{AsyncBufReadExt, AsyncWriteExt, BufReader},
    net::TcpListener,
    sync::broadcast,
};

#[tokio::main]
async fn main() {
    let listener: TcpListener = TcpListener::bind("localhost:8080").await.unwrap();

    let (tx, _rx) = broadcast::channel(10);

    loop {
        let (mut socket, addr) = listener.accept().await.unwrap();
        let tx = tx.clone();
        let mut rx = tx.subscribe();

        tokio::spawn(async move {
            let (sock_read, mut sock_write) = socket.split();
            let mut reader = BufReader::new(sock_read);
            let mut line = String::new();

            loop {
                tokio::select! {
                    result = reader.read_line(&mut line) => {			
			if result.unwrap() == 0 {
			    break;
			}
			
			tx.send((line.clone(), addr)).unwrap();
			line.clear();
                    }
		    result = rx.recv() => {			
			let (msg, other_addr) = result.unwrap();

			if addr != other_addr {
			    sock_write.write_all(msg.as_bytes()).await.unwrap();
			}
		    }
                }
            }
        });
    }
}
