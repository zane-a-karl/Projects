#include "client-utils.h"

/** Set the hints variable.
 */
void set_sock_preferences (struct addrinfo *sock_preferences) {
  memset(sock_preferences, 0, sizeof *sock_preferences);
  sock_preferences->ai_family = AF_UNSPEC;
  sock_preferences->ai_socktype = SOCK_STREAM;
}

/** Err if the client prog doesn't have the correct number of inputs.
 */
void check_number_of_args (int argc) {

  if (argc < 2) {
    fprintf(stderr,"The client program takes input!\n");
    exit(1);
  }
}

/** Get the sockaddr, IPv4 or IPv6:
 */
void *get_in_addr (struct sockaddr *sa) {

  if ( sa->sa_family == AF_INET ) {
    return &( ((struct sockaddr_in*)sa)->sin_addr );
  }
  return &( ((struct sockaddr_in6*)sa)->sin6_addr );
}

/** Try to create a socket() for each possible connection
 * returned from getaddrinfo, and connect() to the first one
 * that is doesn't fail.
 */
void create_sock_and_connect (int clnt_sock_fd,
			      struct addrinfo *poss_cnntns) {
  int cnntn_status;
  struct addrinfo *cnntn;
  for ( cnntn = poss_cnntns;
	cnntn != NULL;
	cnntn = cnntn->ai_next ) {

    clnt_sock_fd = socket(cnntn->ai_family,
			  cnntn->ai_socktype,
			  cnntn->ai_protocol);
    if ( clnt_sock_fd == -1 ) {
      perror("client: socket creation failed.");
      continue;
    }
    // Connect to 1st poss result from poss_cnntns.
    cnntn_status = connect(clnt_sock_fd,
			   cnntn->ai_addr,
			   cnntn->ai_addrlen);
    if ( cnntn_status == -1 ) {
      close(clnt_sock_fd);
      perror("client: connection through socket failed.");
      continue;
    }
    break;
  }
  if ( cnntn == NULL ) {
    fprintf(stderr, "client: failed to connect\n");
    exit(2);
  }
}

/** Calls send() to push msg = argv[1] argv[2] argv[3]  of
 * length msg_len out the client socket.
 */
void send_msg (int clnt_sock_fd,
	       int msg_len,
	       char **argv) {
  char *msg = (char *)calloc(msg_len, sizeof(*msg));
  sprintf(msg, "%s %s %s", argv[1], argv[2], argv[3]);
  if ( send(clnt_sock_fd, msg, msg_len, 0) == -1 ) {
    perror("Failed to send intial client msg to AWS!\n");
    close(clnt_sock_fd);
    exit(1);
  }
  free(msg);
}
