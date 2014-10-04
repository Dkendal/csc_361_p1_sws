#include <string>
#include <strings.h>
#include "request.h"

Request::Request(std::string verb, std::string resource, std::string protocol)
{
  this->verb = verb;
  this->resource = resource;
  this->protocol = protocol;
}

bool Request::IsVerbValid()
{
  return strcasecmp(verb.c_str(), "GET") == 0;
}

bool Request::IsValid()
{
  return IsVerbValid() &&
    resource == "/" &&
    protocol == "HTTP/1.0";
}
