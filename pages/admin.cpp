#include "Phoenix/middleware.hpp"
#include "handler.hpp"
#define OK(connection)                                                         \
  Server.Response(connection, 200, "Ok", R"({"message":"success"})")
using namespace Middleware;
route("/dashboard/admin", dashboard_admin) {
  const char *cookie_header = mg_get_header(connection, "Cookie");
  if (cookie_header) {
    std::string cookies(cookie_header);
    std::string tokenKey = "auth_token=";
    size_t tokenPos = cookies.find(tokenKey);

    if (tokenPos != std::string::npos) {
      size_t start = tokenPos + tokenKey.length();
      size_t end = cookies.find(";", start);
      std::string token = cookies.substr(
          start, (end == std::string::npos) ? std::string::npos : end - start);

      // Open database connection
      Sqlite3 db;
      if (Sqlite_Open()) {
        nlohmann::json checker = sqlite.SELECT("role, nama")
                                     .FROM("users")
                                     .WHERE("token=" + Escape(token))
                                     .JSON();

        Sqlite_Close();

        // Ensure valid token and user role exist
        if (checker.is_array() && !checker.empty()) {
          // Extract role from the checker JSON
          std::string role = checker[0]["role"];

          // Check if the role is authorized for this page
          if (role == Auth::roleToStr(Auth::Roles::Operator)) {
            // Serve the admin dashboard HTML
            Server.static_serve("public/admin/dashboardAdmin.html", connection);
            return 200; // Success
          }
        }
      }
    }
  }

  // If no token or invalid token, return Unauthorized (401)
  return Server.ResponseAsFile(connection, 401, "Unauthorized",
                               "public/401.html");
}
