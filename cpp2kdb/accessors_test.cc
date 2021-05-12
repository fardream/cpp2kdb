// Copyright (C) 2021, Chao Xu
//
// Part of cpp2kdb, which is released under BSD license. See LICENSE for full
// details.
#include "cpp2kdb/accessors.h"

#include <iostream>
#include <string>

namespace {
template <typename L, typename R>
std::string IsSameType() {
  return std::is_same_v<L, R> ? "Yes" : "No";
}

void TestTypes(int connection) {
  std::cout << "Type of char is " << cpp2kdb::q_type_id<char> << std::endl;
  std::cout << "Type of UID is "
            << cpp2kdb::q_type_id<cpp2kdb::QGuid> << std::endl;
  std::cout << "Type of byte is "
            << cpp2kdb::q_type_id<std::uint8_t> << std::endl;
  std::cout << "Type of string is "
            << cpp2kdb::q_type_id<std::string> << std::endl;
  std::cout << "Type ID 1 is mapped to bool? "
            << IsSameType<cpp2kdb::CTypeForQTypeId<1>::CType, bool>()
            << std::endl;
  std::cout << "Type ID 10 is mapped to bool? "
            << IsSameType<cpp2kdb::CTypeForQTypeId<10>::CType, bool>()
            << std::endl;
  std::cout << "Type ID 10 is mapped to char? "
            << IsSameType<cpp2kdb::CTypeForQTypeId<10>::CType, char>()
            << std::endl;
  std::cout << "Type ID 11 is mapped to std::string? "
            << IsSameType<cpp2kdb::CTypeForQTypeId<11>::CType, std::string>()
            << std::endl;
}

void TestDictionary(int connection) {
  std::string query =
      "(`Arthur`Dent; `Zaphod`Beeblebrox; `Ford`Prefect)! 100 42 150";
  void* result = cpp2kdb::RunQueryOnConnection(connection, query.c_str());
  std::cout << "Result type is " << cpp2kdb::GetTypeId(result) << std::endl;
  std::cout << "Size of the list is "
            << cpp2kdb::GetNumberOfVectorElements(result) << std::endl;
  void** key_value_list = static_cast<void**>(cpp2kdb::GetVector(result));
  std::cout << "Key's type is " << cpp2kdb::GetTypeId(key_value_list[0])
            << " and the count is "
            << cpp2kdb::GetNumberOfVectorElements(key_value_list[0])
            << std::endl;
  std::cout << "Value's type is " << cpp2kdb::GetTypeId(key_value_list[1])
            << " and the count is "
            << cpp2kdb::GetNumberOfVectorElements(key_value_list[1])
            << std::endl;
  cpp2kdb::DecreaseReferenceCount(result);
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
  TestDictionary(connection);
  cpp2kdb::CloseConnection(connection);
  return 0;
}
