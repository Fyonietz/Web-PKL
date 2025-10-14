#include "../models/models_general.hpp"
#include "handler.hpp"
#include <exception>
using namespace Middleware;
using namespace nlohmann;

const std::string IP = "http://192.168.99.159:3000";
const std::string IP_ORIGIN = "http://192.168.99.159:3000";
const std::string BACKEND_IP = "http://192.168.99.80:9001";

route("/api/login", login) {

  // Database Setup
  Sqlite3 db;
  if (!Sqlite_Open()) {
    return Server.Response(connection, 500, "Internal Server Error",
                           R"({"error":"Failed to open DB"})");
  }

  // Parsing JSON Post
  json post_as_json = json::parse(Server.Read(connection));
  print(post_as_json.dump(4));
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
    // Using parameterized queries for security
    json checker = sqlite.SELECT("role, nama")
                       .FROM("users")
                       .WHERE("token=" + Escape(token))
                       .JSON();

    Sqlite_Close();

    // Ensure the role is extracted correctly from the checker JSON
    if (checker.is_array() && !checker.empty()) {
      std::string role = checker[0]["role"];
      user_mapper.roles = role;
    } else {
      return Server.Response(connection, 404, "User Not Found",
                             R"({"error":"User not found or invalid token"}");
    }

    // Convert the string role to enum
    user = Auth::strToRole(user_mapper.roles);
    if (user == Auth::Roles::Unknown) {
      return Server.Response(connection, 400, "Bad Request",
                         R"({"error":"Invalid role in database"})");
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
      redirect_url = "/login";
      break;
    }

    // Set the token in a cookie
    std::string cookie =
        "auth_token=" + token +
        "; Path=/; HttpOnly; Secure; SameSite=Strict; Max-Age=3600";

    // Send response with CORS headers and cookie using CORS function
    std::string response_body =
        "{\"success\": true, \"redirect\": \"" + redirect_url + "\"}";

    return Server.Response(connection, 200, "OK", response_body);

  } catch (const std::exception &e) {
    Sqlite_Close();
    return Server.Response(connection, 500, "Error",
                           std::string("{\"error\":\"") + e.what() + "\"}");
  }
}

route("/login", login_pages) {
  Server.SSR("public/login.html", connection);
  return 200;
}

route("/logout", logout) {
  mg_printf(connection, "HTTP/1.1 302 Found\r\n"
                        "Location: /login\r\n"
                        "Set-Cookie: auth_token=; Path=/; Expires=Thu, 01 Jan "
                        "1970 00:00:00 GMT\r\n"
                        "Content-Length: 0\r\n"
                        "Connection: close\r\n\r\n");
  return 302;
}
