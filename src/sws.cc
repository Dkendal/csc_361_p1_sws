#include <string>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cstdio>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "http.h"

#define ARGS_MSG \
  "Arguments: ( port to listen on ) ( directory to mount as root )\n"

#define BUFF_SIZE 256

using namespace std;

int main(int argc, const char *argv[])
{
  string port, root_dir;
  int sock_fd, numbytes;
  socklen_t addr_len;
  struct addrinfo hints, *res;
  struct sockaddr_storage their_addr;

  char buff[BUFF_SIZE], s[INET6_ADDRSTRLEN];
  char *response;

  if (argc < 3) {
    cout << ARGS_MSG;
    exit(EXIT_FAILURE);
  }

  port = argv[1];
  root_dir = argv[2];

  // set root directory for the server
  //http_init( strdup(argv[2]) );

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;

  assert( getaddrinfo(NULL, port.c_str(), &hints, &res) == 0 );

  sock_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  assert(sock_fd != -1 );

  assert( bind(sock_fd, res->ai_addr, res->ai_addrlen) == 0 );

  freeaddrinfo(res);

  cout << "waiting to recvfrom...\n";

  for (;;) {
    addr_len = sizeof their_addr;
    numbytes = recvfrom(sock_fd, buff, BUFF_SIZE -1, 0,
          (struct sockaddr *)&their_addr, &addr_len);
    buff[numbytes] = '\0';

    assert(numbytes != -1);
    puts(response = request(buff));
    free(response);
  }

  assert( close(sock_fd) == 0 );
  return 0;
}
