// Copyright (C) 2021, Chao Xu
//
// Part of cpp2kdb, which is released under BSD license. See LICENSE for full
// details.
#include "cpp2kdb/kdb_wrapper.h"

#include <iostream>
#include <string>

namespace {
void TestAtomValue(int connection) {
  std::string expression_to_evaluate = "4 * 5 + 6";

  std::cout << "Evaluating " << expression_to_evaluate << " on kdb..."
            << std::endl;
  void* x = cpp2kdb::kdb_wrapper::RunQueryOnConnection(
      connection, expression_to_evaluate.c_str());

  std::cout << "Type ID is " << cpp2kdb::kdb_wrapper::GetQTypeId(x)
            << std::endl;

  std::cout << "Cast to int64_t, the value is "
            << *(static_cast<int64_t*>(cpp2kdb::kdb_wrapper::GetValue(x)))
            << std::endl;

  cpp2kdb::kdb_wrapper::DecreaseReferenceCount(x);
}
void TestVectorValue(int connection) {
  std::string expression_for_vector = "1 2 3";
  std::cout << "Now evaluate the following: " << expression_for_vector
            << std::endl;

  void* v = cpp2kdb::kdb_wrapper::RunQueryOnConnection(
      connection, expression_for_vector.c_str());

  std::cout << "Type ID is " << cpp2kdb::kdb_wrapper::GetQTypeId(v)
            << std::endl;

  int64_t n = cpp2kdb::kdb_wrapper::GetNumberOfVectorElements(v);
  std::cout << "Number of elements is " << n << std::endl;

  int64_t* values = static_cast<int64_t*>(cpp2kdb::kdb_wrapper::GetVector(v));

  for (int i = 0; i < n; i++) {
    std::cout << values[i] << " ";
  }
  std::cout << std::endl;
  cpp2kdb::kdb_wrapper::DecreaseReferenceCount(v);
}
void TestSymbol(int connection) {
  std::string expression = "`IBM";
  std::cout << "Now evaluate to a symbol: " << expression << std::endl;
  void* v = cpp2kdb::kdb_wrapper::RunQueryOnConnection(connection,
                                                       expression.c_str());
  std::cout << "Type ID is " << cpp2kdb::kdb_wrapper::GetQTypeId(v)
            << std::endl;
  std::cout << std::string(
                   *static_cast<char**>(cpp2kdb::kdb_wrapper::GetValue(v)))
            << std::endl;
  cpp2kdb::kdb_wrapper::DecreaseReferenceCount(v);
}
}  // namespace

int main(int argc, char** argv) {
  // Open connection
  int connection = cpp2kdb::kdb_wrapper::OpenConnection("127.0.0.1", 5000, "");

  if (connection <= 0) {
    std::cerr << "Connection error: " << connection << std::endl;
    return 1;
  }

  TestAtomValue(connection);
  TestVectorValue(connection);
  TestSymbol(connection);

  cpp2kdb::kdb_wrapper::CloseConnection(connection);
  return 0;
}
