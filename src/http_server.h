#include <string>

#ifndef HTTP_SEVER_H
#define HTTP_SERVER_H
class HttpServer
{
public:
  HttpServer(const std::string);
  ~HttpServer();
  std::string Get(std::string);
  std::string Request(std::string);

private:
  std::string root_dir;
  /* data */
};
#endif
