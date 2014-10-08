#include <string>

#ifndef HTTP_SEVER_H
#define HTTP_SERVER_H
using namespace std;

struct Response {
  string verb,
         resource,
         protocol,
         header,
         body;
};

class HttpServer
{
public:
  HttpServer(const string);
  ~HttpServer();
  string Get(string);
  Response Request(string);

  bool IsVerbValid(string);
  bool IsResourceValid(string);

private:
  string root_dir;
  /* data */
};
#endif
