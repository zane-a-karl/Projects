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

#define PORT "3490" // The port users will be connecting to.

#define BACKLOG 10

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
  int sockfd, new_fd; // listen on sockfd and new connection on new_fd.
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_storage their_addr; //connector's addr info
  socklen_t sin_size;
  struct sigaction sa;
  int yes = 1;
  char *s = (char *)calloc(INET6_ADDRSTRLEN, sizeof(*s));
  int rv;
  int buf_len = 2048;
  char *buf = (char *)calloc(buf_len, sizeof *buf);

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; // Use my IP.
  
  if ( (rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0 ) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  // Loop through all the results and bind to the first one we can.
  for (p = servinfo; p != NULL; p = p->ai_next) {
    if ( (sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      perror("server: socket");
      continue;
    }

    if ( setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1 ) {
      perror("setsockopt");
      exit(1);
    }

    if ( bind(sockfd, p->ai_addr, p->ai_addrlen) == -1 ) {
      close(sockfd);
      perror("server: bind");
      continue;
    }
    
    break;
  }

  freeaddrinfo(servinfo); // all done with this struct.

  if (p == NULL) {
    fprintf(stderr, "server: failed to bind\n");
    exit(1);
  }

  if ( listen(sockfd, BACKLOG) == -1) {
    perror("listen");
    exit(1);
  }

  sa.sa_handler = sigchld_handler; // reap all dead processes
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if ( sigaction(SIGCHLD, &sa, NULL) == -1 ) {
    perror("sigaction");
    exit(1);
  }

  printf("server: waiting for connections...\n");
  while(1) { // main accept() loop

    memset(buf, 0, buf_len * sizeof(*buf));
    
    sin_size = sizeof (their_addr);
    if ( (new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size) ) == -1 ) {
      //      perror("accept");
      //      printf("... ");
      continue;
    }

    inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
    printf("server: got connection from %s\n", s);

    if ( !fork() ) { // this is the child process
      close(sockfd); // child doesn't need the listener

      int num_recv_bytes = recv(new_fd, buf, buf_len, 0);
      if (num_recv_bytes == -1) {
	perror("Error: receiving initial Client Data\n");
	close(new_fd);
	exit(0);
      }
      buf[num_recv_bytes] = '\0';
      fprintf(stderr, "We received %s\n", buf);

      memset(buf, 0, buf_len * sizeof(*buf));

      num_recv_bytes = recv(new_fd, buf, buf_len, 0);
      if (num_recv_bytes == -1) {
	perror("Error: receiving initial Client Data\n");
	close(new_fd);
	exit(0);
      }
      buf[num_recv_bytes] = '\0';
      fprintf(stderr, "We received %s\n", buf);
    }
    close(new_fd); // parent doesn't need this.
  }

  return 0;
}
