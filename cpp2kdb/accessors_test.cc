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
  std::cout << "Type of char is "
            << cpp2kdb::q_types::q_type_id<char> << std::endl;
  std::cout
      << "Type of UID is "
      << cpp2kdb::q_types::q_type_id<cpp2kdb::q_types::QGuid> << std::endl;
  std::cout << "Type of byte is "
            << cpp2kdb::q_types::q_type_id<std::int8_t> << std::endl;
  std::cout << "Type of string is "
            << cpp2kdb::q_types::q_type_id<std::string> << std::endl;
  std::cout << "Type ID 1 is mapped to bool? "
            << IsSameType<cpp2kdb::q_types::CTypeForQTypeId<1>, bool>()
            << std::endl;
  std::cout << "Type ID 10 is mapped to bool? "
            << IsSameType<cpp2kdb::q_types::CTypeForQTypeId<10>, bool>()
            << std::endl;
  std::cout << "Type ID 10 is mapped to char? "
            << IsSameType<cpp2kdb::q_types::CTypeForQTypeId<10>, char>()
            << std::endl;
  std::cout << "Type ID 11 is mapped to std::string? "
            << IsSameType<cpp2kdb::q_types::CTypeForQTypeId<11>, std::string>()
            << std::endl;
}

void TestIsSame() {
  std::cout << "Is bool 1? "
            << SayYesOrNo(cpp2kdb::q_types::IsSameType<bool>(1)) << std::endl;
  std::cout << "Is GUID 2? "
            << SayYesOrNo(
                   cpp2kdb::q_types::IsSameType<cpp2kdb::q_types::QGuid>(2))
            << std::endl;
  std::cout << "Is std::int8_t 4? "
            << SayYesOrNo(cpp2kdb::q_types::IsSameType<std::int8_t>(4))
            << std::endl;
  std::cout << "Is short 5? "
            << SayYesOrNo(cpp2kdb::q_types::IsSameType<short>(5))  // NOLINT
            << std::endl;
  std::cout << "Is int 6? " << SayYesOrNo(cpp2kdb::q_types::IsSameType<int>(6))
            << std::endl;
  std::cout << "Is std::int64_t 7? "
            << SayYesOrNo(cpp2kdb::q_types::IsSameType<std::int64_t>(7))
            << std::endl;
  std::cout << "Is float 8? "
            << SayYesOrNo(cpp2kdb::q_types::IsSameType<float>(8))  // NOLINT
            << std::endl;
  std::cout << "Is double 9? "
            << SayYesOrNo(cpp2kdb::q_types::IsSameType<double>(9)) << std::endl;
  std::cout << "Is char 10? "
            << SayYesOrNo(cpp2kdb::q_types::IsSameType<char>(10)) << std::endl;
  std::cout << "Is std::string 11? "
            << SayYesOrNo(cpp2kdb::q_types::IsSameType<std::string>(11))
            << std::endl;
  std::cout << "Is std::string 10? "
            << SayYesOrNo(cpp2kdb::q_types::IsSameType<std::string>(10))
            << std::endl;
  std::cout << "Is std::string -10? "
            << SayYesOrNo(cpp2kdb::q_types::IsSameType<std::string>(-10))
            << std::endl;
  std::cout << "Is std::int64_t 12? "
            << SayYesOrNo(cpp2kdb::q_types::IsSameType<std::int64_t>(12))
            << std::endl;
  std::cout << "Is int 13? "
            << SayYesOrNo(cpp2kdb::q_types::IsSameType<int>(13)) << std::endl;
  std::cout << "Is int 14? "
            << SayYesOrNo(cpp2kdb::q_types::IsSameType<int>(14)) << std::endl;
  std::cout << "Is double 15? "
            << SayYesOrNo(cpp2kdb::q_types::IsSameType<double>(15))
            << std::endl;
  std::cout << "Is std::int64_t 16? "
            << SayYesOrNo(cpp2kdb::q_types::IsSameType<std::int64_t>(16))
            << std::endl;
  std::cout << "Is int 17? "
            << SayYesOrNo(cpp2kdb::q_types::IsSameType<int>(17)) << std::endl;
  std::cout << "Is int 18? "
            << SayYesOrNo(cpp2kdb::q_types::IsSameType<int>(18)) << std::endl;
  std::cout << "Is int 19? "
            << SayYesOrNo(cpp2kdb::q_types::IsSameType<int>(19)) << std::endl;
}

