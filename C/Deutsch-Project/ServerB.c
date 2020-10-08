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

#define SRVRBPORT "6666" // the port users will be connecting to

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
  int srvrBListeningFD;
  struct addrinfo hints, *servinfo, *p;
  int rv;
  int numbytes;
  struct sockaddr_storage awsAddr;
  int bufSize = 2048;
  char *buf = (char *)calloc(bufSize, sizeof(*buf));
  socklen_t addr_len;
  char s[INET6_ADDRSTRLEN];

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC; // set AF_INET to force IPv4
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE; // use my IP
  if ( (rv = getaddrinfo(NULL, SRVRBPORT, &hints, &servinfo)) != 0 ) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }
  // loop through all the results and bind to the first we can
  for (p = servinfo; p != NULL; p = p->ai_next) {
    if ( (srvrBListeningFD = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1 ) {
      perror("listener: socket");
      continue;
    }
    if ( bind(srvrBListeningFD, p->ai_addr, p->ai_addrlen) == -1 ) {
      close(srvrBListeningFD);
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
  printf("The ServerB is up and running using UDP on port %s.\n", SRVRBPORT);

  // Recv Propagation speed and Transmission Speed.
  int propagation_speed = 0;
  int transmission_speed = 0;
  int num_distances = 0;
  int file_size = 0;
  memset(buf, 0, bufSize * sizeof(*buf));
  addr_len = sizeof awsAddr;
  numbytes = recvfrom(srvrBListeningFD, buf, bufSize * sizeof(*buf), MSG_WAITALL, (struct sockaddr *)&awsAddr, &addr_len);
  if (numbytes == -1) {
    perror("recvfrom (awsAddr not filled)");
    exit(1);
  }
  sscanf(buf, "%d %d %d %d", &propagation_speed, &transmission_speed, &num_distances, &file_size);

  // Recv shortest path distances.
  memset(buf, 0, bufSize * sizeof(*buf));
  addr_len = sizeof awsAddr;
  numbytes = recvfrom(srvrBListeningFD, buf, bufSize * sizeof(*buf), MSG_WAITALL, (struct sockaddr *)&awsAddr, &addr_len);
  if (numbytes == -1) {
    perror("recvfrom (awsAddr not filled)");
    exit(1);
  }
  int *whitespace_locs = (int *)calloc( num_distances, sizeof(*whitespace_locs) );
  int wl_i = 0;
  for ( int i = 0; i < strlen(buf); ++i ) {
    if (buf[i] == ' ') { whitespace_locs[++wl_i] = i; } // pre-fix increment to account for string structure.
  }
  int *distances = (int *)calloc(num_distances, sizeof(*distances));
  for ( int i = 0;  i != num_distances; ++i ) {
    sscanf(buf + ( i == 0 ? i : whitespace_locs[i] ), "%d ", distances + i);
  }

  printf("ServerB: recv'd a packet that is %d bytes long\n", numbytes);
  buf[numbytes] = '\0';
  printf("The Server B has received data for calculation:\n");
  printf("* Propagation speed: %dkm/s\n", propagation_speed);
  printf("* Transmission speed: %dBytes/s\n", transmission_speed);
  for ( int i = 0; i < num_distances; ++i ) {
    printf("* Path length for destination %d: %d\n", i, distances[i]);
  }

  double *total_delays = (double *)calloc( num_distances, sizeof(*total_delays) );
  double *propagation_delays = (double *)calloc( num_distances, sizeof(*propagation_delays) );
  double *transmission_delays = (double *)calloc( num_distances, sizeof(*transmission_delays) );
  printf("\nThe Server B has finished the calculation of the delays:\n");
  for (int i = 0; i < 25; ++i) { printf( (i != 24) ? "-" : "-\n" ); }
  printf("Destination     Delay\n");
  for (int i = 0; i < 25; ++i) { printf( (i != 24) ? "-" : "-\n" ); }
  for ( int i = 0; i < num_distances; ++i ) {
    propagation_delays[i] = (double)distances[i] / propagation_speed;
    transmission_delays[i] =  (double)file_size / (8*transmission_speed);
    total_delays[i] = propagation_delays[i] + transmission_delays[i];
    printf( "%-15d %.6f\n", i, total_delays[i]);
  }
  for (int i = 0; i < 25; ++i) { printf( (i != 24) ? "-" : "-\n" ); }

  memset(buf, 0, bufSize * sizeof(*buf));
  for (int i = 0; i < num_distances; ++i) {
    sprintf( buf + strlen(buf),
	     (i != num_distances-1 ? "%.3f %.3f %.3f\n" : "%.3f %.3f %.3f"), // redundant code here
	     transmission_delays[i],
	     propagation_delays[i],
	     total_delays[i] );
  }
  numbytes = sendto(srvrBListeningFD, buf, strlen(buf), 0, (struct sockaddr *)&awsAddr, addr_len);
  if (numbytes == -1) {
    perror("talker: sendto");
    exit(1);
  }

  printf("The Server B has finished sending the output to AWS\n");

  close(srvrBListeningFD);

  return 0;
}
