#include <string>
#include <cstring>
#include "response.h"
#include <iostream>
#include <algorithm>

using namespace std;

Response::Response() {}

Response::Response(string request)
{
  int str_size = request.size();
  char *req_temp = new char[str_size + 1];
  const char *delimiters = " \n";

  request.copy(req_temp, str_size);
  req_temp[str_size] = '\0';

  const char *tok = strtok(req_temp, delimiters);

  for (int i=0; i < 3; i++) {
    if (tok == NULL) {
      status = 400;
      header = "HTTP/1.0 400 Bad Request";
      break;
    }

    switch (i) {
      case 0:
        verb = tok;
        break;
      case 1:
        resource = tok;
        if (resource.compare("/") == 0) {
          resource = "/index.html";
        }
        break;
      case 2:
        protocol = tok;
        break;
    }

    tok = strtok(NULL, delimiters);
  }

  delete[] req_temp;

  transform(verb.begin(), verb.end(), verb.begin(), ::toupper);
  transform(protocol.begin(), protocol.end(), protocol.begin(), ::toupper);
}
