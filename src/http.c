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

char *dir_root = "../www";

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

bool resource_uri_valid(char *uri)
{
  // the only validation I'm going to do is check if it's an absolute path
  return uri[0] == '/';
}

FILE * get(char *path)
{
  char *rel_path;
  FILE *document;
  int string_length;

  if (strcmp(path, "/") == 0) {
    path = "/index.html";
  }

  //check that path is not in parent
  unless( strstr(path, "/../") == NULL ) {
    return NULL;
  }

  // prepend the root directory to the target file path
  string_length= strlen(path) + strlen(dir_root) + 1;
  rel_path = malloc( sizeof(char) * string_length );
  assert(rel_path != NULL);

  strcpy(rel_path, dir_root);
  strcat(rel_path, path);

  puts(rel_path);
  return fopen(rel_path, "r");
}

char* request(char* msg)
{
  char *verb, *resource, *protocol, *buffer;
  FILE *document;

  buffer = strdup(msg);
  assert(buffer != NULL);

  verb = strtok(buffer, DELIMITERS);
  resource = strtok(NULL, DELIMITERS);
  protocol = strtok(NULL, DELIMITERS);

  unless(verb_is_supported(verb) &&
      resource_uri_valid(resource) &&
      protocol_is_supported(protocol) ) {
    return STATUS_400;
  }

  document = get(resource);

  if( document ==  NULL ) {
    return STATUS_404;
  }

  fclose(document);

  return STATUS_200;
}
