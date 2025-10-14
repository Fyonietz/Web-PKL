#pragma once

#include "../core/engine.hpp"
#include "../core/starter.hpp"
#include "../libs/Phoenix/controller.hpp"
#include "../libs/Phoenix/middleware.hpp"
#include "../libs/Phoenix/sqlite3.hpp"
#include "../libs/json.hpp"
#include "../routes/register.hpp"
#include "pyro.hpp"
#include <optional>
#include <string>
#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __attribute__((visibility("default")))
#endif
extern Pnix Server;
extern Global test;
extern const std::string IP;
extern const std::string IP_ORIGIN;
extern const std::string BACKEND_IP;
extern const std::string DATABASE;
template <typename... Args> void print(Args &&...args) {
  (std::cout << ... << args) << std::endl;
}
// Forward declarations
struct mg_connection;

EXPORT int default_handler(struct mg_connection *connection,
                           void * /*callbackdata*/);
EXPORT int home(struct mg_connection *connection, void *callback);
EXPORT std::optional<nlohmann::json>
CheckAuthToken(struct mg_connection *conn,
               std::optional<Middleware::Auth::Roles> requiredRole);
EXPORT int
CORSWithAuth(struct mg_connection *conn, const struct mg_request_info *req_info,
             const std::string &allowed_origin,
             std::optional<Middleware::Auth::Roles> requiredRole = std::nullopt,
             nlohmann::json *userInfo = nullptr);
// Even simpler - no EXPORT in macro
#define route(PATH, NAME)                                                      \
  int NAME(struct mg_connection *connection, void *cb);                        \
  namespace {                                                                  \
  struct NAME##_Reg {                                                          \
    NAME##_Reg() { add_route(PATH, NAME); }                                    \
  } NAME##_instance;                                                           \
  }                                                                            \
  int NAME(struct mg_connection *connection, void *cb)
