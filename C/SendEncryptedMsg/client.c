#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <openssl/bn.h>
#include <openssl/rand.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#include <openssl/dh.h>


#define PORT "3490" // the port client will be connecting to

#define MAXDATASIZE 100 // max number of bytes we can get at once

/*gcc client.c -o client.out -I /usr/local/opt/openssl@1.1/include -L /usr/local/opt/openssl@1.1/lib -lcrypto -Wall*/


// get sockaddr, IPv4 or IPv6:
void *get_in_addr (struct sockaddr *sa)
{
  if ( sa->sa_family == AF_INET ) {
    return &( ((struct sockaddr_in*)sa)->sin_addr );
  }
  return &( ((struct sockaddr_in6*)sa)->sin6_addr );
}

int main (int argc, char *argv[])
{
  int sockfd, numbytes;
  char buf[MAXDATASIZE];
  struct addrinfo hints, *servinfo, *p; int rv;
  char srvr_ip_addr[INET6_ADDRSTRLEN];


  /*START: initializing the libcrypto stuff*/
  /* Load the human readable error strings for libcrypto */
  ERR_load_crypto_strings();

  /* Load all digest and cipher algorithms */
  OpenSSL_add_all_algorithms();

  /* Load config file, and other important initialisation */
  OPENSSL_config(NULL);
  /*END: initializing the libcrypto stuff*/

  
  

  if (argc != 2) {
    fprintf(stderr,"usage: client hostname\n");
    exit(1);
  }

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  // loop through all the results and connect to the first we can
  for ( p = servinfo; p != NULL; p = p->ai_next ) {
    if ( (sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1 ) {
      perror("client: socket");
      continue;
    }
    if ( connect(sockfd, p->ai_addr, p->ai_addrlen) == -1 ) {
      close(sockfd);
      perror("client: connect");
      continue;
    }
    break;
  }

  if ( p == NULL ) {
    fprintf(stderr, "client: failed to connect\n");
    return 2;
  }

  inet_ntop( p->ai_family, get_in_addr( (struct sockaddr *)p->ai_addr ), srvr_ip_addr, sizeof (srvr_ip_addr) );
  printf("client: connecting to %s\n", srvr_ip_addr);

  freeaddrinfo(servinfo); // all done with this structure 80
  if ( ( numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0) ) == -1 ) {
    perror("recv");
    exit(1);
  }
  buf[numbytes] = '\0';

  printf("client: received '%s'\n",buf);
  close(sockfd);

  
  /*START: cleaning up the libcrypto stuff*/
  /* Removes all digests and ciphers */
  EVP_cleanup();

  /* if you omit the next, a small leak may be left when you make use of the BIO (low level API) for e.g. base64 transformations */
  CRYPTO_cleanup_all_ex_data();

  /* Remove error strings */
  ERR_free_strings();
  /*END: cleaning up the libcrypto stuff*/

  return 0;
}
