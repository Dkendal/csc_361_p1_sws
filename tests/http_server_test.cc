#include <gtest/gtest.h>
#include "../src/http_server.cc"

class HttpServerTest : public ::testing::Test
{
  protected:
    virtual void SetUp() { server = new HttpServer("./www"); }
    virtual void TearDown() { delete server; }
    HttpServer *server;
};
class RequestTest : public HttpServerTest { };
class IsResourceReadableTest : public HttpServerTest { };

// IsResourceReadableTest
TEST_F(IsResourceReadableTest, ValidPath)
{
  // at the root
  EXPECT_TRUE(server->IsResourceReadable("/index.html"));
  // in a subdir
  EXPECT_TRUE(server->IsResourceReadable("/gnu/main.html"));
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
  // a directory
  EXPECT_FALSE(server->IsResourceReadable("/gnu/"));
}
// end IsResourceReadableTest

TEST_F(RequestTest, Index)
{
  EXPECT_EQ("HTTP/1.0 200 OK",
      server->Request("GET /index.html HTTP/1.0").header);
}

TEST_F(RequestTest, Root)
{
  Response result = server->Request("GET / HTTP/1.0");

  EXPECT_EQ("HTTP/1.0 200 OK", result.header);
}

TEST_F(RequestTest, NestedResource)
{
  Response result = server->Request("GET /gnu/main.html HTTP/1.0");

  EXPECT_EQ("HTTP/1.0 200 OK", result.header);
}

TEST_F(RequestTest, NonexistantFile)
{
  EXPECT_EQ("HTTP/1.0 404 Not Found", server->Request("GET /nofile HTTP/1.0").header);
}

TEST_F(RequestTest, NonexistantNestedFile)
{
  EXPECT_EQ("HTTP/1.0 404 Not Found",
      server->Request("GET /gnu/nofile HTTP/1.0").header);
}
TEST_F(RequestTest, VerbAndProtocolCaseInsensitive)
{
  EXPECT_EQ("HTTP/1.0 200 OK",
   server->Request("get / http/1.0").header);
}

TEST_F(RequestTest, ResourceCaseSensitive)
{
  EXPECT_EQ("HTTP/1.0 404 Not Found",
      server->Request("GET /Index.html HTTP/1.0").header);
}

TEST_F(RequestTest, InvalidURI)
{
  EXPECT_EQ("HTTP/1.0 400 Bad Request",
      server->Request("GET http.request HTTP/1.0").header);
}

TEST_F(RequestTest, UnreadableFile)
{
  EXPECT_EQ("HTTP/1.0 404 Not Found",
      server->Request("GET /cantRead HTTP/1.0").header);
}

TEST_F(RequestTest, InvalidProtocol)
{
  EXPECT_EQ("HTTP/1.0 400 Bad Request",
      server->Request("GET /index.html HTTP/1.1").header);
}

TEST_F(RequestTest, CantReadDirectory)
{
  EXPECT_EQ("HTTP/1.0 400 Bad Request",
      server->Request("GET /gnu/ HTTP/1.1").header);
}

TEST_F(RequestTest, UnknownVerb)
{
  EXPECT_EQ("HTTP/1.0 400 Bad Request",
      server->Request("got / http/1.0").header);
}

TEST_F(RequestTest, ParentDirectory)
{
  EXPECT_EQ("HTTP/1.0 404 Not Found",
      server->Request("GET /../ HTTP/1.0").header);
}
