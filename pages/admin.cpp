#include "Phoenix/middleware.hpp"
#include "handler.hpp"
#define OK(connection)                                                         \
  Server.Response(connection, 200, "Ok", R"({"message":"success"})")

