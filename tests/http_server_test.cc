#include <gtest/gtest.h>
#include "../src/http_server.cc"

class GetTest : public ::testing::Test
{
  protected:
    virtual void SetUp() { server = new HttpServer(".../www"); }
    virtual void TearDown() { delete server; }
    HttpServer *server;
};

TEST_F(GetTest, Index)
{
  std::string result = server->Get("GET /index.html HTTP/1.0");

  EXPECT_STREQ(result.c_str(), "HTTP/1.0 200 OK");
}

TEST_F(GetTest, Root)
{
  std::string result = server->Get("GET / HTTP/1.0");

  EXPECT_STREQ(result.c_str(),"HTTP/1.0 200 OK");
}

TEST_F(GetTest, NestedResource)
{
  std::string result = server->Get("GET /gnu/main.html HTTP/1.0");

  EXPECT_STREQ(result.c_str(),"HTTP/1.0 200 OK");
}

TEST_F(GetTest, NonexistantFile)
{
  std::string result = server->Get("GET /nofile HTTP/1.0");

  EXPECT_STREQ(result.c_str(),"HTTP/1.0 404 Not Found");
}

TEST_F(GetTest, NonexistantNestedFile)
{
  std::string result = server->Get("GET /gnu/nofile HTTP/1.0");

  EXPECT_STREQ(result.c_str(),"HTTP/1.0 404 Not Found");
}
TEST_F(GetTest, VerbAndProtocolCaseInsensitive)
{
  std::string result = server->Get("get / http/1.0");

  EXPECT_STREQ(result.c_str(),"HTTP/1.0 200 OK");
}

TEST_F(GetTest, ResourceCaseSensitive)
{
  std::string result = server->Get("GET /Index.html HTTP/1.0");

  EXPECT_STREQ(result.c_str(),"HTTP/1.0 404 Not Found");
}

TEST_F(GetTest, InvalidURI)
{
  std::string result = server->Get("GET http.request HTTP/1.0");

  EXPECT_STREQ(result.c_str(),"HTTP/1.0 400 Bad Request");
}

TEST_F(GetTest, UnreadableFile)
{
  std::string result = server->Get("GET /cantRead HTTP/1.0");

  EXPECT_STREQ(result.c_str(),"HTTP/1.0 404 Not Found");
}

TEST_F(GetTest, InvalidProtocol)
{
  std::string result = server->Get("GET /index.html HTTP/1.1");

  EXPECT_STREQ(result.c_str(),"HTTP/1.0 400 Bad Request");
}

TEST_F(GetTest, CantReadDirectory)
{
  std::string result = server->Get("GET /gnu/ HTTP/1.1");

  EXPECT_STREQ(result.c_str(),"HTTP/1.0 400 Bad Request");
}

TEST_F(GetTest, UnknownVerb)
{
  std::string result = server->Get("got / http/1.0");

  EXPECT_STREQ(result.c_str(),"HTTP/1.0 400 Bad Request");
}

TEST_F(GetTest, ParentDirectory)
{
  std::string result = server->Get("GET /../ HTTP/1.0");

  EXPECT_STREQ(result.c_str(),"HTTP/1.0 404 Not Found");
}
