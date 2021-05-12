// Copyright (C) 2021, Chao Xu
//
// Part of cpp2kdb, which is released under BSD license. See LICENSE for full
// details.
#include "cpp2kdb/accessors.h"

#include <iostream>
#include <string>

namespace {
std::string SayYesOrNo(bool yes) { return yes ? "Yes" : "No"; }

template <typename L, typename R>
std::string IsSameType() {
  return SayYesOrNo(std::is_same_v<L, R>);
}

void TestTypes(int connection) {
  std::cout << "Type of char is " << cpp2kdb::q_type_id<char> << std::endl;
  std::cout << "Type of UID is "
            << cpp2kdb::q_type_id<cpp2kdb::QGuid> << std::endl;
  std::cout << "Type of byte is "
            << cpp2kdb::q_type_id<std::int8_t> << std::endl;
  std::cout << "Type of string is "
            << cpp2kdb::q_type_id<std::string> << std::endl;
  std::cout << "Type ID 1 is mapped to bool? "
            << IsSameType<cpp2kdb::CTypeForQTypeId<1>, bool>() << std::endl;
  std::cout << "Type ID 10 is mapped to bool? "
            << IsSameType<cpp2kdb::CTypeForQTypeId<10>, bool>() << std::endl;
  std::cout << "Type ID 10 is mapped to char? "
            << IsSameType<cpp2kdb::CTypeForQTypeId<10>, char>() << std::endl;
  std::cout << "Type ID 11 is mapped to std::string? "
            << IsSameType<cpp2kdb::CTypeForQTypeId<11>, std::string>()
            << std::endl;
}

void TestIsSame() {
  std::cout << "Is bool 1? " << SayYesOrNo(cpp2kdb::IsSameType<bool>(1))
            << std::endl;
  std::cout << "Is GUID 2? "
            << SayYesOrNo(cpp2kdb::IsSameType<cpp2kdb::QGuid>(2)) << std::endl;
  std::cout << "Is std::int8_t 4? "
            << SayYesOrNo(cpp2kdb::IsSameType<std::int8_t>(4)) << std::endl;
  std::cout << "Is short 5? "
            << SayYesOrNo(cpp2kdb::IsSameType<short>(5))  // NOLINT
            << std::endl;
  std::cout << "Is int 6? " << SayYesOrNo(cpp2kdb::IsSameType<int>(6))
            << std::endl;
  std::cout << "Is std::int64_t 7? "
            << SayYesOrNo(cpp2kdb::IsSameType<std::int64_t>(7)) << std::endl;
  std::cout << "Is float 8? "
            << SayYesOrNo(cpp2kdb::IsSameType<float>(8))  // NOLINT
            << std::endl;
  std::cout << "Is double 9? " << SayYesOrNo(cpp2kdb::IsSameType<double>(9))
            << std::endl;
  std::cout << "Is char 10? " << SayYesOrNo(cpp2kdb::IsSameType<char>(10))
            << std::endl;
  std::cout << "Is std::string 11? "
            << SayYesOrNo(cpp2kdb::IsSameType<std::string>(11)) << std::endl;
  std::cout << "Is std::string 10? "
            << SayYesOrNo(cpp2kdb::IsSameType<std::string>(10)) << std::endl;
  std::cout << "Is std::string -10? "
            << SayYesOrNo(cpp2kdb::IsSameType<std::string>(-10)) << std::endl;
  std::cout << "Is std::int64_t 12? "
            << SayYesOrNo(cpp2kdb::IsSameType<std::int64_t>(12)) << std::endl;
  std::cout << "Is int 13? " << SayYesOrNo(cpp2kdb::IsSameType<int>(13))
            << std::endl;
  std::cout << "Is int 14? " << SayYesOrNo(cpp2kdb::IsSameType<int>(14))
            << std::endl;
  std::cout << "Is double 15? " << SayYesOrNo(cpp2kdb::IsSameType<double>(15))
            << std::endl;
  std::cout << "Is std::int64_t 16? "
            << SayYesOrNo(cpp2kdb::IsSameType<std::int64_t>(16)) << std::endl;
  std::cout << "Is int 17? " << SayYesOrNo(cpp2kdb::IsSameType<int>(17))
            << std::endl;
  std::cout << "Is int 18? " << SayYesOrNo(cpp2kdb::IsSameType<int>(18))
            << std::endl;
  std::cout << "Is int 19? " << SayYesOrNo(cpp2kdb::IsSameType<int>(19))
            << std::endl;
}

void TestDictionary(int connection) {
  std::string query =
      "(`Arthur`Dent; `Zaphod`Beeblebrox; `Ford`Prefect)! 100 42 150";
  void* result = cpp2kdb::RunQueryOnConnection(connection, query.c_str());
  std::cout << "Is value error? " << SayYesOrNo(cpp2kdb::IsError(result))
            << std::endl;
  std::cout << "Is value atomic? " << SayYesOrNo(cpp2kdb::IsAtomic(result))
            << std::endl;
  std::cout << "Is value dict? " << SayYesOrNo(cpp2kdb::IsDict(result))
            << std::endl;
  std::cout << "Is value table? " << SayYesOrNo(cpp2kdb::IsTable(result))
            << std::endl;
  std::cout << "Result type is " << cpp2kdb::GetQTypeId(result) << std::endl;
  std::cout << "Size of the list is "
            << cpp2kdb::GetNumberOfVectorElements(result) << std::endl;
  void** key_value_list = static_cast<void**>(cpp2kdb::GetVector(result));
  std::cout << "Key's type is " << cpp2kdb::GetQTypeId(key_value_list[0])
            << " and the count is "
            << cpp2kdb::GetNumberOfVectorElements(key_value_list[0])
            << std::endl;
  const int number_of_elements =
      cpp2kdb::GetNumberOfVectorElements(key_value_list[1]);
  std::cout << "Value's type is " << cpp2kdb::GetQTypeId(key_value_list[1])
            << " and the count is " << number_of_elements << std::endl;
  std::vector<std::int64_t> values(number_of_elements);
  cpp2kdb::DataRetrievalResult value_result =
      cpp2kdb::RetrieveVectorData(key_value_list[1], values.data());
  std::cout << "Retrieving Data Successful? "
            << SayYesOrNo(value_result == cpp2kdb::DataRetrievalResult::Ok)
            << " " << static_cast<int>(value_result) << std::endl;
  for (std::size_t i = 0; i < values.size(); i++) {
    std::cout << values[i] << " ";
  }
  std::cout << std::endl;
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
  std::cout << "----------------------" << std::endl;
  TestIsSame();
  std::cout << "----------------------" << std::endl;
  TestDictionary(connection);
  cpp2kdb::CloseConnection(connection);
  return 0;
}
