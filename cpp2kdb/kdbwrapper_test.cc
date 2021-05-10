// Copyright (C) 2021, Chao Xu
//
// Part of cpp2kdb, which is released under BSD license. See LICENSE or full
// details.
#include "cpp2kdb/kdbwrapper.h"

#include <iostream>
#include <string>

namespace {
void TestAtomValue(int connection) {
  std::string expression_to_evaluate = "4 * 5 + 6";

  std::cout << "Evaluating " << expression_to_evaluate << " on kdb..."
            << std::endl;
  void* x =
      cpp2kdb::RunQueryOnConnection(connection, expression_to_evaluate.c_str());

  std::cout << "Type ID is " << cpp2kdb::GetTypeId(x) << std::endl;

  std::cout << "Cast to int64_t, the value is "
            << *(static_cast<int64_t*>(cpp2kdb::GetValue(x))) << std::endl;

  cpp2kdb::DecreaseReferenceCount(x);
}
void TestVectorValue(int connection) {
  std::string expression_for_vector = "1 2 3";
  std::cout << "Now evaluate the following: " << expression_for_vector
            << std::endl;

  void* v =
      cpp2kdb::RunQueryOnConnection(connection, expression_for_vector.c_str());

  std::cout << "Type ID is " << cpp2kdb::GetTypeId(v) << std::endl;

  int64_t n = cpp2kdb::GetNumberOfVectorElements(v);
  std::cout << "Number of elements is " << n << std::endl;

  int64_t* values = static_cast<int64_t*>(cpp2kdb::GetVector(v));

  for (int i = 0; i < n; i++) {
    std::cout << values[i] << " ";
  }
  std::cout << std::endl;
  cpp2kdb::DecreaseReferenceCount(v);
}
void TestSymbol(int connection) {
  std::string expression = "`IBM";
  std::cout << "Now evaluate to a symbol: " << expression << std::endl;
  void* v = cpp2kdb::RunQueryOnConnection(connection, expression.c_str());
  std::cout << "Type ID is " << cpp2kdb::GetTypeId(v) << std::endl;
  std::cout << std::string(*static_cast<char**>(cpp2kdb::GetValue(v)))
            << std::endl;
  cpp2kdb::DecreaseReferenceCount(v);
}
}  // namespace

int main(int argc, char** argv) {
  // Open connection
  int connection = cpp2kdb::OpenConnection("127.0.0.1", 5000, "");

  if (connection <= 0) {
    std::cerr << "Connection error: " << connection << std::endl;
    return 1;
  }

  TestAtomValue(connection);
  TestVectorValue(connection);
  TestSymbol(connection);

  cpp2kdb::CloseConnection(connection);
  return 0;
}