void TestDictionary(int connection) {
  std::string query =
      "(`Arthur`Dent; `Zaphod`Beeblebrox; `Ford`Prefect)! 100 42 150";
  void* result =
      cpp2kdb::kdb_wrapper::RunQueryOnConnection(connection, query.c_str());
  cpp2kdb::kdb_wrapper::DecreaseReferenceCountGuard guard(result);
  std::cout << "Is value error? "
            << SayYesOrNo(cpp2kdb::accessors::IsError(result)) << std::endl;
  std::cout << "Is value atomic? "
            << SayYesOrNo(cpp2kdb::accessors::IsAtomic(result)) << std::endl;
  std::cout << "Is value dict? "
            << SayYesOrNo(cpp2kdb::accessors::IsDict(result)) << std::endl;
  std::cout << "Is value table? "
            << SayYesOrNo(cpp2kdb::accessors::IsTable(result)) << std::endl;
  std::cout << "Result type is " << cpp2kdb::kdb_wrapper::GetQTypeId(result)
            << std::endl;
  std::cout << "Size of the list is "
            << cpp2kdb::kdb_wrapper::GetNumberOfVectorElements(result)
            << std::endl;
  void** key_value_list =
      static_cast<void**>(cpp2kdb::kdb_wrapper::GetVector(result));
  std::cout << "Key's type is "
            << cpp2kdb::kdb_wrapper::GetQTypeId(key_value_list[0])
            << " and the count is "
            << cpp2kdb::kdb_wrapper::GetNumberOfVectorElements(
                   key_value_list[0])
            << std::endl;
  const int number_of_elements =
      cpp2kdb::kdb_wrapper::GetNumberOfVectorElements(key_value_list[1]);
  std::cout << "Value's type is "
            << cpp2kdb::kdb_wrapper::GetQTypeId(key_value_list[1])
            << " and the count is " << number_of_elements << std::endl;
  std::vector<std::int64_t> values(number_of_elements);
  cpp2kdb::accessors::DataRetrievalResult value_result =
      cpp2kdb::accessors::RetrieveVectorData(key_value_list[1], values.data());
  std::cout << "Retrieving Data Successful? "
            << SayYesOrNo(value_result ==
                          cpp2kdb::accessors::DataRetrievalResult::Ok)
            << " " << static_cast<int>(value_result) << std::endl;
  for (std::size_t i = 0; i < values.size(); i++) {
    std::cout << values[i] << " ";
  }
  std::cout << std::endl;

  std::vector<void*> keys(3, nullptr);
  auto key_list_result =
      cpp2kdb::accessors::RetrieveVectorData(key_value_list[0], keys.data());
  std::cout << "Getting keys result is " << static_cast<int>(key_list_result)
            << std::endl;
  std::cout << "Type of first key is "
            << cpp2kdb::kdb_wrapper::GetQTypeId(keys[0]) << std::endl;
  std::cout << "Size of first key is "
            << cpp2kdb::kdb_wrapper::GetNumberOfVectorElements(keys[0])
            << std::endl;
  std::vector<std::string> first_key(
      cpp2kdb::kdb_wrapper::GetNumberOfVectorElements(keys[0]));
  cpp2kdb::accessors::RetrieveVectorData(keys[0], first_key.data());
  std::cout << "First keys are: ";
  for (std::size_t i = 0; i < first_key.size(); i++) {
    std::cout << first_key[i] << "|";
  }
  std::cout << std::endl;
}
void TestString(int connection) {
  std::string query = "(\"abc\";\"defg\";\"hi\")";
  void* result =
      cpp2kdb::kdb_wrapper::RunQueryOnConnection(connection, query.c_str());
  cpp2kdb::kdb_wrapper::DecreaseReferenceCountGuard guard(result);

  std::cout << "Is value error? "
            << SayYesOrNo(cpp2kdb::accessors::IsError(result)) << std::endl;
  std::cout << "Is value atomic? "
            << SayYesOrNo(cpp2kdb::accessors::IsAtomic(result)) << std::endl;
  std::cout << "Is value dict? "
            << SayYesOrNo(cpp2kdb::accessors::IsDict(result)) << std::endl;
  std::cout << "Is value table? "
            << SayYesOrNo(cpp2kdb::accessors::IsTable(result)) << std::endl;
  std::cout << "Result type is " << cpp2kdb::kdb_wrapper::GetQTypeId(result)
            << std::endl;
  std::cout << "Size of the list is "
            << cpp2kdb::kdb_wrapper::GetNumberOfVectorElements(result)
            << std::endl;
  std::vector<std::string> values(
      cpp2kdb::kdb_wrapper::GetNumberOfVectorElements(result));
  auto d = cpp2kdb::accessors::RetrieveVectorData(result, values.data());
  std::cout << "Retrieval result is " << d << std::endl;
  for (std::size_t i = 0; i < values.size(); i++) {
    std::cout << values[i] << "|";
  }
  std::cout << std::endl;

  std::string query1 = "\"abc\"";
  void* result1 =
      cpp2kdb::kdb_wrapper::RunQueryOnConnection(connection, query1.c_str());
  std::cout << "Type of query " << query1 << " is "
            << cpp2kdb::kdb_wrapper::GetQTypeId(result1) << std::endl;
  auto d1 = cpp2kdb::accessors::RetrieveVectorData(result1, values.data());
  std::cout << "Retrieve result from query " << query1 << " is " << d1
            << std::endl;
}

