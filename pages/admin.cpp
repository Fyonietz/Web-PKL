#include "../models/models_general.hpp"
#include "Phoenix/middleware.hpp"
#include "handler.hpp"
#include <iostream>
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

route("/api/admin/user/create", admin_create_user) {
  try {
    const char *cookie_header = mg_get_header(connection, "Cookie");

    if (cookie_header) {
      std::string cookies(cookie_header);
      std::string tokenKey = "auth_token=";
      size_t tokenPos = cookies.find(tokenKey);

      if (tokenPos != std::string::npos) {
        size_t start = tokenPos + tokenKey.length();
        size_t end = cookies.find(";", start);
        std::string token =
            cookies.substr(start, (end == std::string::npos) ? std::string::npos
                                                             : end - start);

        // Open database connection
        Sqlite3 db;
        if (Sqlite_Open()) {
          nlohmann::json checker = sqlite.SELECT("role, nama")
                                       .FROM("users")
                                       .WHERE("token=" + Escape(token))
                                       .JSON();

          Sqlite_Close();

          if (checker.is_array() && !checker.empty()) {
            std::string role = checker[0]["role"];
            if (role != Auth::roleToStr(Auth::Roles::Operator)) {
              Server.ResponseAsFile(connection, 401, "Unauthorized",
                                    "public/401.html");
              return 401;
            }
          }
        }
      }
    }

    // Read Post Data (moved inside try to catch bad JSON)
    nlohmann::json post_as_json =
        nlohmann::json::parse(Server.Read(connection));

    Sqlite3 db;
    Model<users> user_binder;
    user_binder.bind("nama", &users::nama)
        .bind("password", &users::password)
        .bind("role", &users::roles)
        .bind("jurusan", &users::jurusan)
        .bind("kelas", &users::kelas);

    auto user_mapper = user_binder.parse_one(post_as_json);
    std::string recipe =
        user_mapper.nama + user_mapper.password + "walataqrobuzina";
    std::string token = Auth::tokenizer(recipe);

    if (Sqlite_Open()) {
      sqlite
          .INSERT("users", "(nama,password,role,jurusan,kelas,token)",
                  "(" + Escape(user_mapper.nama) + "," +
                      Escape(user_mapper.password) + "," +
                      Escape(user_mapper.roles) + "," +
                      Escape(user_mapper.jurusan) + "," +
                      Escape(user_mapper.kelas) + "," + Escape(token) + ")")
          .execute();
      Sqlite_Close();
    }
  } catch (const nlohmann::json::exception &je) {
    std::cerr << "[JSON Error] " << je.what() << std::endl;
    return Server.Response(connection, 400, "Invalid JSON", "");
  } catch (const std::exception &e) {
    std::cerr << "[Exception] " << e.what() << std::endl;
    return Server.Response(connection, 500, "Internal Server Error", "");
  } catch (...) {
    std::cerr << "[Unknown Exception]" << std::endl;
    return Server.Response(connection, 500, "Internal Server Error", "");
  }

  return 200;
};
