#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "http.h"

#define BUFFER_SIZE 1024
#define DELIMITERS " \t"
#define TRUE 1
#define FALSE 0

#define unless(EXP) if(!(EXP))

int verb_is_supported(char *verb)
{
  return TRUE;
}

int protocol_is_supported(char *protocol)
{
  return FALSE;
}

int file_is_readable(char *path)
{
  return FALSE;
}

char* request(char* msg)
{
  char *verb, *path, *protocol, *buffer;
  buffer = strdup(msg);
  assert(buffer != NULL);

  //split tokens
  verb = strtok(buffer, DELIMITERS);
  path = strtok(NULL, DELIMITERS);
  protocol = strtok(NULL, DELIMITERS);

  //verb directory protocol
  unless(verb_is_supported(verb) == TRUE &&
      protocol_is_supported(protocol) == TRUE) {
    // return  400
  }

  unless( file_is_readable(path) == TRUE ) {
    // return 404
  }

  //return 200

  return "";
}
