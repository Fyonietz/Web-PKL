#include "handler.hpp"
Pnix Server;
Global test;

bool file_exists(const std::string &filename) {
  struct stat buffer;
  return (stat(filename.c_str(), &buffer) == 0);
}

std::string get_mime_type(const char *path) {
  const char *extension_types = strrchr(path, '.');
  if (!extension_types)
    return "text/plain";
  if (strcmp(extension_types, ".html") == 0)
    return "text/html";
  if (strcmp(extension_types, ".css") == 0)
    return "text/css";
  if (strcmp(extension_types, ".json") == 0)
    return "application/json";
  if (strcmp(extension_types, ".js") == 0)
    return "application/javascript";
  if (strcmp(extension_types, ".png") == 0)
    return "image/png";
  if (strcmp(extension_types, ".jpg") == 0 ||
      strcmp(extension_types, ".jpeg") == 0)
    return "image/jpeg";
  if (strcmp(extension_types, ".gif") == 0)
    return "image/gif";
  return "text/plain";
}

route("/", default_handler) {
  const struct mg_request_info *request_info = mg_get_request_info(connection);
  std::string uri = request_info->request_uri;

  // Sanitize path to prevent directory traversal
  if (uri.find("..") != std::string::npos) {
    mg_printf(connection,
              "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n");
    return 1;
  }

  // If root path, call your home function
  if (uri == "/") {
    home(connection, 0);
    return 1;
  }

  // Otherwise, try to serve static file from public folder
  std::string filepath = "public" + uri;

  if (file_exists(filepath)) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file) {
      mg_printf(connection,
                "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n");
      return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string file_content = buffer.str();
    file.close();

    std::string mime = get_mime_type(filepath.c_str());

    mg_printf(connection,
              "HTTP/1.1 200 OK\r\n"
              "Content-Type: %s\r\n"
              "Content-Length: %zu\r\n\r\n",
              mime.c_str(), file_content.size());

    mg_write(connection, file_content.data(), file_content.size());

    return 1;
  }

  // Not found
  mg_printf(connection, "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n");
  return 1;
}

EXPORT int home(struct mg_connection *connection, void *callback) {
  Server.home("public/layout.html", connection);
  return 200;
};

EXPORT std::optional<nlohmann::json> CheckAuthToken(
    struct mg_connection *conn,
    std::optional<Middleware::Auth::Roles> requiredRole = std::nullopt) {
  const char *cookie_header = mg_get_header(conn, "Cookie");
  if (!cookie_header)
    return std::nullopt;

  std::string cookies(cookie_header);
  std::string tokenKey = "auth_token=";
  size_t tokenPos = cookies.find(tokenKey);
  if (tokenPos == std::string::npos)
    return std::nullopt;

  size_t start = tokenPos + tokenKey.length();
  size_t end = cookies.find(";", start);
  std::string token = cookies.substr(
      start, (end == std::string::npos) ? std::string::npos : end - start);

  Sqlite3 db;
  if (!Sqlite_Open())
    return std::nullopt;

  nlohmann::json result = sqlite.SELECT("role, nama")
                              .FROM("users")
                              .WHERE("token=" + Escape(token))
                              .JSON();
  Sqlite_Close();

  if (!result.is_array() || result.empty())
    return std::nullopt;

  std::string roleStr = result[0]["role"];
  if (requiredRole.has_value() &&
      roleStr != Middleware::Auth::roleToStr(requiredRole.value())) {
    return std::nullopt;
  }

  return result[0]; // Contains `role` and `nama`
}

EXPORT int CORSWithAuth(struct mg_connection *conn,
                        const struct mg_request_info *req_info,
                        const std::string &allowed_origin,
                        std::optional<Middleware::Auth::Roles> requiredRole,
                        nlohmann::json *userInfo) {

  // Handle OPTIONS preflight first
  if (std::string(req_info->request_method) == "OPTIONS") {
    mg_printf(
        conn,
        "HTTP/1.1 204 No Content\r\n"
        "Access-Control-Allow-Origin: %s\r\n"
        "Access-Control-Allow-Methods: GET, POST, OPTIONS, DELETE, PUT\r\n"
        "Access-Control-Allow-Headers: Content-Type\r\n"
        "Access-Control-Allow-Credentials: true\r\n"
        "Access-Control-Max-Age: 86400\r\n"
        "Connection: close\r\n\r\n",
        allowed_origin.c_str());
    return 1; // OPTIONS handled
  }

  // Check authentication
  auto authResult = CheckAuthToken(conn, requiredRole);

  if (!authResult.has_value()) {
    // Auth failed - send 401 with CORS headers
    std::string error_msg =
        requiredRole.has_value()
            ? R"({"error":"Unauthorized: Invalid token or insufficient permissions"})"
            : R"({"error":"Unauthorized: No valid authentication token"})";

    mg_printf(
        conn,
        "HTTP/1.1 401 Unauthorized\r\n"
        "Content-Type: application/json\r\n"
        "Access-Control-Allow-Origin: %s\r\n"
        "Access-Control-Allow-Methods: GET, POST, OPTIONS, DELETE, PUT\r\n"
        "Access-Control-Allow-Headers: Content-Type\r\n"
        "Access-Control-Allow-Credentials: true\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s",
        allowed_origin.c_str(), error_msg.length(), error_msg.c_str());
    return 401;
  }

  // Auth succeeded - populate user info if pointer provided
  if (userInfo != nullptr) {
    *userInfo = authResult.value();
  }

  return 0; // Continue with route logic
}
