#include "../models/models_general.hpp"
#include "civetweb.h"
#include "handler.hpp"
#include <exception>
#include <iostream>
#include <optional>
using namespace Middleware;
using namespace nlohmann;

const std::string IP = "http://192.168.99.114:3000";
const std::string IP_ORIGIN = "http://192.168.99.114:3000";
const std::string BACKEND_IP = "http://192.168.99.80:9001";

route("/api/login", login) {
  const struct mg_request_info *req_info = mg_get_request_info(connection);
  std::cout << "=== LOGIN REQUEST STARTED ===" << std::endl;
  std::cout << "Method: " << req_info->request_method << std::endl;
  std::cout << "URI: " << req_info->request_uri << std::endl;
  std::cout.flush(); // Force output
  print(req_info->conn_data);
  // Handle OPTIONS manually instead of using CORS function
  if (std::string(req_info->request_method) == "OPTIONS") {
    std::cout << "Handling OPTIONS preflight manually" << std::endl;
    mg_printf(connection,
              "HTTP/1.1 204 No Content\r\n"
              "Access-Control-Allow-Origin: http://192.168.99.114:3000\r\n"
              "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
              "Access-Control-Allow-Headers: Content-Type\r\n"
              "Access-Control-Allow-Credentials: true\r\n"
              "Access-Control-Max-Age: 86400\r\n"
              "Connection: close\r\n\r\n");
    return 204;
  } else {
    std::cout << "error dikit" << std::endl;
  }
  Sqlite3 db;
  if (!Sqlite_Open()) {
    return Server.CORS(connection, 500, "Internal Server Error",
                       R"({"error":"Failed to open DB"})", IP_ORIGIN);
  }

  // Parsing JSON Post
  std::string request_body = Server.Read(connection);

  json post_as_json;
  try {
    post_as_json = json::parse(request_body);
  } catch (const std::exception &e) {
    Sqlite_Close();
    return Server.CORS(connection, 400, "Bad Request",
                       std::string("{\"error\":\"Invalid JSON: ") + e.what() +
                           "\"}",
                       IP_ORIGIN);
  }

  Model<users> user_binder;
  user_binder.bind("nama", &users::nama)
      .bind("password", &users::password)
      .bind("role", &users::roles);

  auto user_mapper = user_binder.parse_one(post_as_json);

  // Tokenize the password and create a token
  std::string recipe =
      user_mapper.nama + user_mapper.password + "walataqrobuzina";
  std::string token = Auth::tokenizer(recipe);

  Auth::Roles user;

  // Check in Database
  try {
    std::string query = "token=" + Escape(token);

    json checker =
        sqlite.SELECT("role, nama").FROM("users").WHERE(query).JSON();

    Sqlite_Close();

    // Ensure the role is extracted correctly from the checker JSON
    if (checker.is_array() && !checker.empty()) {
      std::string role = checker[0]["role"];
      user_mapper.roles = role;
    } else {
      return Server.CORS(connection, 404, "User Not Found",
                         R"({"error":"User not found or invalid token"})",
                         IP_ORIGIN);
    }

    user = Auth::strToRole(user_mapper.roles);

    if (user == Auth::Roles::Unknown) {
      return Server.CORS(connection, 400, "Bad Request",
                         R"({"error":"Invalid role in database"})", IP_ORIGIN);
    }

    // Redirect based on the user's role
    std::string redirect_url;
    switch (user) {
    case Auth::Roles::Operator:
      redirect_url = IP + "/admin";
      break;
    case Auth::Roles::Kaprodi:
      redirect_url = "/kaprodi/home";
      break;
    case Auth::Roles::Pembimbing:
      redirect_url = "/pembimbing/dashboard";
      break;
    case Auth::Roles::Perusahaan:
      redirect_url = "/perusahaan/jobs";
      break;
    case Auth::Roles::Siswa:
      redirect_url = "/student/portal";
      break;
    default:
      redirect_url = "/login/test";
      break;
    }

    // Set the token in a cookie
    std::string cookie = "auth_token=" + token +
                         "; Path=/; HttpOnly;  SameSite=Lax; Max-Age=3600";

    // Send response with CORS headers and cookie using CORS function
    std::string response_body =
        "{\"success\": true, \"redirect\": \"" + redirect_url + "\"}";

    // USE CORS WITH COOKIE FOR THE SUCCESS RESPONSE
    return Server.CORS(connection, 200, "OK", response_body, IP_ORIGIN, cookie);

  } catch (const std::exception &e) {
    std::cout << "EXCEPTION in database operation: " << e.what() << std::endl;
    Sqlite_Close();
    return Server.CORS(connection, 500, "Error",
                       std::string("{\"error\":\"") + e.what() + "\"}",
                       IP_ORIGIN);
  }
}
route("/login", login_pages) {
  Server.SSR("public/login.html", connection);
  return 200;
}

route("/logout", logout) {
  std::cout << "Logout requested" << std::endl;
  mg_printf(connection, "HTTP/1.1 302 Found\r\n"
                        "Location: /login\r\n"
                        "Set-Cookie: auth_token=; Path=/; Expires=Thu, 01 Jan "
                        "1970 00:00:00 GMT\r\n"
                        "Content-Length: 0\r\n"
                        "Connection: close\r\n\r\n");
  return 302;
}

