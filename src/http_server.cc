#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "response.h"
#include "http_server.h"

#define PROTOCOL_VERSION "HTTP/1.0"
#define STATUS_200 PROTOCOL_VERSION " 200 OK"
#define STATUS_400 PROTOCOL_VERSION " 400 Bad Request"
#define STATUS_404 PROTOCOL_VERSION " 404 Not Found"

using namespace std;

HttpServer::HttpServer(const string root_dir) : root_dir(root_dir) {}
HttpServer::~HttpServer() {}

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

  // this is very naive, will report false positive if filename is prefixed with ..
  if ( resource.find("/..") != string::npos ) {
    return false;
  }

  ifstream file(uri.c_str());
  return file.good();
}

bool HttpServer::IsResourceValid(string resource)
{
  return true;
}

Response HttpServer::Request(string request_str)
{
  Response my_response(request_str);

  if (!IsProtocolValid(my_response.protocol)) {
    my_response.header = STATUS_400;
  }
  else if (IsVerbValid(my_response.verb)) {
    if (IsResourceReadable(my_response.resource)) {
      my_response.header = STATUS_200;
    }
    else {
      my_response.header = STATUS_404;
    }
  }
  else {
    my_response.header = STATUS_400;
  }

  return my_response;
}
