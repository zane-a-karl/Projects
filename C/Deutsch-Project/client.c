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
#include "client-utils.h"

#define CLNTPORT "4141"
#define AWSPORT "4242" // the port client will be connecting to

#define MAXDATASIZE 100 // max number of bytes we can get at once


int main (int argc,
	  char *argv[]) {

  int clnt_sock_fd, numbytes;
  int getaddrinfo_failed;
  char buf[MAXDATASIZE];
  char s[INET6_ADDRSTRLEN];
  struct addrinfo sock_preferences;
  struct addrinfo *possible_cnntns;

  check_number_of_args(argc);
  set_sock_preferences(&sock_preferences);
  getaddrinfo_failed = getaddrinfo(NULL,
				   AWSPORT,
				   &sock_preferences,
				   &possible_cnntns);
  if ( getaddrinfo_failed ) {
    fprintf(stderr,
	    "getaddrinfo: %s\n",
	    gai_strerror(getaddrinfo_failed));
    return 1;
  }

  printf("The client is up and running.\n");
  create_sock_and_connect(clnt_sock_fd, possible_cnntns);
  freeaddrinfo(possible_cnntns);

  send_msg(clnt_sock_fd, 256, argv);
  printf("The client has sent query to AWS using TCP");
  printf("over port %s:\n", AWSPORT);
  printf("start vertex %s; ", argv[2]);
  printf("map %s; ", argv[1]);
  printf("file size %s.\n", argv[3]);

  int clientBufSize = 2048;
  char *clientBuf = (char *)calloc(clientBufSize, sizeof(*clientBuf));
  int numRecvBytes;

  /*START: Receive the Distances*/
  if ( ( numRecvBytes = recv(clnt_sock_fd, clientBuf, clientBufSize, 0) ) == -1) {
    perror("Error: receiving initial Client Data\n");
    close(clnt_sock_fd);
    exit(0);
  }
  clientBuf[numRecvBytes] = '\0';
  int num_distances = 1; // The last line has no '\n'
  for ( int i = 0; i < strlen(clientBuf); ++i ) {
    if (clientBuf[i] == ' ') { ++num_distances; }
  }

  int *whitespace_locs = (int *)calloc( num_distances, sizeof(*whitespace_locs) );
  int wl_i = 0;
  for ( int i = 0; i < strlen(clientBuf); ++i ) {
    if (clientBuf[i] == ' ') { whitespace_locs[++wl_i] = i; } // pre-fix increment to account for string structure.
  }
  int *distances = (int *)calloc(num_distances, sizeof(*distances));
  for ( int i = 0;  i != num_distances; ++i ) {
    sscanf(clientBuf + ( i == 0 ? i : whitespace_locs[i] ), "%d ", distances + i);
  }
  /*END: Receive the Distances*/

  /*START: Receive the Delays*/
  memset(clientBuf, 0, clientBufSize * sizeof(*clientBuf));
  if ( ( numRecvBytes = recv(clnt_sock_fd, clientBuf, clientBufSize, 0) ) == -1) {
    perror("Error: receiving initial Client Data\n");
    close(clnt_sock_fd);
    exit(0);
  }
  clientBuf[numRecvBytes] = '\0';

  double *total_delays = (double *)calloc( num_distances, sizeof(*total_delays) );
  double *propagation_delays = (double *)calloc( num_distances, sizeof(*propagation_delays) );
  double *transmission_delays = (double *)calloc( num_distances, sizeof(*transmission_delays) );
  int *newline_locs = (int *)calloc( num_distances, sizeof(*newline_locs) );
  int nl_i = 0;
  for ( int i = 0; i < strlen(clientBuf); ++i ) {
    if (clientBuf[i] == '\n') { newline_locs[++nl_i] = i; } // pre-fix increment to account for string structure.
  }
  for ( int i = 0;  i != num_distances; ++i ) {
    sscanf(clientBuf + (i == 0 ? i : newline_locs[i] ),
	   "%lf %lf %lf",
	   transmission_delays + i,
	   propagation_delays + i,
	   total_delays + i );
  }
  /*END: Receive the Delays*/

  printf("The client has received results from AWS:\n");
  for (int i = 0; i < 67; ++i) { printf( (i != 66) ? "-" : "-\n" ); }
  printf("Destination     MinLength     TransDelay     PropDelay     TotDelay\n");
  for (int i = 0; i < 67; ++i) { printf( (i != 66) ? "-" : "-\n" ); }
  for (int i = 0; i < num_distances; ++i) {
    printf( "%-15d %-13d %-14.3f %-13.3f %-16.3f\n", i, distances[i], transmission_delays[i], propagation_delays[i], total_delays[i]);
  }
  for (int i = 0; i < 67; ++i) { printf( (i != 66) ? "-" : "-\n" ); }


  close(clnt_sock_fd);
  return 0;
}
