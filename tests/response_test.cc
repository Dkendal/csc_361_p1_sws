#include <gtest/gtest.h>
#include "../src/response.h"

TEST(ResponseTest, ConstructorValid)
{
  Response response("GET /index.html HTTP/1.0\n\n");

  EXPECT_EQ(response.verb, "GET");
  EXPECT_EQ(response.resource, "/index.html");
  EXPECT_EQ(response.protocol, "HTTP/1.0");
}

TEST(ResponseTest, SubstitutesIndex)
{
  Response response("GET / HTTP/1.0\n\n");

  EXPECT_EQ(response.verb, "GET");
  EXPECT_EQ(response.resource, "/index.html");
  EXPECT_EQ(response.protocol, "HTTP/1.0");
}

TEST(ResponseTest, ConvertsCase)
{
  Response response("get /index.html HttP/1.0\n\n");

  EXPECT_EQ(response.verb, "GET");
  EXPECT_EQ(response.resource, "/index.html");
  EXPECT_EQ(response.protocol, "HTTP/1.0");
}

TEST(ResponseTest, ConstructorInvalid)
{
  Response response("GET /index.html");

  EXPECT_EQ(response.verb, "GET");
  EXPECT_EQ(response.resource, "/index.html");
  EXPECT_EQ(response.protocol, "");
}
