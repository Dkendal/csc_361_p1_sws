#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "http.h"

#define BUFFER_SIZE 1024
#define DELIMITERS " \t\r\n"
#define PROTOCOL_VERSION "HTTP/1.0"

#define STATUS_200 PROTOCOL_VERSION " 200 OK\n\n"
#define STATUS_400 PROTOCOL_VERSION " 400 Bad Request\n\n"
#define STATUS_404 PROTOCOL_VERSION " 404 Not Found\n\n"

#define unless(EXP) if(!(EXP))

static char *root_dir;

void http_cleanup(void)
{
  free(root_dir);
}

void http_init(char *path)
{
  root_dir = strdup(path);
  assert(atexit(http_cleanup) == 0);
}

bool verb_is_supported(char *verb)
{
  if( strcasecmp(verb, "GET") == 0 ) {
    return true;
  }
  else {
    return false;
  }
}

bool protocol_is_supported(char *protocol)
{
  if( strcasecmp(protocol, "HTTP/1.0") == 0 ) {
    return true;
  }
  else {
    return false;
  }
}

bool resource_uri_valid(char *uri)
{
  // the only validation I'm going to do is check if it's an absolute path
  if(uri[0] == '/') {
    return true;
  }
  else {
    return false;
  }
}

char * get(char *path)
{
  char *rel_path, *resource_contents;
  FILE *resource;
  int string_length;
  long resource_size;

  if (strcmp(path, "/") == 0) {
    path = "/index.html";
  }

  //check that path is not in parent
  unless( strstr(path, "/../") == NULL ) {
    return NULL;
  }

  // prepend the root directory to the target file path
  string_length = strlen(path) + strlen(root_dir) + 1;
  rel_path = malloc( sizeof(char) * string_length );
  assert(rel_path != NULL);

  strcpy(rel_path, root_dir);
  strncat(rel_path, path, string_length);

  resource = fopen(rel_path, "r");

  free(rel_path);

  if(resource == NULL) return NULL;

  fseek(resource, 0, SEEK_END);
  resource_size = ftell(resource);
  rewind(resource);
  resource_contents = malloc(resource_size * (sizeof(char)));
  fread(resource_contents, sizeof(char), resource_size, resource);

  fclose(resource);

  return resource_contents;
}

char* request(char* msg)
{
  char *verb, *resource, *protocol, *buffer, *resource_contents;

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

  resource_contents = get(resource);

  free(buffer);

  if(resource_contents == NULL) {
    return STATUS_404;
  }

  int string_length = 1 + strlen(resource_contents) + strlen(STATUS_200);

  char *response = malloc(sizeof(char) * string_length);
  assert(response != NULL);

  strncpy(response, STATUS_200, string_length);
  strncat(response, resource_contents, string_length);
  response[string_length] = '\0';

  free(resource_contents);

  return response;
}
