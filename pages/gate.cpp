#include "../models/models_general.hpp"
#include "handler.hpp"
#include <exception>
using namespace Middleware;
using namespace nlohmann;

route("/api/login", login) {
  // Database Setup
  Sqlite3 db;
  if (!Sqlite_Open()) {
    return Server.Response(connection, 500, "Internal Server Error",
                           R"({"error":"Failed to open DB"})");
  }

  // Parsing JSON Post
  json post_as_json = json::parse(Server.Read(connection));
  Model<users> user_binder;
  user_binder.bind("nama", &users::nama)
      .bind("password", &users::password)
      .bind("role", &users::roles); // Bind role to users::roles

  auto user_mapper = user_binder.parse_one(post_as_json);

  // Tokenize the password and create a token
  std::string recipe = user_mapper.nama + user_mapper.password;
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
      // Extract role and assign to user_mapper
      std::string role = checker[0]["role"];
      user_mapper.roles =
          role; // Directly assign the role field to the user_mapper
    } else {
      return Server.Response(connection, 404, "User Not Found",
                             R"({"error":"User not found or invalid token"})");
    }

    // Convert the string role to enum
    user = Auth::strToRole(user_mapper.roles);
    if (user == Auth::Roles::Unknown) {
      return Server.Response(connection, 400, "Bad Request",
                             R"({"error":"Invalid role in database"})");
    }

    // Check if password matches (you should compare hashed passwords here)
    // For simplicity, assume password verification is done here.

    // Redirect based on the user's role
    std::string redirect_url;
    switch (user) {
    case Auth::Roles::Operator:
      redirect_url = "/admin/dashboard";
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
      redirect_url = "/login"; // Default fallback if role is unknown
      break;
    }

    // Now, perform the redirect
    return Server.Response(connection, 302, "Redirecting",
                           R"({"redirect_url":")" + redirect_url + R"("})");

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
