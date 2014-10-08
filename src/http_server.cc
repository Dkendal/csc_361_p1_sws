#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
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

bool HttpServer::IsVerbValid(string verb)
{
  return strcasecmp(verb.c_str(), "get") == 0;
}

bool HttpServer::IsResourceValid(string resource)
{
  if (resource.compare("/") == 0) {
    resource = "/index.html";
  }

  string uri = this->root_dir + resource;

  // this is very naive, will report false positive if filename is prefixed with ..
  if ( resource.find("/..") != string::npos ) {
    return false;
  }

  struct stat file_stats;
  if (stat(uri.c_str(), &file_stats) != 0 || // file doesn't exist
      file_stats.st_mode & S_IFDIR) {        // it's a directory
    return false;
  }

  ifstream file(uri.c_str());
  return file.good();
}

Response HttpServer::Request(string request_str)
{
  Response my_response;
  {
    int str_size = request_str.size();
    char *req_temp = new char[str_size + 1];

    request_str.copy(req_temp, str_size);
    req_temp[str_size + 1] = 0;

    const char *tok = strtok(req_temp, " ");
    my_response.verb = tok;

    tok = strtok(NULL, " ");
    my_response.resource = tok;

    tok = strtok(NULL, " ");
    my_response.protocol = tok;
    delete req_temp;
  }


  if (strcasecmp(my_response.protocol.c_str(), "HTTP/1.0") != 0) {
    my_response.header = STATUS_400;
  }
  if (IsVerbValid(my_response.verb)) {
    if(IsResourceValid(my_response.resource)) {
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
