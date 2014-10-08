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
class IsResourceValidTest : public HttpServerTest { };

// IsResourceValidTest
TEST_F(IsResourceValidTest, ValidFile)
{
  EXPECT_TRUE(server->IsResourceValid("/index.html"));
  EXPECT_TRUE(server->IsResourceValid("/"));
  EXPECT_TRUE(server->IsResourceValid("/gnu/main.html"));
}

TEST_F(IsResourceValidTest, InvalidFile)
{
  EXPECT_FALSE(server->IsResourceValid("/cantRead"));
  EXPECT_FALSE(server->IsResourceValid("/../"));
  EXPECT_FALSE(server->IsResourceValid("/.."));
  EXPECT_FALSE(server->IsResourceValid("/some_file.html"));
  EXPECT_FALSE(server->IsResourceValid("http.request"));
  EXPECT_FALSE(server->IsResourceValid("/gnu/"));
}
// end IsResourceValidTest

TEST_F(RequestTest, Index)
{
  Response result = server->Request("GET /index.html HTTP/1.0");

  EXPECT_STREQ(result.header.c_str(), "HTTP/1.0 200 OK");
}

TEST_F(RequestTest, Root)
{
  Response result = server->Request("GET / HTTP/1.0");

  EXPECT_STREQ(result.header.c_str(),"HTTP/1.0 200 OK");
}

TEST_F(RequestTest, NestedResource)
{
  Response result = server->Request("GET /gnu/main.html HTTP/1.0");

  EXPECT_STREQ(result.header.c_str(),"HTTP/1.0 200 OK");
}

TEST_F(RequestTest, NonexistantFile)
{
  Response result = server->Request("GET /nofile HTTP/1.0");

  EXPECT_STREQ(result.header.c_str(),"HTTP/1.0 404 Not Found");
}

TEST_F(RequestTest, NonexistantNestedFile)
{
  Response result = server->Request("GET /gnu/nofile HTTP/1.0");

  EXPECT_STREQ(result.header.c_str(),"HTTP/1.0 404 Not Found");
}
TEST_F(RequestTest, VerbAndProtocolCaseInsensitive)
{
  Response result = server->Request("get / http/1.0");

  EXPECT_STREQ(result.header.c_str(),"HTTP/1.0 200 OK");
}

TEST_F(RequestTest, ResourceCaseSensitive)
{
  Response result = server->Request("GET /Index.html HTTP/1.0");

  EXPECT_STREQ(result.header.c_str(),"HTTP/1.0 404 Not Found");
}

TEST_F(RequestTest, InvalidURI)
{
  Response result = server->Request("GET http.request HTTP/1.0");

  EXPECT_STREQ(result.header.c_str(),"HTTP/1.0 400 Bad Request");
}

TEST_F(RequestTest, UnreadableFile)
{
  Response result = server->Request("GET /cantRead HTTP/1.0");

  EXPECT_STREQ(result.header.c_str(),"HTTP/1.0 404 Not Found");
}

TEST_F(RequestTest, InvalidProtocol)
{
  EXPECT_STREQ( server->Request("GET /index.html HTTP/1.1").header.c_str(),
      "HTTP/1.0 400 Bad Request");
}

TEST_F(RequestTest, CantReadDirectory)
{
  Response result = server->Request("GET /gnu/ HTTP/1.1");

  EXPECT_STREQ(result.header.c_str(),"HTTP/1.0 400 Bad Request");
}

TEST_F(RequestTest, UnknownVerb)
{
  Response result = server->Request("got / http/1.0");

  EXPECT_STREQ(result.header.c_str(),"HTTP/1.0 400 Bad Request");
}

TEST_F(RequestTest, ParentDirectory)
{
  Response result = server->Request("GET /../ HTTP/1.0");

  EXPECT_STREQ(result.header.c_str(),"HTTP/1.0 404 Not Found");
}
