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


void set_sock_preferences (struct addrinfo *sock_preferences);

void check_number_of_args (int argc);

void *get_in_addr (struct sockaddr *sa);

void create_sock_and_connect (int clnt_sock_fd,
			      struct addrinfo *poss_cnntns);

void send_msg (int clnt_sock_fd,
	       int msg_len,
	       char **argv);
