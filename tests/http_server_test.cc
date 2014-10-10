#include <gtest/gtest.h>
#include "../src/http_server.h"

class HttpServerTest : public ::testing::Test
{
  protected:
    virtual void SetUp() { server = new HttpServer("3000", "./www"); }
    virtual void TearDown() { delete server; }
    HttpServer *server;
};
class InitTest : public HttpServerTest { };
class RequestTest : public HttpServerTest { };
class IsResourceReadableTest : public HttpServerTest { };
class IsResourceValidTest : public HttpServerTest { };
class GetTest : public HttpServerTest { };

TEST_F(GetTest, ValidFile)
{
  string body = server->Get("/index.html");
  string expected =
    "Hello, this is the index.html file of your root directory.\n"
    "You should retrieve this file file as default file when the http request "
    "URI has no specific file name.\n";

  EXPECT_EQ(expected, body);
}

TEST_F(InitTest, SetsSockfd)
{
  server->Init();
  EXPECT_TRUE(server->IsSocketOpen());
}

TEST_F(IsResourceValidTest, ValidPath)
{
  EXPECT_TRUE(server->IsResourceValid("/index.html"));
  EXPECT_TRUE(server->IsResourceValid("/"));
}

TEST_F(IsResourceValidTest, InvalidPath)
{
  EXPECT_FALSE(server->IsResourceValid("http.request"));
}
// IsResourceReadableTest
TEST_F(IsResourceReadableTest, ValidPath)
{
  // at the root
  EXPECT_TRUE(server->IsResourceReadable("/index.html"));
  // in a subdir
  EXPECT_TRUE(server->IsResourceReadable("/gnu/main.html"));
  // a dir
  EXPECT_TRUE(server->IsResourceReadable("/gnu/"));
}

TEST_F(IsResourceReadableTest, InvalidPath)
{
  // insufficient permissions
  EXPECT_FALSE(server->IsResourceReadable("/cantRead"));
  // parent directory
  EXPECT_FALSE(server->IsResourceReadable("/../"));
  EXPECT_FALSE(server->IsResourceReadable("/.."));
  // file in parent directory
  EXPECT_FALSE(server->IsResourceReadable("/../Makefile"));
  // non-existant file
  EXPECT_FALSE(server->IsResourceReadable("/some_file.html"));
  // file at root without '/'
  EXPECT_FALSE(server->IsResourceReadable("http.request"));
}
// end IsResourceReadableTest

TEST_F(RequestTest, Index)
{
  Response response = server->Request("GET /index.html HTTP/1.0");
  EXPECT_EQ("HTTP/1.0", response.protocol);
  EXPECT_EQ("HTTP/1.0 200 OK", response.header);
}

TEST_F(RequestTest, Root)
{
  EXPECT_EQ("HTTP/1.0 200 OK",
      server->Request("GET / HTTP/1.0").header);
}

TEST_F(RequestTest, NestedResource)
{
  EXPECT_EQ("HTTP/1.0 200 OK",
      server->Request("GET /gnu/main.html HTTP/1.0").header);
}

TEST_F(RequestTest, VerbAndProtocolCaseInsensitive)
{
  EXPECT_EQ("HTTP/1.0 200 OK",
      server->Request("get / http/1.0").header);
}

// nonexistant file
TEST_F(RequestTest, NonexistantFile)
{
  EXPECT_EQ("HTTP/1.0 404 Not Found",
      server->Request("GET /nofile HTTP/1.0").header);
  EXPECT_EQ("HTTP/1.0 404 Not Found",
      server->Request("GET /gnu/nofile HTTP/1.0").header);
}
// case sensitive uri
TEST_F(RequestTest, ResourceCaseSensitive)
{
  EXPECT_EQ("HTTP/1.0 404 Not Found",
      server->Request("GET /Index.html HTTP/1.0").header);
}
// unreadable file
TEST_F(RequestTest, UnreadableFile)
{
  EXPECT_EQ("HTTP/1.0 404 Not Found",
      server->Request("GET /cantRead HTTP/1.0").header);
}
// parent dir
TEST_F(RequestTest, ParentDirectory)
{
  EXPECT_EQ("HTTP/1.0 404 Not Found",
      server->Request("GET /../ HTTP/1.0").header);
}
// unrecognized protocol
TEST_F(RequestTest, InvalidProtocol)
{
  EXPECT_EQ("HTTP/1.0 400 Bad Request",
      server->Request("GET /index.html HTTP/1.1").header);
  EXPECT_EQ("HTTP/1.0 400 Bad Request",
      server->Request("GET /gnu/ HTTP/1.1").header);
}
// unrecognized verb
TEST_F(RequestTest, UnknownVerb)
{
  EXPECT_EQ("HTTP/1.0 400 Bad Request",
      server->Request("got / http/1.0").header);
}
// not well formed uri
TEST_F(RequestTest, InvalidURI)
{
  EXPECT_EQ("HTTP/1.0 400 Bad Request",
      server->Request("GET http.request HTTP/1.0").header);
}
