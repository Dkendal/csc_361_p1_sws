#include <string>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include "http_server.h"

#define ARGS_MSG \
  "Arguments: ( port to listen on ) ( directory to mount as root )\n"


using namespace std;

int main(int argc, const char *argv[])
{
  if (argc < 3) {
    cout << ARGS_MSG << endl;
    exit(EXIT_FAILURE);
  }

  HttpServer server(argv[1], argv[2]);
  server.Init();

  return server.Start();
}
