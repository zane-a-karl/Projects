#include "djikstra.h"

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



  /*START: parsing the map file*/
  int num_maps = 0, map_cursor = 0;
  char seen_vertices = 0;
  int v1 = 0, v2 = 0, c;
  FILE *fin;
  if ( (fin = fopen("map.txt", "r")) == NULL ) {
    perror("Failed to open map.txt");
    exit(1);
  }
  while (fgets(buf, bufSize, fin)) {
    if ( isalpha(buf[0]) ) {
      ++num_maps;
    }
  }
  printf("The Server A has constructed a list of %d maps:\n", num_maps);
  int *num_vertices = (int *)calloc(num_maps, sizeof(*num_vertices));
  int *num_edges =  (int *)calloc(num_maps, sizeof(*num_vertices));
  int map_i = -1;
  int *propagation_speed = (int *)calloc(num_maps, sizeof(*propagation_speed));
  int *transmission_speed = (int *)calloc(num_maps, sizeof(*transmission_speed));


  for (int i = 0; i < 34; ++i) { printf( (i != 33) ? "-" : "-\n" ); }
  printf("MapID     NumVertices     NumEdges\n");
  for (int i = 0; i < 34; ++i) { printf( (i != 33) ? "-" : "-\n" ); }
  rewind(fin);
  while (fgets(buf, bufSize, fin)) {
    switch (map_cursor) {
    case 0:
      ++map_i;
      printf("%-10c", buf[0]); // mapid + 6whitespaces
      ++map_cursor;
      break;
    case 1:
      sscanf(buf, "%d", propagation_speed + map_i);
      ++map_cursor;
      break;
    case 2:
      sscanf(buf, "%d", transmission_speed + map_i);      
      ++map_cursor;
      break;
    default:
      sscanf(buf, "%d %d %*d", &v1, &v2);
      if ( !((seen_vertices >> v1) & 1) ) {
	seen_vertices ^= (1 << v1);
	++num_vertices[map_i];
      }
      if ( !((seen_vertices >> v2) & 1) ) {
	seen_vertices ^= (1 << v2);
	++num_vertices[map_i];
      }
      ++num_edges[map_i];
      break;
    }
    /*START: mimic peek()*/
    c = fgetc(fin);
    if ( isalpha(c) || feof(fin) ) {
      sprintf(buf, "%d", num_vertices[map_i]);       // num_vertices -> str
      printf("%-16s", buf);
      memset(buf, 0, bufSize * sizeof(*buf)); // clear buf
      sprintf(buf, "%d", num_edges[map_i]);          // num_edges -> str
      printf("%-13s\n", buf);
      memset(buf, 0, bufSize * sizeof(*buf)); // clear buf
      map_cursor = 0;                         // Reset the cursor
      //      num_vertices = 0;
      //      num_edges = 0;
      seen_vertices = 0;
    }
    ungetc(c, fin);
    /*END: mimic peek()*/
  } // end while
  for (int i = 0; i < 34; ++i) { printf( (i != 33) ? "-" : "-\n" ); }
  /*END: parsing the map file*/



  /*START: receiveing udp packets*/
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
  char mapID;
  int startNode;
  sscanf(buf, "%c %d %*d", &mapID, &startNode);
  printf("The Server A has received input for finding shortest paths: starting vertex %d of map %c.\n", startNode, mapID);
  /*END: receiving udp packets*/

  /*START: constructing the chosen graph*/
  rewind(fin);
  memset(buf, 0, bufSize * sizeof(*buf));
  map_i = mapID - 65; // convert map ID to int.
  int **adj_mat = (int **)calloc(num_vertices[map_i], sizeof(*adj_mat));
  for (int i = 0; i < num_vertices[map_i]; ++i) {
    adj_mat[i] = (int *)calloc(num_vertices[map_i], sizeof(*adj_mat[i]));
  }
  int dist = 0;

  while ( fgets(buf, bufSize, fin) ) {
    if ( buf[0] == mapID ) {
      ++map_cursor;
      while ( fgets(buf, bufSize, fin) ) {
	if ( isalpha(buf[0]) ) { break; }
	switch (map_cursor) {
	case 0:
	  break;
	case 1:
	  ++map_cursor;
	  break;
	case 2:
	  ++map_cursor;
	  break;
	default:
	  sscanf(buf, "%d %d %d", &v1, &v2, &dist);
	  adj_mat[v1][v2] = dist;
	  adj_mat[v2][v1] = dist;
	  break;
	} // end switch
      } // end while
      break;
    } //end if
  } // end while


  fclose(fin);
  /*END: constructing the chosen graph*/

  /*START: Printing the adjacency matrix
  for (int i = 0; i < num_vertices[map_i]; ++i) {
    for (int j = 0; j < num_vertices[map_i]; ++j) {
      printf( (j==num_vertices[map_i]-1) ? "%-3d\n" : "%-3d ", adj_mat[i][j]);
    }
  }
  printf("\n");
  END: Printing the adjacency matrix*/

  /*START: Perform djikstra's alg*/
  int *distances = calloc(num_vertices[map_i], sizeof(*distances));
  distances = djikstra_Ov2(adj_mat, num_vertices[map_i], startNode);
  printf("The Server A has identified the following shortest paths:\n");
  for (int i = 0; i < 25; ++i) { printf( (i != 24) ? "-" : "-\n" ); }
  printf("Destination     MinLength\n");
  for (int i = 0; i < 25; ++i) { printf( (i != 24) ? "-" : "-\n" ); }
  for (int i = 0; i < num_vertices[map_i]; ++i) {
    printf( "%-15d %d\n", i, distances[i] );
  }
  for (int i = 0; i < 25; ++i) { printf( (i != 24) ? "-" : "-\n" ); }
  /*END: Perform djikstra's alg*/

  /*START: sending udp packets*/
  memset(buf, 0, bufSize * sizeof(*buf));
  sprintf( buf, "%d %d", propagation_speed[map_i], transmission_speed[map_i]);
  numbytes = sendto(srvrAListeningFD, buf, strlen(buf), 0, (struct sockaddr *)&awsAddr, addr_len);
  if (numbytes == -1) {
    perror("talker: sendto");
    exit(1);
  }

  memset(buf, 0, bufSize * sizeof(*buf));
  for (int i = 0; i < num_vertices[map_i]; ++i) {
    // You'd have to create a new (FILE *) but fmemopen(3) would append these strings w/o recalc'ing strlen.
    sprintf( buf + strlen(buf), (i != num_vertices[map_i]-1) ? "%-15d %d\n" : "%-15d %d", i, distances[i] );
  }
  numbytes = sendto(srvrAListeningFD, buf, strlen(buf), 0, (struct sockaddr *)&awsAddr, addr_len);
  if (numbytes == -1) {
    perror("talker: sendto");
    exit(1);
  }
  printf("The Server A has sent shortest paths to AWS.\n");
  /*END: sending udp packets*/

  close(srvrAListeningFD);

  return 0;
}
