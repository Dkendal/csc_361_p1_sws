#ifndef HTTP_SEVER_H
#define HTTP_SERVER_H
class Response;
class sockaddr_storage;
class sockaddr_in;

using namespace std;

struct network_info
{
  string ip;
  unsigned short port;
};


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
  bool IsReadyToRead();

  int Start();
  int Init();
private:
  string port;
  string root_dir;
  int sock_fd;
};

string Timestamp();
string StrUntil(const char *, const char *);
struct network_info GetInfo(sockaddr_in *);
void LogRequest(string, sockaddr_storage, Response);
#endif