void TestTable(int connection) {
  std::string query =
      "([]B:01010b;X:0x0001020304;H:0 1 2 3 4h;I:0 1 2 3 4i;J:0 1 2 3 4j;E:0 1 "
      "2 3 4e;F:0 1 2 3 4.;C:\"abcde\";S:`a`b`c`d`e;P:.z.p+til "
      "5;M:2001.01m+til 5;D:2001.01.01+til 5;Z:.z.z+til 5;N:.z.n+til "
      "5;U:10:00:00+til 5;T:10:00:00.000+til 5)";
  std::cout << "Query to run is: " << std::endl << query << std::endl;
  void* result =
      cpp2kdb::kdb_wrapper::RunQueryOnConnection(connection, query.c_str());
  if (result == nullptr) {
    std::cout << "Result is nullptr" << std::endl;
  }

  cpp2kdb::kdb_wrapper::DecreaseReferenceCountGuard guard(result);
  std::cout << "Type of result is " << cpp2kdb::kdb_wrapper::GetQTypeId(result)
            << std::endl;
  std::size_t nc, nr;
  void* column_heading;
  void** values;
  if (cpp2kdb::accessors::GetSimpleTable(result, &column_heading, &values, &nc,
                                         &nr) ==
      cpp2kdb::accessors::DataRetrievalResult::Ok) {
    std::cout << "Shape of the table is: row count " << nr << " column count "
              << nc << std::endl;
    std::vector<std::string> column_names(nc);
    if (cpp2kdb::accessors::RetrieveVectorData(column_heading,
                                               column_names.data()) !=
        cpp2kdb::accessors::DataRetrievalResult::Ok) {
      return;
    }
    for (std::size_t i = 0; i < column_names.size(); i++) {
      std::cout << "column " << i << ": " << column_names[i]
                << ", column type is "
                << cpp2kdb::kdb_wrapper::GetQTypeId(values[i]) << std::endl;
    }
    std::vector<std::string> s_column(nr);
    if (cpp2kdb::accessors::RetrieveVectorData(values[8], s_column.data()) ==
        cpp2kdb::accessors::DataRetrievalResult::Ok) {
      for (std::size_t i = 0; i < s_column.size(); i++) {
        std::cout << s_column[i] << " ";
      }
    }
    std::cout << std::endl;
  }

  return;
}
void TestKeyedTable(int connection) {
  std::string query =
      "([eid:1001 1002 1003] name:`Dent`Beeblebrox`Prefect; iq:98 42 126)";
  std::cout << "Query to run is: " << std::endl << query << std::endl;
  void* result =
      cpp2kdb::kdb_wrapper::RunQueryOnConnection(connection, query.c_str());
  if (result == nullptr) {
    std::cout << "Result is nullptr" << std::endl;
  }
  std::cout << "Type of result is " << cpp2kdb::kdb_wrapper::GetQTypeId(result)
            << std::endl;

  void** key_value_list =
      reinterpret_cast<void**>(cpp2kdb::kdb_wrapper::GetVector(result));
  std::cout << "Type of the first k is "
            << cpp2kdb::kdb_wrapper::GetQTypeId(key_value_list[0])
            << " and the type of the second K is "
            << cpp2kdb::kdb_wrapper::GetQTypeId(key_value_list[1]) << std::endl;

  void* temp_result = result;
  result = cpp2kdb::kdb_wrapper::DekeyKeyedTable(temp_result);

  cpp2kdb::kdb_wrapper::DecreaseReferenceCountGuard guard(result);

  std::size_t nc, nr;
  void* column_heading;
  void** values;
  if (cpp2kdb::accessors::GetSimpleTable(result, &column_heading, &values, &nc,
                                         &nr) ==
      cpp2kdb::accessors::DataRetrievalResult::Ok) {
    std::cout << "Shape of the table is: row count " << nr << " column count "
              << nc << std::endl;
    std::vector<std::string> column_names(nc);
    if (cpp2kdb::accessors::RetrieveVectorData(column_heading,
                                               column_names.data()) !=
        cpp2kdb::accessors::DataRetrievalResult::Ok) {
      return;
    }
    for (std::size_t i = 0; i < column_names.size(); i++) {
      std::cout << "column " << i << ": " << column_names[i]
                << ", column type is "
                << cpp2kdb::kdb_wrapper::GetQTypeId(values[i]) << std::endl;
    }
    std::vector<std::string> s_column(nr);
    if (cpp2kdb::accessors::RetrieveVectorData(values[1], s_column.data()) ==
        cpp2kdb::accessors::DataRetrievalResult::Ok) {
      for (std::size_t i = 0; i < s_column.size(); i++) {
        std::cout << s_column[i] << " ";
      }
    }
    std::cout << std::endl;
  }

  return;
}
}  // namespace

int main(int argc, char** argv) {
  // Open connection
  int connection = cpp2kdb::kdb_wrapper::OpenConnection("127.0.0.1", 5000, "");

  if (connection <= 0) {
    std::cerr << "Connection error: " << connection << std::endl;
    return 1;
  }

  TestTypes(connection);
  std::cout << "----------------------" << std::endl;
  TestIsSame();
  std::cout << "----------------------" << std::endl;
  TestDictionary(connection);
  std::cout << "----------------------" << std::endl;
  TestString(connection);
  std::cout << "----------------------" << std::endl;
  TestTable(connection);
  std::cout << "----------------------" << std::endl;
  TestKeyedTable(connection);
  cpp2kdb::kdb_wrapper::CloseConnection(connection);
  return 0;
}
