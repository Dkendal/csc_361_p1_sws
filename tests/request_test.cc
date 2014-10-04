#include <gtest/gtest.h>
#include "../src/request.cc"

TEST(Request, IsValid)
{
  Request req_uppercase("GET", "/", "HTTP/1.0");
  Request req_lowercase("get", "/", "HTTP/1.0");
  Request req_resource("get", "/gnu/main.html", "HTTP/1.0");

  EXPECT_TRUE(req_uppercase.IsValid());
  EXPECT_TRUE(req_lowercase.IsValid());
}
