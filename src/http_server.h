#ifndef HTTP_SEVER_H
#define HTTP_SERVER_H

using namespace std;
#include "response.h"

class HttpServer
{
public:
  HttpServer(const string);
  ~HttpServer();
  string Get(string);
  Response Request(string);

  bool IsVerbValid(string);
  bool IsResourceValid(string);
  bool IsResourceReadable(string);
  bool IsProtocolValid(string);

private:
  string root_dir;
  /* data */
};
#endif
