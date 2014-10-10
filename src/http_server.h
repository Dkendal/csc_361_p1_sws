#ifndef HTTP_SEVER_H
#define HTTP_SERVER_H

#include "response.h"

using namespace std;

class HttpServer
{
public:
  HttpServer(string, string);
  ~HttpServer();
  string Get(string);
  Response Request(string);

  bool IsVerbValid(string);
  bool IsResourceValid(string);
  bool IsResourceReadable(string);
  bool IsProtocolValid(string);
  bool IsSocketOpen();

  int Start();
  int Init();
private:
  string port;
  string root_dir;
  int sock_fd;
};
#endif
