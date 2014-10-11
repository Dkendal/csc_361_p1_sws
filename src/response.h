#ifndef RESPONSE_H
#define RESPONSE_H
using namespace std;

struct Response
{
  string verb,
         resource,
         protocol,
         header,
         body;
  short status;

  Response();
  Response(string);
};
#endif
