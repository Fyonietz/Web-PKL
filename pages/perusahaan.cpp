#include "../models/models_general.hpp"
#include "handler.hpp"
using namespace Middleware;

route("/dashboard/admin/perusahaan/crud", admin_perusahaan_crud) {
  try {
    auto authInfo = CheckAuthToken(connection, Auth::Roles::Operator);
    if (!authInfo) {
      Server.ResponseAsFile(connection, 401, "Unauthorized", "public/401.html");
      return 401;
    }

    Server.static_serve("public/admin/perusahaan/crudPerusahaan.html",
                        connection);
  } catch (...) {
    return Server.Response(connection, 500, "Internal Server Error", "");
  }

  return 200;
}

route("/dashboard/admin/perusahaan/lists", admin_perusahaan_lists) {
  try {
    auto authInfo = CheckAuthToken(connection, Auth::Roles::Operator);
    if (!authInfo) {
      Server.ResponseAsFile(connection, 401, "Unauthorized", "public/401.html");
      return 401;
    }

    Server.static_serve("public/admin/perusahaan/tabelPerusahaan.html",
                        connection);
  } catch (...) {
    return Server.Response(connection, 500, "Internal Server Error", "");
  }

  return 200;
}

// Create Perusahaan
route("/api/perusahaan/create", create_perusahaan) {
  try {
    const struct mg_request_info *req_info = mg_get_request_info(connection);

    // Automatically handle OPTIONS
    if (Server.CORS(connection, req_info, IP))
      return 1;
    auto authInfo = CheckAuthToken(connection, Auth::Roles::Operator);
    if (!authInfo) {
      Server.ResponseAsFile(connection, 401, "Unauthorized", "public/401.html");
      return 401;
    }
    nlohmann::json post_as_json =
        nlohmann::json::parse(Server.Read(connection));
    Model<perusahaan> perusahaan_binder;
    perusahaan_binder.bind("nama", &perusahaan::nama)
        .bind("kuota", &perusahaan::kuota)
        .bind("alamat", &perusahaan::alamat)
        .bind("jurusan", &perusahaan::jurusan)
        .bind("benefit", &perusahaan::benefit);
    auto perusahaan_mapper = perusahaan_binder.parse_one(post_as_json);
    Sqlite3 db;
    if (Sqlite_Open()) {
      sqlite
          .INSERT("perusahaan", "(nama,jurusan,benefit,alamat,kuota)",
                  "(" + Escape(perusahaan_mapper.nama) + "," +
                      Escape(perusahaan_mapper.jurusan) + "," +
                      Escape(perusahaan_mapper.benefit) + "," +
                      Escape(perusahaan_mapper.alamat) + "," +
                      Escape(perusahaan_mapper.kuota) + ")")
          .execute();
      Sqlite_Close();
      return Server.CORS(connection, 200, "OK", "", IP);
    }
  } catch (const nlohmann::json::exception &je) {
    std::cerr << "[JSON Error] " << je.what() << std::endl;
    return Server.CORS(connection, 400, "Invalid JSON", "", IP);
  } catch (const std::exception &e) {
    std::cerr << "[Exception] " << e.what() << std::endl;
    return Server.CORS(connection, 500, "Internal Server Error", "", IP);
  } catch (...) {
    std::cerr << "[Unknown Exception]" << std::endl;
    return Server.CORS(connection, 500, "Internal Server Error", "", IP);
  }
  return 200;
}

route("/api/perusahaan/create/content", perusahaan_content) {
  const struct mg_request_info *req_info = mg_get_request_info(connection);

  // Automatically handle OPTIONS
  if (Server.CORS(connection, req_info, IP))
    return 1;
  auto authInfo = CheckAuthToken(connection, Auth::Roles::Operator);
  if (!authInfo) {
    Server.ResponseAsFile(connection, 401, "Unauthorized", "public/401.html");
    return 401;
  }

  return 200;
}
