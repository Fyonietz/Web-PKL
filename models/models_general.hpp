#pragma once
#include <string>
struct admin {
  std::string id;
  std::string nama;
  std::string password;
};

struct siswa {
  std::string id;
  std::string nama;
  std::string password;
  std::string jurusan;
  std::string pembimbing;
  std::string perusahaan;
};

struct pembimbing {
  std::string id;
  std::string nama;
  std::string password;
  std::string jurusan;
};

struct kaprodi {
  std::string id;
  std::string nama;
  std::string password;
  std::string jurusan;
};

struct perusahaan {
  std::string id;
  std::string nama;
  std::string password;
  std::string jurusan;
  std::string alamat;
  std::string kuota;
};

struct users {
  std::string id;
  std::string nama;
  std::string password;
  std::string jurusan;
  std::string alamat;
  std::string kuota;
  std::string perusahaan;
  std::string pembimbing;
  std::string roles;
};
