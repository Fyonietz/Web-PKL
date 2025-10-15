#include "../models/models_general.hpp"
#include "Phoenix/middleware.hpp"
#include "civetweb.h"
#include "handler.hpp"
#include "json.hpp"
#include <exception>
#include <iostream>
#include <iterator>
#include <string>

#define OK(connection)                                                         \
  Server.Response(connection, 200, "Ok", R"({"message":"success"})")

using namespace Middleware;

// Dashboard admin page
route("/dashboard/admin", dashboard_admin) {
  auto authInfo = CheckAuthToken(connection, Auth::Roles::Operator);

  if (!authInfo) {
    return Server.ResponseAsFile(connection, 401, "Unauthorized",
                                 "public/401.html");
  }

  // User is authorized as Operator, serve the admin dashboard
  Server.static_serve("public/admin/dashboardAdmin.html", connection);
  return 200;
}

// Admin account lists page
route("/dashboard/admin/akun/lists", admin_akun) {
  auto authInfo = CheckAuthToken(connection, Auth::Roles::Operator);

  if (!authInfo) {
    return Server.ResponseAsFile(connection, 401, "Unauthorized",
                                 "public/401.html");
  }

  Server.static_serve("public/admin/pantauAkun.html", connection);
  return 200;
}

// Admin account CRUD page
route("/dashboard/admin/akun/crud", admin_akun_crud) {
  auto authInfo = CheckAuthToken(connection, Auth::Roles::Operator);

  if (!authInfo) {
    return Server.ResponseAsFile(connection, 401, "Unauthorized",
                                 "public/401.html");
  }

  Server.static_serve("public/admin/crudAkun.html", connection);
  return 200;
}

// API: Get roles list
route("/api/ext/roles", roles_get) {
  const struct mg_request_info *req_info = mg_get_request_info(connection);
  nlohmann::json userInfo;

  // Handle CORS + Auth
  int authResult = CORSWithAuth(connection, req_info, IP_ORIGIN,
                                Auth::Roles::Operator, &userInfo);
  if (authResult != 0)
    return authResult;

  try {
    Sqlite3 db;
    if (!Sqlite_Open()) {
      return Server.CORS(connection, 500, "Internal Server Error",
                         R"({"error":"Failed to open database"})", IP_ORIGIN);
    }

    nlohmann::json query = sqlite.SELECT("nama").FROM("roles").JSON();
    Sqlite_Close();

    return Server.CORS(connection, 200, "OK", query.dump(4), IP_ORIGIN);

  } catch (const std::exception &e) {
    std::cerr << "[Exception] " << e.what() << std::endl;
    return Server.CORS(connection, 500, "Internal Server Error",
                       R"({"error":"Internal server error"})", IP_ORIGIN);
  }
}

// API: Create new user
route("/api/admin/user/create", admin_create_user) {
  const struct mg_request_info *req_info = mg_get_request_info(connection);
  nlohmann::json userInfo;

  // Handle CORS + Auth
  int authResult = CORSWithAuth(connection, req_info, IP_ORIGIN,
                                Auth::Roles::Operator, &userInfo);
  if (authResult != 0)
    return authResult;

  try {
    // Read and parse POST data
    nlohmann::json post_as_json =
        nlohmann::json::parse(Server.Read(connection));

    // Bind user model
    Model<users> user_binder;
    user_binder.bind("nama", &users::nama)
        .bind("password", &users::password)
        .bind("role", &users::roles)
        .bind("jurusan", &users::jurusan)
        .bind("kelas", &users::kelas);

    auto user_mapper = user_binder.parse_one(post_as_json);

    // Generate token
    std::string recipe =
        user_mapper.nama + user_mapper.password + "walataqrobuzina";
    std::string token = Auth::tokenizer(recipe);

    // Insert into database
    Sqlite3 db;
    if (!Sqlite_Open()) {
      return Server.CORS(connection, 500, "Internal Server Error",
                         R"({"error":"Failed to open database"})", IP_ORIGIN);
    }

    sqlite
        .INSERT("users", "(nama,password,role,jurusan,kelas,token)",
                "(" + Escape(user_mapper.nama) + "," +
                    Escape(user_mapper.password) + "," +
                    Escape(user_mapper.roles) + "," +
                    Escape(user_mapper.jurusan) + "," +
                    Escape(user_mapper.kelas) + "," + Escape(token) + ")")
        .execute();

    Sqlite_Close();

    return Server.CORS(
        connection, 200, "OK",
        R"({"success":true,"message":"User created successfully"})", IP_ORIGIN);

  } catch (const nlohmann::json::exception &je) {
    std::cerr << "[JSON Error] " << je.what() << std::endl;
    return Server.CORS(connection, 400, "Bad Request",
                       R"({"error":"Invalid JSON format"})", IP_ORIGIN);
  } catch (const std::exception &e) {
    std::cerr << "[Exception] " << e.what() << std::endl;
    return Server.CORS(connection, 500, "Internal Server Error",
                       R"({"error":"Internal server error"})", IP_ORIGIN);
  }
}
route("/api/admin/user/delete", user_delete) {
  const struct mg_request_info *req_info = mg_get_request_info(connection);
  nlohmann::json userInfo;

  int authResult = CORSWithAuth(connection, req_info, IP_ORIGIN,
                                Auth::Roles::Operator, &userInfo);

  if (authResult != 0)
    return authResult;

  try {
    nlohmann::json post_as_json =
        nlohmann::json::parse(Server.Read(connection));

    print(post_as_json.dump(4));
    Model<users> user_binder;
    user_binder.bind("id", &users::id);

    auto user_mapper = user_binder.parse_one(post_as_json);
    Sqlite3 db;
    if (Sqlite_Open()) {
      sqlite.DELETE("users").WHERE("id=" + Escape(user_mapper.id)).execute();
      Sqlite_Close();

      return Server.CORS(
          connection, 200, "OK",
          R"({"success":true,"message":"User deleted successfully"})",
          IP_ORIGIN);
    }
  } catch (const nlohmann::json::exception &je) {
    std::cerr << "[JSON Error] " << je.what() << std::endl;
    return Server.CORS(connection, 400, "Bad Request",
                       R"({"error":"Invalid JSON format"})", IP_ORIGIN);
  } catch (const std::exception &e) {
    std::cerr << "[Exception] " << e.what() << std::endl;
    return Server.CORS(connection, 500, "Internal Server Error",
                       R"({"error":"Internal server error"})", IP_ORIGIN);
  }
  return 200;
}
// API: Get user lists
route("/api/admin/user/lists", user_lists) {
  const struct mg_request_info *req_info = mg_get_request_info(connection);
  nlohmann::json userInfo;

  // Handle CORS + Auth
  int authResult = CORSWithAuth(connection, req_info, IP_ORIGIN,
                                Auth::Roles::Operator, &userInfo);
  if (authResult != 0)
    return authResult;

  try {
    Sqlite3 db;
    if (!Sqlite_Open()) {
      return Server.CORS(connection, 500, "Internal Server Error",
                         R"({"error":"Failed to open database"})", IP_ORIGIN);
    }

    nlohmann::json query = sqlite.SELECT("id,nama,password,jurusan,role,kelas")
                               .FROM("users")
                               .JSON();

    Sqlite_Close();

    return Server.CORS(connection, 200, "OK", query.dump(4), IP_ORIGIN);

  } catch (const std::exception &e) {
    std::cerr << "[Exception] " << e.what() << std::endl;
    return Server.CORS(connection, 500, "Internal Server Error",
                       R"({"error":"Internal server error"})", IP_ORIGIN);
  }
}
