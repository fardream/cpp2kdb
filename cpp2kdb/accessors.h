// Copyright (C) 2021, Chao Xu
//
// Part of cpp2kdb, which is released under BSD license. See LICENSE for full
// details.
#ifndef CPP2KDB_ACCESSORS_H__
#define CPP2KDB_ACCESSORS_H__
/// \file cpp2kdb/accessors.h
/// Convenient accessor functions using kdb_wrappers.h

#include <string>
#include <vector>

// Including the wrapper
#include "cpp2kdb/kdb_wrapper.h"
// Including the types.
#include "cpp2kdb/q_types.h"

namespace cpp2kdb::accessors {
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
  InvalidQTypeId
};

/// Check if the input is valid for retrieving data into a vector.
/// This support symbol vector, mixed list if all the elements are vector of
/// chars.
DataRetrievalResult CheckVectorForVectorDataRetrieval(void* input_vector);

/// Specialization for type std::string of Retrieving Data into Vector
DataRetrievalResult RetrieveVectorData(void* input_vector,
                                       std::string* output_vector);
/// Specialization for type void** of Retrieving Data into Vector.
/// This is for mixed type vector.
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
}  // namespace cpp2kdb::accessors
#endif  // CPP2KDB_ACCESSORS_H__
