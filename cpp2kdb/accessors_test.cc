// Copyright (C) 2021, Chao Xu
//
// Part of cpp2kdb, which is released under BSD license. See LICENSE for full
// details.
#include "cpp2kdb/accessors.h"

#include <iostream>
#include <string>

namespace {
void TestTypes(int connection) {
  std::cout << "Type of char is " << cpp2kdb::QTypeId<char>::type_id
            << std::endl;
  std::cout << "Type of UID is " << cpp2kdb::QTypeId<cpp2kdb::QGuid>::type_id
            << std::endl;
  std::cout << "Type of byte is " << cpp2kdb::QTypeId<std::uint8_t>::type_id
            << std::endl;
}
}  // namespace

int main(int argc, char** argv) {
  // Open connection
  int connection = cpp2kdb::OpenConnection("127.0.0.1", 5000, "");

  if (connection <= 0) {
    std::cerr << "Connection error: " << connection << std::endl;
    return 1;
  }

  TestTypes(connection);

  cpp2kdb::CloseConnection(connection);
  return 0;
}
