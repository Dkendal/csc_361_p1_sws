#include <gtest/gtest.h>
#include "../src/response.h"

TEST(ResponseTest, Constructor)
{
  Response response("GET /index.html HTTP/1.0");

  EXPECT_EQ(response.verb, "GET");
  EXPECT_EQ(response.resource, "/index.html");
  EXPECT_EQ(response.protocol, "HTTP/1.0");
}
