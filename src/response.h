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
  Response();
  Response(string);
};
#endif
