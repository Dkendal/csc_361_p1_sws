#include <string>
#include <cstring>
#include "response.h"
#include <iostream>

using namespace std;

Response::Response() {}

Response::Response(string request)
{
  int str_size = request.size();
  char *req_temp = new char[str_size + 1];

  request.copy(req_temp, str_size);
  req_temp[str_size] = 0;

  const char *tok = strtok(req_temp, " ");
  verb = tok;

  tok = strtok(NULL, " ");

  resource = tok;

  if (resource.compare("/") == 0) {
    resource = "/index.html";
  }

  tok = strtok(NULL, " ");
  protocol = tok;

  delete req_temp;
}
