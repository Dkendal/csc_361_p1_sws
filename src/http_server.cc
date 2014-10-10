#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdlib>
#include "response.h"
#include "http_server.h"


#define PROTOCOL_VERSION "HTTP/1.0"
#define STATUS_200 PROTOCOL_VERSION " 200 OK"
#define STATUS_400 PROTOCOL_VERSION " 400 Bad Request"
#define STATUS_404 PROTOCOL_VERSION " 404 Not Found"

using namespace std;

HttpServer::HttpServer(string port, string root_dir) :
 port(port), root_dir(root_dir)
{
  sock_fd = -1;
}

HttpServer::~HttpServer() {
  if (IsSocketOpen()) { close(sock_fd); }
}

bool HttpServer::IsSocketOpen()
{
  return sock_fd != 1;
}

string HttpServer::Get(string resource)
{
  return "";
}

bool HttpServer::IsProtocolValid(string protocol)
{
  return strcasecmp(protocol.c_str(), "HTTP/1.0") == 0;
}

bool HttpServer::IsVerbValid(string verb)
{
  return strcasecmp(verb.c_str(), "get") == 0;
}

bool HttpServer::IsResourceReadable(string resource)
{
  string uri = this->root_dir + resource;
  const char *uri_c = uri.c_str();

  // this is very naive, will report false positive if filename is prefixed with ..
  if ( resource.find("/..") != string::npos ) {
    return false;
  }

  ifstream file(uri_c);
  return file.good();
}

bool HttpServer::IsResourceValid(string resource)
{
  const char *res = resource.c_str();
  return res[0] == '/';
}

Response HttpServer::Request(string request_str)
{
  Response response(request_str);

  if (!IsVerbValid(response.verb) ||
      !IsResourceValid(response.resource) ||
      !IsProtocolValid(response.protocol)) {
    response.header = STATUS_400;
  }
  else {
    if (IsResourceReadable(response.resource)) {
      response.header = STATUS_200;
    }
    else {
      response.header = STATUS_404;
    }
  }

  return response;
}

int HttpServer::Init()
{
  struct addrinfo hints, *res;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;

  if (getaddrinfo(NULL, port.c_str(), &hints, &res) != 0)
  {
    perror("getaddrinfo: ");
    exit(EXIT_FAILURE);
  }

  sock_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

  if (sock_fd == -1)
  {
    perror("socket: ");
    exit(EXIT_FAILURE);
  }

  if ( bind(sock_fd, res->ai_addr, res->ai_addrlen) != 0) {
    perror("bind: ");
    exit(EXIT_FAILURE);
  }

  freeaddrinfo(res);

  return 0;
}

#define BUFF_SIZE 256

int HttpServer::Start()
{
  socklen_t addr_len;
  struct sockaddr_storage their_addr;
  char buff[BUFF_SIZE];
  addr_len = sizeof their_addr;

  std::cout << "waiting to recvfrom" << std::endl;

  for (;;) {
    int numbytes = recvfrom(sock_fd, buff, BUFF_SIZE -1, 0,
          (struct sockaddr *)&their_addr, &addr_len);
    buff[numbytes] = '\0';

    Response response = Request(buff);

    string msg_s = response.header + "\r\n" + response.body + "\r\n";

    if (numbytes == -1) {
      perror("recvfrom: ");
      exit(EXIT_FAILURE);
    }

    ssize_t reti = sendto(sock_fd,
        msg_s.c_str(),
        msg_s.size(),
        0,
        (const struct sockaddr *) &their_addr,
        addr_len);

    if (reti == -1) {
      perror("sendto: ");
      exit(EXIT_FAILURE);
    }
  }

  return 0;
}
