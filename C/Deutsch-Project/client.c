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

#define CLNTPORT "4141"
#define AWSPORT "4242" // the port client will be connecting to

#define MAXDATASIZE 100 // max number of bytes we can get at once


// get sockaddr, IPv4 or IPv6:
void *get_in_addr (struct sockaddr *sa)
{
  if ( sa->sa_family == AF_INET ) {
    return &( ((struct sockaddr_in*)sa)->sin_addr );
  }
  return &( ((struct sockaddr_in6*)sa)->sin6_addr );
}

void checkClientProgInput (int argc)
{
  if (argc < 2) {
    fprintf(stderr,"The client program takes input!\n");
    exit(1);
  }
}

int main (int argc, char *argv[])
{
  int sockfd, numbytes;
  char buf[MAXDATASIZE];
  struct addrinfo hints, *servinfo, *p; int rv;
  char s[INET6_ADDRSTRLEN];

  checkClientProgInput(argc);

  /*START: get addrinfo*/
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  if ( ( rv = getaddrinfo(NULL, AWSPORT, &hints, &servinfo) ) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }
  /*END: get addrinfo*/
  
  printf("The client is up and running.\n");
  /*START:create socket and connect to AWS*/
  for ( p = servinfo; p != NULL; p = p->ai_next ) {
    if ( (sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1 ) {
      perror("client: socket");
      continue;
    }
    // Connect to 1st poss result from servinfo.
    if ( connect(sockfd, p->ai_addr, p->ai_addrlen) == -1 ) {
      close(sockfd);
      perror("client: connect");
      continue;
    }
    break;
  }
  /*END:create socket and connect to AWS*/

  /*START: connection error check*/
  if ( p == NULL ) {
    fprintf(stderr, "client: failed to connect\n");
    return 2;
  }
  /*END: connection error check*/
  
  freeaddrinfo(servinfo); // all done with this structure
  
  char *msgToAWS = (char *)calloc(256, sizeof(*msgToAWS));
  sprintf(msgToAWS, "%s %s %s", argv[1], argv[2], argv[3]);
  if ( send(sockfd, msgToAWS, 256, 0) == -1 ) {
    perror("Failed to send intial client msg to AWS!\n");
    close(sockfd);
    exit(0);
  }
  printf("The client has sent query to AWS using TCP over port %s:\n", AWSPORT);
  printf("start vertex %s; map %s; file size %s.\n", argv[2], argv[1], argv[3]);
  free(msgToAWS);

  int clientBufSize = 1024;
  char *clientBuf = (char *)calloc(clientBufSize, sizeof(*clientBuf));
  int numRecvBytes;
  if ( ( numRecvBytes = recv(sockfd, clientBuf, clientBufSize, 0) ) == -1) {
    perror("Error: receiving initial Client Data\n");
    close(sockfd);
    exit(0);
  }
  clientBuf[numRecvBytes] = '\0';
  printf("%s\n", clientBuf);

  
  close(sockfd);
  return 0;
}
