#include <string>
#include "http_server.h"

#define STATUS_200 PROTOCOL_VERSION " 200 OK\n\n"
#define STATUS_400 PROTOCOL_VERSION " 400 Bad Request\n\n"
#define STATUS_404 PROTOCOL_VERSION " 404 Not Found\n\n"


HttpServer::HttpServer(const std::string root_dir) : root_dir(root_dir) {}
HttpServer::~HttpServer() {};

std::string HttpServer::Get(std::string request)
{
  return "";
}

std::string HttpServer::Request(std::string request)
{
  return "";
}

