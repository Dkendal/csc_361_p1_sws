#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <netdb.h>
#include <string>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "response.h"
#include "http_server.h"

#define PROTOCOL_VERSION "HTTP/1.0"
#define STATUS_200 PROTOCOL_VERSION " 200 OK"
#define STATUS_400 PROTOCOL_VERSION " 400 Bad Request"
#define STATUS_404 PROTOCOL_VERSION " 404 Not Found"
#define BUFF_SIZE 256

using namespace std;

string Timestamp()
{
  time_t unixtime = time(NULL);
  struct tm *time_s = localtime(&unixtime);
  const int size = 13;
  char tmp[size];

  // MMM DD HH:MM
  string pattern = "%b %d %H:%M";

  tmp[size] = '\0';

  strftime(tmp, size, pattern.c_str(), time_s);

  string out = tmp;

  return out;
}

string StrUntil(const char *str, const char *delimiters)
{
  size_t pos = strcspn(str, delimiters);
  string out = str;
  out.resize(pos, 0); // truncate string at first match
  return out;
}

struct network_info GetInfo(sockaddr_in *addr)
{
  const struct in_addr *addr_bin = &addr->sin_addr;
  const unsigned short sending_port = addr->sin_port;
  char ip_addr[INET6_ADDRSTRLEN];

  inet_ntop(AF_INET, addr_bin, ip_addr, INET6_ADDRSTRLEN);

  struct network_info out;
  out.ip = ip_addr;
  out.port = sending_port;
  return out;
}

void LogRequest(string request, sockaddr_storage response_addr, Response response)
{
  struct network_info info = GetInfo((struct sockaddr_in *) &response_addr);
  //MMM DD HH:MM:SS Client-IP:Client-Port request-line; response-line; [filename]
  std::cout << Timestamp() << " " << info.ip << ":" << info.port << " ";
  std::cout << request << "; " <<  response.header << "; ";
  if (response.status == 200)
  {
    std::cout << response.resource;
  }
  std::cout << std::endl;
}

HttpServer::HttpServer(string port, string root_dir) :
  port(port), root_dir(root_dir)
{
  sock_fd = -1;
  std::cout << "sws is running on UPT port " << port << " and serving " << root_dir << std::endl;
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
  string out;
  const char *uri = (root_dir + resource).c_str();
  ifstream file(uri);

  if (file.is_open()) {
    string line;
    while (getline(file, line)) {
      out += line + "\n";
    }
    file.close();
  }
  else {
    std::cout << "couldn't open file" << std::endl;
  }
  return out;
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
  string uri = root_dir + resource;
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
  return resource[0] == '/';
}

Response HttpServer::Request(string request_str)
{
  Response response(request_str);

  if (!IsVerbValid(response.verb) ||
      !IsResourceValid(response.resource) ||
      !IsProtocolValid(response.protocol)) {
    response.header = STATUS_400;
    response.status = 400;
  }
  else {
    if (IsResourceReadable(response.resource)) {
      response.header = STATUS_200;
      response.status = 200;
      if (response.verb == "GET") {
        response.body = Get(response.resource);
      }
      else {
        // shouldn't ever get here
        response.header = STATUS_400;
        response.status = 400;
      }
    }
    else {
      response.header = STATUS_404;
      response.status = 404;
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

  int optval = 1;
  setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

  if ( bind(sock_fd, res->ai_addr, res->ai_addrlen) != 0) {
    perror("bind: ");
    exit(EXIT_FAILURE);
  }

  freeaddrinfo(res);

  return 0;
}

bool HttpServer::IsReadyToRead()
{
  for (;;) {
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    FD_SET(sock_fd, &readfds);

    int retval = select(sock_fd+1, &readfds, NULL, NULL, NULL);

    if(FD_ISSET(sock_fd, &readfds)) {
      return true;
    }

    if(retval <=0) { //error or timeout
      perror("IsReadyToRead: ");
      return false;
    }

    if(FD_ISSET(STDIN_FILENO, &readfds)) {
      string input;
      std::cin >> input;
      if (input == "q") {
        return false;
      }
    }
  }
  return false;
}

int HttpServer::Start()
{
  std::cout << "press 'q' to quit ..." << std::endl;
  socklen_t addr_len;
  struct sockaddr_storage their_addr;
  char buffer[BUFF_SIZE];
  addr_len = sizeof their_addr;

  std::cout << "waiting to recvfrom" << std::endl;

  for (;;) {

    if(!IsReadyToRead()) {
      std::cout << "shutting down" << std::endl;
      return EXIT_SUCCESS;
    }

    ssize_t numbytes = recvfrom(sock_fd,
        buffer,
        BUFF_SIZE -1,
        0,
        (struct sockaddr *)&their_addr,
        &addr_len);

    if (numbytes == -1) {
      perror("recvfrom: ");
      exit(EXIT_FAILURE);
    }

    buffer[numbytes] = '\0';

    string request = StrUntil(buffer, "\n\r");

    Response response = Request(request);

    LogRequest(request, their_addr, response);

    string msg_s = response.header + "\r\n\r\n" + response.body;

    numbytes = sendto(sock_fd,
        msg_s.c_str(),
        msg_s.size(),
        0,
        (const struct sockaddr *) &their_addr,
        addr_len);

    if (numbytes == -1) {
      perror("sendto: ");
      exit(EXIT_FAILURE);
    }
  }

  return 0;
}
