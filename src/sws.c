#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MYPORT "3000"

#define BUFF_SIZE 256

#define check(EXP) \
  if(( EXP ) == -1) { \
    perror("error: " #EXP); \
    exit(EXIT_FAILURE); \
  }

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void) {
  struct addrinfo hints, *res;
  struct sockaddr_storage their_addr;
  int sock_fd, numbytes;
  socklen_t addr_len;
  char buff[BUFF_SIZE];
  char s[INET6_ADDRSTRLEN];

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;

  check( getaddrinfo(NULL, MYPORT, &hints, &res) );

  check( sock_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol) );

  check( bind(sock_fd, res->ai_addr, res->ai_addrlen) );

  freeaddrinfo(res);

  printf("waiting to recvfrom...\n");

  addr_len = sizeof their_addr;
  check( numbytes = recvfrom(sock_fd, buff, BUFF_SIZE -1, 0,
        (struct sockaddr *)&their_addr, &addr_len) );

  printf("listener: got packet from %s\n",
      inet_ntop(their_addr.ss_family,
        get_in_addr((struct sockaddr *)&their_addr),
        s, sizeof s));
  printf("listener: packet is %d bytes long\n", numbytes);
  buff[numbytes] = '\0';
  printf("listener: packet contains \"%s\"\n", buff);

  check( close(sock_fd) );
  return 0;
}
