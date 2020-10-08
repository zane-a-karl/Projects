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
#include <sys/wait.h>
#include <signal.h>

#define CLNTPORT "4141"
#define AWSTCPPORT "4242" // The port users will be connecting to.
#define AWSUDPPORT "4343" // The port users will be connecting to.
#define SRVRAPORT "6565"
#define SRVRBPORT "6666"

#define BACKLOG 10

typedef enum {
	      CTOAWS, AWSTOSRVRA, AWSTOSRVRB, AWSTOC, ENDTRANS
} AWSSTATE;

void sigchld_handler (int s)
{
  // waitpid() might overwrite errno so we save and restore it.
  int saved_errno = errno;
  while (waitpid(-1, NULL, WNOHANG) > 0);
  errno = saved_errno;
}

// get sockaddr IPv4 or IPv6
void *get_in_addr (struct sockaddr *sa)
{
  if (sa->sa_family == AF_INET) {
    return &( ((struct sockaddr_in  *)sa)->sin_addr );
  } else {
    return &( ((struct sockaddr_in6  *)sa)->sin6_addr );
  }
}

int main (int argc, char **argv)
{
  int awsTcpFD;  // listen on awsTcpFD.
  int awsUdpFD; // Receive via awsUdpFD.
  struct addrinfo tcpHints, *tcpInfo, *tcpPtr;
  struct addrinfo udpHints, *udpInfo, *udpPtr;
  int awsBufSize = 2048;
  char *awsBuf = (char *)calloc(awsBufSize, sizeof(*awsBuf));
  int numSendBytes, numRecvBytes;
  int tcpAddrInfoCheck, udpAddrInfoCheck;


  int clientFD;       // new connection on clientFD, this is from accept().
  struct sockaddr_storage clntAddr; //connector's addr info
  socklen_t clntAddrSize;
  char *clntIPAddr = (char *)calloc(INET6_ADDRSTRLEN, sizeof(*clntIPAddr));
  int clntPort;

  struct addrinfo srvrAInfo, *srvrARes, *srvrAPtr;
  int srvrAAddrInfoCheck, srvrAPort;
  memset(&srvrAInfo, 0, sizeof(srvrAInfo));
  srvrAInfo.ai_family = AF_UNSPEC;
  srvrAInfo.ai_socktype = SOCK_DGRAM;
  char *srvrAIPAddr = (char *)calloc(INET6_ADDRSTRLEN, sizeof(*srvrAIPAddr));
  srvrAAddrInfoCheck = getaddrinfo(NULL, SRVRAPORT, &srvrAInfo, &srvrARes);
  if (srvrAAddrInfoCheck != 0) {
    fprintf( stderr, "getaddrinfo (srvrA): %s\n", gai_strerror(srvrAAddrInfoCheck) );
    return 1;
  }
  for (srvrAPtr = srvrARes; srvrAPtr != NULL; srvrAPtr = srvrAPtr->ai_next) {
    if ( socket(srvrAPtr->ai_family, srvrAPtr->ai_socktype, 0) == -1 ) {
      perror("srvrA has no valid sockaddr's");
      continue;
    }
    break;
  }

  struct addrinfo srvrBInfo, *srvrBRes, *srvrBPtr;
  int srvrBAddrInfoCheck, srvrBPort;
  memset(&srvrBInfo, 0, sizeof(srvrBInfo));
  srvrBInfo.ai_family = AF_UNSPEC;
  srvrBInfo.ai_socktype = SOCK_DGRAM;
  char *srvrBIPAddr = (char *)calloc(INET6_ADDRSTRLEN, sizeof(*srvrBIPAddr));
  srvrBAddrInfoCheck = getaddrinfo(NULL, SRVRBPORT, &srvrBInfo, &srvrBRes);
  if (srvrBAddrInfoCheck != 0) {
    fprintf( stderr, "getaddrinfo (srvrB): %s\n", gai_strerror(srvrBAddrInfoCheck) );
    return 1;
  }
  for (srvrBPtr = srvrBRes; srvrBPtr != NULL; srvrBPtr = srvrBPtr->ai_next) {
    if ( socket(srvrBPtr->ai_family, srvrBPtr->ai_socktype, 0) == -1) {
      perror("srvrB has no valid sockaddr's");
      continue;
    }
    break;
  }

  struct sigaction sa;
  int yes = 1;
  char mapID;
  int startNode;
  int fileSize;

  /*START: get TCP addrinfo*/
  memset(&tcpHints, 0, sizeof(tcpHints));
  tcpHints.ai_family = AF_UNSPEC;
  tcpHints.ai_socktype = SOCK_STREAM;
  tcpHints.ai_flags = AI_PASSIVE; // Use my IP.

  if ( (tcpAddrInfoCheck = getaddrinfo(NULL, AWSTCPPORT, &tcpHints, &tcpInfo)) != 0 ) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(tcpAddrInfoCheck));
    return 1;
  }
  /*END: get TCP addrinfo*/

  /*START: get UDP addrinfo*/
  memset(&udpHints, 0, sizeof(udpHints));
  udpHints.ai_family = AF_UNSPEC;
  udpHints.ai_socktype = SOCK_DGRAM;

  if ( (udpAddrInfoCheck = getaddrinfo(NULL, AWSUDPPORT, &udpHints, &udpInfo)) != 0 ) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(udpAddrInfoCheck));
    return 1;
  }
  /*END: get UDP addrinfo*/

  // Loop through all the results and bind to the first one we can.
  /*START:create TCP socket and bind to AWSTCPPORT*/
  for (tcpPtr = tcpInfo; tcpPtr != NULL; tcpPtr = tcpPtr->ai_next) {

    if ( (awsTcpFD = socket(tcpPtr->ai_family, tcpPtr->ai_socktype, tcpPtr->ai_protocol)) == -1) {
      perror("server: socket");
      continue;
    }
    if ( setsockopt(awsTcpFD, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1 ) {
      perror("setsockopt");
      exit(1);
    }
    if ( bind(awsTcpFD, tcpPtr->ai_addr, tcpPtr->ai_addrlen) == -1 ) {
      close(awsTcpFD);
      perror("server: bind");
      continue;
    }
    break;

  }
  freeaddrinfo(tcpInfo); // all done with this struct.
  /*END:create TCP socket and bind to AWSTCPPORT*/

  /*START:create UDP socket*/
  for (udpPtr = udpInfo; udpPtr != NULL; udpPtr = udpPtr->ai_next) {

    if ( (awsUdpFD = socket(udpPtr->ai_family, udpPtr->ai_socktype, udpPtr->ai_protocol)) == -1 ) {
      perror("talker: socket");
      continue;
    }
    if ( bind(awsUdpFD, udpPtr->ai_addr, udpPtr->ai_addrlen) == -1 ) {
      close(awsUdpFD);
      perror("listener: bind");
      continue;
    }
    break;

  }
  freeaddrinfo(udpInfo); // all done with this struct.
  /*END:create UDP socket*/

  /*START: TCP connection error check*/
  if (tcpPtr == NULL) {
    fprintf(stderr, "tcp aws: failed to bind\n");
    exit(1);
  }
  /*END: TCP connection error check*/

  /*START: UDP connection error check*/
  if (udpPtr == NULL) {
    fprintf(stderr, "udp aws: failed to bind\n");
    exit(1);
  }
  /*END: UDP connection error check*/

  /*START: Begin listening on the tcp port*/
  if ( listen(awsTcpFD, BACKLOG) == -1) {
    perror("listen");
    exit(1);
  }
  /*END: Begin listening on the tcp port*/

  /*START: reaping zombie processes*/
  sa.sa_handler = sigchld_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if ( sigaction(SIGCHLD, &sa, NULL) == -1 ) {
    perror("sigaction");
    exit(1);
  }
  /*END: reaping zombie processes*/

  printf("The AWS is up and running");
  printf("...waiting for connections...\n");
  int state = CTOAWS;

  while (1) { // main accept() loop

    memset(awsBuf, 0, awsBufSize * sizeof(*awsBuf));
    switch (state) {

    case CTOAWS:
      /*START: CLNT -> AWS */
      clntAddrSize = sizeof(clntAddr);
      clientFD = accept(awsTcpFD, (struct sockaddr *)&clntAddr, &clntAddrSize);

      // Use clntAddr output from accept to grab the clnt port #
      inet_ntop(clntAddr.ss_family, get_in_addr((struct sockaddr *)&clntAddr), clntIPAddr, sizeof(clntIPAddr));
      if (clntAddr.ss_family == AF_INET) {
	clntPort = ((struct sockaddr_in *)&clntAddr)->sin_port;
      } else if (clntAddr.ss_family == AF_INET6) {
	clntPort = ((struct sockaddr_in6 *)&clntAddr)->sin6_port;
      } else {
	perror("Client socket type is not IPv4 or IPv6\n");
	exit(1);
      }

      // Child process creation is wrecking the SERVER A communication.
      if ( /*!fork() &&*/ clientFD != -1 ) { // Only create child process if there was an accepted connection
	numRecvBytes = recv(clientFD, awsBuf, awsBufSize, 0);
	if (numRecvBytes == -1) {
	  perror("Error: receiving initial Client Data\n");
	  close(clientFD);
	  exit(0);
	}
	awsBuf[numRecvBytes] = '\0';
	sscanf(awsBuf, "%c %d %d", &mapID, &startNode, &fileSize);
	printf("The AWS has received map ID %c, start vertex %d and file size %d from the client using TCP over port %d\n", mapID, startNode, fileSize, clntPort);

      } // if !fork
      state = (numSendBytes != -1 ? AWSTOSRVRA : state);
      /*END: CLNT -> AWS */
      break;

    case AWSTOSRVRA:
      /*START: AWS-> ServerA*/
      if (srvrAPtr->ai_addr->sa_family == AF_INET) {
	srvrAPort = ((struct sockaddr_in *)srvrAPtr->ai_addr)->sin_port;
      } else if (srvrAPtr->ai_addr->sa_family == AF_INET6) {
	srvrAPort = ((struct sockaddr_in6 *)srvrAPtr->ai_addr)->sin6_port;
      } else {
	perror("srvrA socket type is not IPv4 or IPv6\n");
	exit(1);
      }
      sprintf(awsBuf, "%c %d %d", mapID, startNode, srvrAPort);
      numSendBytes = sendto(awsUdpFD, awsBuf, awsBufSize * sizeof(*awsBuf), 0, srvrAPtr->ai_addr, srvrAPtr->ai_addrlen);
      if (numSendBytes == -1) {
	perror("srvrA sendto failed");
	exit(1);
      }
      printf("The AWS has sent map ID=%c and starting vertex=%d to server A; server A using UDP over port %d\n", mapID, startNode, srvrAPort);
      // Recv propagation and transmission speeds.
      int propagation_speed = 0;
      int transmission_speed = 0;
      memset(awsBuf, 0, awsBufSize * sizeof(*awsBuf));
      numRecvBytes = recvfrom(awsUdpFD, awsBuf, awsBufSize * sizeof(*awsBuf), MSG_WAITALL, srvrAPtr->ai_addr, &srvrAPtr->ai_addrlen);
      if (numRecvBytes == -1) {
	perror("srvrA recvfrom failed");
	exit(1);
      }
      sscanf(awsBuf, "%d %d", &propagation_speed, &transmission_speed);
      printf("The p_speed(km/s) = %d, the t_speed(B/s) = %d\n", propagation_speed, transmission_speed);
      // Recv shortest path calculation.
      memset(awsBuf, 0, awsBufSize * sizeof(*awsBuf));
      numRecvBytes = recvfrom(awsUdpFD, awsBuf, awsBufSize * sizeof(*awsBuf), MSG_WAITALL, srvrAPtr->ai_addr, &srvrAPtr->ai_addrlen);
      if (numRecvBytes == -1) {
	perror("srvrA recvfrom failed");
	exit(1);
      }

      printf("aws: packet is %d bytes long\n", numRecvBytes);
      printf("The AWS has received shortest path from server A:\n");
      for (int i = 0; i < 25; ++i) { printf( (i != 24) ? "-" : "-\n" ); }
      printf("Destination     MinLength\n");
      for (int i = 0; i < 25; ++i) { printf( (i != 24) ? "-" : "-\n" ); }
      printf("%s\n", awsBuf);
      for (int i = 0; i < 25; ++i) { printf( (i != 24) ? "-" : "-\n" ); }

      /*START: parse awsBuf into distances array*/
      int num_distances = 1; // The last line has no '\n'
      for ( int i = 0; i < strlen(awsBuf); ++i ) {
	if (awsBuf[i] == '\n') { ++num_distances; }
      }
      int *newline_locs = (int *)calloc( num_distances, sizeof(*newline_locs) );
      int nl_i = 0;
      for ( int i = 0; i < strlen(awsBuf); ++i ) {
	if (awsBuf[i] == '\n') { newline_locs[++nl_i] = i; } // pre-fix increment to account for string structure.
      }

      int *distances = (int *)calloc(num_distances, sizeof(*distances));
      for ( int i = 0;  i != num_distances; ++i ) {
	sscanf(awsBuf + ( i == 0 ? i : newline_locs[i] ), "%*d %d\n", distances + i);
      }
      /*END: parse awsBuf into distances array*/

      awsBuf[numRecvBytes] = '\0';
      state = (numRecvBytes != -1 ? AWSTOSRVRB : state);
      /*END: AWS-> ServerA*/
      break;

    case AWSTOSRVRB:
      /*START: AWS-> ServerB*/
      if (srvrBPtr->ai_addr->sa_family == AF_INET) {
	srvrBPort = ((struct sockaddr_in *)srvrBPtr->ai_addr)->sin_port;
      } else if (srvrBPtr->ai_addr->sa_family == AF_INET6) {
	srvrBPort = ((struct sockaddr_in6 *)srvrBPtr->ai_addr)->sin6_port;
      } else {
	perror("srvrB socket type is not IPv4 or IPv6\n");
	exit(1);
      }

      // Send Propagation Speed, Transmission Speed, num_distances, and fileSize.
      memset(awsBuf, 0, awsBufSize * sizeof(*awsBuf));
      sprintf(awsBuf, "%d %d %d %d", propagation_speed, transmission_speed, num_distances, fileSize);
      numSendBytes = sendto(awsUdpFD, awsBuf, awsBufSize * sizeof(*awsBuf), 0, srvrBPtr->ai_addr, srvrBPtr->ai_addrlen);
      if (numSendBytes == -1) {
	perror("srvrB sendto failed");
	exit(1);
      }

      // Send shortest path distances.
      memset(awsBuf, 0, awsBufSize * sizeof(*awsBuf));
      for (int i = 0; i < num_distances; ++i) {
	sprintf( awsBuf + strlen(awsBuf), (i != num_distances-1 ? "%d " : "%d"), distances[i] );
      }
      numSendBytes = sendto(awsUdpFD, awsBuf, awsBufSize * sizeof(*awsBuf), 0, srvrBPtr->ai_addr, srvrBPtr->ai_addrlen);
      if (numSendBytes == -1) {
	perror("srvrB sendto failed");
	exit(1);
      }
      printf("The AWS has sent path length, propagation speed and transmission speed to server B using UDP over port %d.\n", srvrBPort);

      double *total_delays = (double *)calloc( num_distances, sizeof(*total_delays) );
      double *propagation_delays = (double *)calloc( num_distances, sizeof(*propagation_delays) );
      double *transmission_delays = (double *)calloc( num_distances, sizeof(*transmission_delays) );
      memset(awsBuf, 0, awsBufSize * sizeof(*awsBuf));
      numRecvBytes = recvfrom(awsUdpFD, awsBuf, awsBufSize * sizeof(*awsBuf), MSG_WAITALL, srvrBPtr->ai_addr, &srvrBPtr->ai_addrlen);
      if (numRecvBytes == -1) {
	perror("srvrB recvfrom failed");
	exit(1);
      }
      nl_i = 0;
      for ( int i = 0; i < strlen(awsBuf); ++i ) {
	if (awsBuf[i] == '\n') { newline_locs[++nl_i] = i; } // pre-fix increment to account for string structure.
      }
      for ( int i = 0;  i != num_distances; ++i ) {
	sscanf(awsBuf + (i == 0 ? i : newline_locs[i] ),
	       "%lf %lf %lf",
	       transmission_delays + i,
	       propagation_delays + i,
	       total_delays + i );
      }
      printf("The AWS has received delays from server B:\n");
      for (int i = 0; i < 54; ++i) { printf( (i != 53) ? "-" : "-\n" ); }
      printf("Destination      TransDelay     PropDelay     TotDelay\n");
      for (int i = 0; i < 54; ++i) { printf( (i != 53) ? "-" : "-\n" ); }
      for (int i = 0; i < num_distances; ++i) {
	printf( "%-16d %-14.3f %-13.3f %-16.3f\n", i, transmission_delays[i], propagation_delays[i], total_delays[i]);
      }
      for (int i = 0; i < 54; ++i) { printf( (i != 53) ? "-" : "-\n" ); }
      awsBuf[numRecvBytes] = '\0';
      state = (numRecvBytes != -1 ? AWSTOC : state);
      /*END: AWS-> ServerB*/
      break;

    case AWSTOC:
      /*START: AWS-> Client*/

      // Send shortest path distances.
      if ( /*!fork() &&*/ clientFD != -1 ) { // Only create child process if there was an accepted connection

	memset(awsBuf, 0, awsBufSize * sizeof(*awsBuf));
	for (int i = 0; i < num_distances; ++i) {
	  sprintf( awsBuf + strlen(awsBuf), (i != num_distances-1 ? "%d " : "%d"), distances[i] );
	}
	numSendBytes = send(clientFD, awsBuf, awsBufSize * sizeof(*awsBuf), 0);
	if (numSendBytes == -1) {
	  perror("Failed to send shortest paths to client!\n");
	  close(clientFD);
	  exit(0);
	}
	
      } // if !fork

      // Send delays.
      if ( /*!fork() &&*/ clientFD != -1 ) { // Only create child process if there was an accepted connection
	
	memset(awsBuf, 0, awsBufSize * sizeof(*awsBuf));
	for (int i = 0; i < num_distances; ++i) {
	  sprintf( awsBuf + strlen(awsBuf),
		   (i != num_distances-1 ? "%.3f %.3f %.3f\n" : "%.3f %.3f %.3f"), // redundant code here
		   transmission_delays[i],
		   propagation_delays[i],
		   total_delays[i] );
	}
	numSendBytes = send(clientFD, awsBuf, awsBufSize * sizeof(*awsBuf), 0);
	if (numSendBytes == -1) {
	  perror("Failed to send delays to client!\n");
	  close(clientFD);
	  exit(0);
	}
	
      } // if !fork
      
      printf("The AWS has sent calculated delay to client using TCP over port = %d\n", clntPort);
      state = (numSendBytes != -1 ? ENDTRANS : state);
      /*END: AWS-> Client*/
      break;

    case ENDTRANS:
    default:
      break;

    } // switch
    if (state == ENDTRANS) {
      break;
    }
  } // while loop

  close(clientFD); // parent doesn't need this.

  return 0;
}
