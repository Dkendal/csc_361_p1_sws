#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <assert.h>
#include "http.h"
#define BUFF_SIZE 256

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, const char *argv[])
{
  char* port;

  assert(argc == 3);

  port = strdup(argv[1]);

  // set root directory for the server
  http_init( strdup(argv[2]) );

  struct addrinfo hints, *res;
  struct sockaddr_storage their_addr;
  int sock_fd, numbytes;
  socklen_t addr_len;
  char buff[BUFF_SIZE], s[INET6_ADDRSTRLEN];
  char *response;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;

  assert( getaddrinfo(NULL, port, &hints, &res) == 0 );

  sock_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  assert(sock_fd != -1 );

  assert( bind(sock_fd, res->ai_addr, res->ai_addrlen) == 0 );

  freeaddrinfo(res);

  printf("waiting to recvfrom...\n");

  for (;;) {
    addr_len = sizeof their_addr;
    numbytes = recvfrom(sock_fd, buff, BUFF_SIZE -1, 0,
          (struct sockaddr *)&their_addr, &addr_len);
    buff[numbytes] = '\0';

    assert(numbytes != -1);

    printf("listener: got packet from %s\n",
        inet_ntop(their_addr.ss_family,
          get_in_addr((struct sockaddr *)&their_addr),
          s, sizeof s));
    printf("listener: packet is %d bytes long\n", numbytes);

    printf("listener: packet contains \"%s\"\n", buff);

    puts(response = request(buff));
    free(response);
  }

  assert( close(sock_fd) == 0 );
  return 0;
}
