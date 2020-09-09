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
#include <ctype.h> // isalpha()

#define SRVRAPORT "6565" // the port users will be connecting to

// get sockaddr, IPv4 or IPv6:
void *get_in_addr (struct sockaddr *sa)
{
  if (sa->sa_family == AF_INET) {
    return &( ((struct sockaddr_in*)sa)->sin_addr );
  }
  return &( ((struct sockaddr_in6*)sa)->sin6_addr );
}

int main (void)
{
  int srvrAListeningFD;
  struct addrinfo hints, *servinfo, *p;
  int rv;
  int numbytes;
  struct sockaddr_storage awsAddr;
  int bufSize = 2048;
  char *buf = (char *)calloc(bufSize, sizeof(*buf));
  socklen_t addr_len;
  char s[INET6_ADDRSTRLEN];

  /*START: parsing the map file*/
  int *adjmat;
  int num_maps = 0, map_cursor = 0;
  int num_vertices = 0, num_edges = 0;
  char seen_vertices = 0;
  int v1 = 0, v2 = 0, c;
  FILE *fin = fopen("map.txt", "r");
  while (fgets(buf, bufSize, fin)) {
    if ( isalpha(buf[0]) ) {
      ++num_maps;
    }
  }
  printf("The Server A has constructed a list of %d maps:\n", num_maps);
  for (int i = 0; i < 30; ++i) { printf( (i != 29) ? "-" : "-\n" ); }
  printf("MapID  NumVertices  NumEdges\n");
  for (int i = 0; i < 30; ++i) { printf( (i != 29) ? "-" : "-\n" ); }
  rewind(fin);
  while (fgets(buf, bufSize, fin)) {
    if ( isalpha(buf[0]) ) { map_cursor = 0; }
    switch (map_cursor) {
    case 0:
      printf("%-7s", buf); // mapid + 6whitespaces
      ++map_cursor;
      break;
    case 1:
      ++map_cursor;
      break;
    case 2:
      ++map_cursor;
      break;
    default:
      /*TODO: YOU ARE MISSING THE CASE WHERE V1/V2 = 0*/
      sscanf(buf, "%d %d %*d", &v1, &v2);
      seen_vertices ^= (1 << v1);
      seen_vertices ^= (1 << v2);
      num_vertices += ( (seen_vertices >> v1) & 1 ) ? 0 : 1;
      num_vertices += ( (seen_vertices >> v2) & 1 ) ? 0 : 1;
      ++num_edges;
      break;
    }
    /*START: mimic peek()*/
    c = fgetc(fin);
    if ( isalpha(c) ) {
      sprintf(buf, "%d", num_vertices);
      printf("%-7s", buf);
      memset(buf, 0, bufSize * sizeof(*buf));
      sprintf(buf, "%d", num_edges);
      printf("%-7s\n", buf);
      memset(buf, 0, bufSize * sizeof(*buf));
    }
    ungetc(c, fin);
    /*END: mimic peek()*/
  }
  fclose(fin);
  /*END: parsing the map file*/

  /*START: set up ServerA Socket*/
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC; // set AF_INET to force IPv4
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE; // use my IP
  if ( (rv = getaddrinfo(NULL, SRVRAPORT, &hints, &servinfo)) != 0 ) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }
  // loop through all the results and bind to the first we can
  for (p = servinfo; p != NULL; p = p->ai_next) {
    if ( (srvrAListeningFD = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1 ) {
      perror("listener: socket");
      continue;
    }
    if ( bind(srvrAListeningFD, p->ai_addr, p->ai_addrlen) == -1 ) {
      close(srvrAListeningFD);
      perror("listener: bind");
      continue;
    }
    break;
  }
  if (p == NULL) {
    fprintf(stderr, "listener: failed to bind socket\n");
    return 2;
  }
  freeaddrinfo(servinfo);
  /*END: set up ServerA Socket*/
  printf("The ServerA is up and running using UDP on port %s.\n", SRVRAPORT);

  /*START: receiveing and sending udp packets*/
  memset(buf, 0, bufSize * sizeof(*buf));
  addr_len = sizeof awsAddr;
  numbytes = recvfrom(srvrAListeningFD, buf, bufSize * sizeof(*buf), MSG_WAITALL, (struct sockaddr *)&awsAddr, &addr_len);
  if (numbytes == -1) {
    perror("recvfrom (awsAddr not filled)");
    exit(1);
  }
  printf("ServerA: got packet from %s\n", inet_ntop(awsAddr.ss_family, get_in_addr((struct sockaddr *)&awsAddr), s, sizeof s));
  printf("ServerA: packet is %d bytes long\n", numbytes);
  buf[numbytes] = '\0';
  printf("ServerA: packet contains \"%s\"\n", buf);

  memset(buf, 0, bufSize * sizeof(*buf));
  sprintf(buf, "ACK the message from AWS->ServerA");
  numbytes = sendto(srvrAListeningFD, buf, strlen(buf), 0, (struct sockaddr *)&awsAddr, addr_len);
  if (numbytes == -1) {
    perror("talker: sendto");
    exit(1);
  }
  /*END: receiveing and sending udp packets*/

  close(srvrAListeningFD);

  return 0;
}
