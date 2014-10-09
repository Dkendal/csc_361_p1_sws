#include <gtest/gtest.h>
#include "../src/response.h"

TEST(ResponseTest, ConstructorValid)
{
  Response response("GET /index.html HTTP/1.0\n\n");

  EXPECT_EQ(response.verb, "GET");
  EXPECT_EQ(response.resource, "/index.html");
  EXPECT_EQ(response.protocol, "HTTP/1.0");
}
TEST(ResponseTest, ConstructorInvalid)
{
  Response response("asdf asd");

  EXPECT_EQ(response.verb, "asdf");
  EXPECT_EQ(response.resource, "asd");
  EXPECT_EQ(response.protocol, "");
}
