#pragma once
#ifndef MIDDLEWARE
#include <iomanip>
#include <openssl/evp.h>
#include <string>

namespace Middleware {
struct Auth {
  enum class Roles {
    Operator = 1,
    Siswa = 2,
    Pembimbing = 3,
    Perusahaan = 4,
    Kaprodi = 5,
    Unknown = 6
  };
  static Roles strToRole(const std::string &user);
  static std::string roleToStr(Roles roles);
  static std::string tokenizer(const std::string &input);
};
} // namespace Middleware

#endif // !MIDDLEWARE
