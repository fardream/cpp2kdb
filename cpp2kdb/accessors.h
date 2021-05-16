// Copyright (C) 2021, Chao Xu
//
// Part of cpp2kdb, which is released under BSD license. See LICENSE for full
// details.
#ifndef CPP2KDB_ACCESSORS_H__
#define CPP2KDB_ACCESSORS_H__
/// \file cpp2kdb/accessors.h
/// Convenient accessor functions using kdb_wrappers.h

#include <iostream>
#include <string>
#include <vector>

// Including the wrapper
#include "cpp2kdb/kdb_wrapper.h"
// Including the types.
#include "cpp2kdb/q_types.h"

/// Pure C++ to KDB
namespace cpp2kdb {
/// Convenient accessor functions for kdb using kdb_wrapper.h
namespace accessors {
/// Check if the value is error.
///
/// In reality, this checks for if type_id == -128
bool IsError(void* x);

/// Check if the value is atomic (a.k.a not a vector)
///
/// This checks if the type_id is negative and not an error.
bool IsAtomic(void* x);

/// Check if the value is vector
///
/// This checks if the type_id is >= 0 and not dictionary or table
bool IsVector(void* x);

/// Check if the value is a mixed vector
///
/// This checks if the type_id is 0.
bool IsMixedVector(void* x);

/// Check if the value is a dictionary or keyed table.
///
/// This checks if the type_id is 99
bool IsDict(void* x);

/// Check if the value is a table
///
/// This checks if the type_id is 98
bool IsTable(void* x);

/// Data Retrieval Result
enum class DataRetrievalResult {
  /// Ok.
  Ok = 0,
  /// Input pointer is nullptr
  NullInput,
  /// Input k0 object is of type -128, error.
  ValueError,
  /// Not a table (a.k.a type is not 98)
  NotTable,
  /// Not a dictionary (a.k.a type is not 99)
  NotDictionary,
  /// Not a keyed table (a.k.a type is not 98 and 99)
  NotKeyedTable,
  /// Not a vector
  NotVector,
  /// Requesting Numerical Vector But Get a Mixed Vector or Symbol Vector (Note
  /// char vector is valid).
  NotNumericalVector,
  /// Not a valid Q Type Id
  InvalidQTypeId,
  /// Not a mixed vector
  NotMixedVector,
  /// Not a std::string vector
  NotStringVector,
  /// Mixed Vector Contains an element that is not char vector.
  /// This happens when std::string vector is requested, and a mixed list is
  /// input. The mixed input contains an element that is not a char vector.
  NotCharVectorInMixedVector,
  /// Not simple type (type is not 98)
  NotSimpleTable
};

/// Names for the enums....
/// There got to be a better way.
constexpr const char* DataRetrievalResultNames[] = {
    "Ok",
    "NullInput",
    "ValueError",
    "NotTable",
    "NotDictionary",
    "NotKeyedTable",
    "NotVector",
    "NotNumericalVector",
    "InvalidQTypeId",
    "NotMixedVector",
    "NotStringVector",
    "NotCharVectorInMixedVector",
    "NotSimpleTable"};

/// Number of data retrieval result names
constexpr const std::size_t number_of_data_retrieval_result_names =
    sizeof(DataRetrievalResultNames) / sizeof(DataRetrievalResultNames[0]);

/// Get the name from a result
const char* GetDataRetrievalResultName(DataRetrievalResult result);

/// Overload << for reuslt type.
std::ostream& operator<<(std::ostream& output_stream,
                         DataRetrievalResult result);

/// Check if the input is valid for retrieving data into a vector.
/// This support symbol vector, mixed list if all the elements are vector of
/// chars.
DataRetrievalResult CheckVectorForVectorDataRetrieval(void* input_vector);

/// Get std::string from a char vector.
///
/// In kdb, string is actually char vector, but without the terminating \0.
/// Symbols are \0 terminated strings in C. The function doesn't perform check
/// on the type. It takes the pointer defined by the k and the number of chars
/// and construct a std::string
std::string GetStringFromCharVector(void* input_char_vector);

/// Specialization for type std::string of Retrieving Data into Vector.
/// std::string can be either a vector of symbol (type 11), or a mixed vector
/// with each element a vector of char (type 0, and then type 10 for all
/// elements).
DataRetrievalResult RetrieveVectorData(void* input_vector,
                                       std::string* output_vector);

/// Specialization for type void** of Retrieving Data into Vector.
/// This is for mixed type vector (so the q type id should be 0).
DataRetrievalResult RetrieveVectorData(void* input_vector,
                                       void** output_vector);

/// Retrieve Data Into Vector.
///
/// Note that std::string and void* (k) have corresponding overload and
/// therefore only arithmatic vectors should reach this - which is boolean,
/// char, integers and float.
template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
DataRetrievalResult RetrieveVectorData(void* input_vector, T* output_vector) {
  // Check the vector first.
  DataRetrievalResult check_vector_result =
      CheckVectorForVectorDataRetrieval(input_vector);
  // return if not OK.
  if (check_vector_result != DataRetrievalResult::Ok) {
    return check_vector_result;
  }
  // Check if this is mixed vector or symbol vector
  if (IsMixedVector(input_vector) || kdb_wrapper::GetQTypeId(input_vector) ==
                                         q_types::q_type_id<std::string>) {
    return DataRetrievalResult::NotNumericalVector;
  }

  // Now copy the data over.
  if (q_types::TryCopyDataByQTypeIdAndType<T>(
          kdb_wrapper::GetQTypeId(input_vector),
          kdb_wrapper::GetNumberOfVectorElements(input_vector),
          kdb_wrapper::GetVector(input_vector), output_vector)) {
    return DataRetrievalResult::Ok;
  } else {
    return DataRetrievalResult::InvalidQTypeId;
  }
}

/// Get all the data a simple table.
///
/// A simple table is type 98 K. It contains an atomic K pointing to a
/// dictionary. The dictionary's keys are the columns, and the values are all
/// the values by column. The function will also get the number of columns and
/// the rows.
DataRetrievalResult GetSimpleTable(
    /// Input simple table.
    void* simple_table,
    /// Output, K object containing the column heading
    void** column_heading,
    /// Output, an list of k object containing by column
    void*** values,
    /// Output, number of columns.
    std::size_t* number_of_columns,
    /// Output, number of rows.
    std::size_t* number_of_rows);
}  // namespace accessors
}  // namespace cpp2kdb
#endif  // CPP2KDB_ACCESSORS_H__
