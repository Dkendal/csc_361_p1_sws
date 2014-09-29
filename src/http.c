#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "http.h"

#define BUFFER_SIZE 1024
#define DELIMITERS " \t"
#define PROTOCOL_VERSION "HTTP/1.0"

#define STATUS_200 PROTOCOL_VERSION " 200 OK"
#define STATUS_400 PROTOCOL_VERSION " 400 Bad Request"
#define STATUS_404 PROTOCOL_VERSION " 404 Not Found"

#define unless(EXP) if(!(EXP))

bool verb_is_supported(char *verb)
{
  if( strcasecmp(verb, "GET") == 0 )
    return true;
  else
    return false;
}

bool protocol_is_supported(char *protocol)
{
  if( strcasecmp(protocol, "HTTP/1.0") == 0 )
    return true;
  else
    return false;
}

bool file_is_readable(char *path)
{
  return true;
}

char* request(char* msg)
{
  char *verb, *path, *protocol, *buffer;

  buffer = strdup(msg);
  assert(buffer != NULL);

  verb = strtok(buffer, DELIMITERS);
  path = strtok(NULL, DELIMITERS);
  protocol = strtok(NULL, DELIMITERS);

  unless(verb_is_supported(verb) &&
      protocol_is_supported(protocol) ) {
    return STATUS_400;
  }

  unless( file_is_readable(path) ) {
    return STATUS_404;
  }

  return STATUS_200;
}
