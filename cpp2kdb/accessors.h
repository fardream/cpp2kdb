// Copyright (C) 2021, Chao Xu
//
// Part of cpp2kdb, which is released under BSD license. See LICENSE for full
// details.
#ifndef CPP2KDB_ACCESSORS_H__
#define CPP2KDB_ACCESSORS_H__
/// \file cpp2kdb/accessors.h
/// Convenient accessor functions using kdbwrappers.h

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <string>
#include <type_traits>
#include <vector>

// Including the wrapper
#include "cpp2kdb/kdbwrapper.h"
// Including the types.
#include "cpp2kdb/q_types.h"

namespace cpp2kdb {
/// Check if C Type T is mapped to input_q_type_id
template <typename T>
constexpr bool IsCTypeMappedToQTypeId(
    /// Input Q Type Id
    int input_q_type_id) {
  return
      // Not Error
      input_q_type_id != q_error_type_id &&
      // mapped id is input_q_type_id
      q_type_id<T> ==
          (input_q_type_id > 0 ? input_q_type_id : -input_q_type_id);
}

/// Check if input_q_type_id is mapped to int
///
/// This works both for atomic types and vector types.
constexpr bool IsQTypeIdInt(
    /// Input Q Type Id
    int input_q_type_id) {
  // If the input is negative, make it positive.
  int positive_q_type_id =
      input_q_type_id > 0 ? input_q_type_id : -input_q_type_id;
  // Iterate through all the possible types, including int.
  return positive_q_type_id == q_time_type_id ||
         positive_q_type_id == q_month_type_id ||
         positive_q_type_id == q_date_type_id ||
         positive_q_type_id == q_minute_type_id ||
         positive_q_type_id == q_second_type_id ||
         positive_q_type_id == q_type_id<int>;
}

/// Check if input_q_type_id is mapped to std::int64_t
constexpr bool IsQTypeIdInt64(
    /// Input Q Type Id
    int input_q_type_id) {
  // If the input is negative, make it positive.
  int positive_q_type_id =
      input_q_type_id > 0 ? input_q_type_id : -input_q_type_id;
  return positive_q_type_id == q_timestamp_type_id ||
         positive_q_type_id == q_timespan_type_id ||
         positive_q_type_id == q_type_id<std::int64_t>;
}

/// Check if input_q_type_id is mapped to double
constexpr bool IsQTypeIdDouble(
    /// Input Q Type Id
    int input_q_type_id) {
  // If the input is negative, make it positive.
  int positive_q_type_id =
      input_q_type_id > 0 ? input_q_type_id : -input_q_type_id;
  return positive_q_type_id == q_datetime_type_id ||
         positive_q_type_id == q_type_id<double>;
}

/// Check if input_q_type_id is mapped to std::string.
/// char list, which is type 10, is also a std;;string, but not -10, which is a
/// single char.
constexpr bool IsQTypeIdString(
    /// Input Q Type Id
    int input_q_type_id) {
  return input_q_type_id == q_type_id<std::string> ||
         input_q_type_id == q_type_id<char>;
}

/// Check if the type indicted by q_type_id is the same as T
/// \tparam T input c_type to check.
template <typename T>
constexpr bool IsSameType(
    /// Input q type id
    int input_q_type_id) {
  return  // Check if C type is mapped directly to this id.
      IsCTypeMappedToQTypeId<T>(input_q_type_id) ||
      // Check if the ID is an int
      (std::is_same_v<int, T> && IsQTypeIdInt(input_q_type_id)) ||
      // Check if the ID is a std::int64_t
      (std::is_same_v<std::int64_t, T> && IsQTypeIdInt64(input_q_type_id)) ||
      // Check if the ID is a double
      (std::is_same_v<double, T> && IsQTypeIdDouble(input_q_type_id)) ||
      // Check if the ID is a string
      (std::is_same_v<std::string, T> && IsQTypeIdString(input_q_type_id));
}

/// Check if the q type id is -128/error.
constexpr bool IsQTypeIdError(int input_q_type_id) {
  return input_q_type_id == q_error_type_id;
}

/// Check if the q type id is atomic
constexpr bool IsQTypeIdAtomic(int input_q_type_id) {
  return input_q_type_id < 0 && !IsQTypeIdError(input_q_type_id);
}

/// Check if the q type id is table/98.
/// Note this does **NOT** check if it's keyed table.
constexpr bool IsQTypeIdTable(int input_q_type_id) {
  return input_q_type_id == q_table_type_id;
}

/// Check if the q type id is dict.
/// Note the actual may be a keyed table, which is also a dict.
constexpr bool IsQTypeIdDict(int input_q_type_id) {
  return input_q_type_id == q_dict_type_id;
}

/// Check if the q type id is a vector
///
/// This checks if q type id is >= 0, not a table or dict
constexpr bool IsQTypeIdVector(int input_q_type_id) {
  return input_q_type_id >= 0 && !IsQTypeIdTable(input_q_type_id) &&
         !IsQTypeIdDict(input_q_type_id);
}

/// Check if the q type id is a mixed vector.
///
/// Thsi checks if the q type id is 0.
constexpr bool IsQTypeIdMixedVector(int input_q_type_id) {
  return input_q_type_id == q_mixed_type_id;
}

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

/// Copy Data if NQTypeId matches input_q_type_id.
/// First check if NQTypeId and input_q_type_id matches. If they don't match, do
/// nothing. If they match, Copy data from input_data pointer into output_data
/// pointer. The input_data pointer will be cast into a pointer of NQTypeId
/// indicated type.
/// \returns a bool indicates if the type matches and copy happened.
template <typename T, int NQTypeId>
bool CopyDataByQTypeIQAndType(
    /// Q Type Id of void*
    int input_q_type_id,
    /// Number of elements to copy.
    std::size_t number_of_elements,
    /// Input data, which is the vector data from K, not the pointer
    /// to K itself.
    void* input_data,
    /// Output data.
    T* output_data) {
  // Check if the two q type id matches
  if (input_q_type_id != NQTypeId) {
    return false;
  }
  // Correctly typed input data
  CTypeForQTypeId<NQTypeId>* typed_input_data =
      reinterpret_cast<CTypeForQTypeId<NQTypeId>*>(input_data);

  // Copy
  std::copy_n(typed_input_data, number_of_elements, output_data);

  return true;
}

/// Copy from a void* indicated by q_type_id into a T*.
///
/// \tparam NQTypeId A q_type_id indicating the type of the data.
/// \tparam T Desired type for output.
template <typename T, int... NQTypeIds>
DataRetrievalResult TryCopyDataByQTypeIdAndType(
    /// Q Type Id of void*
    int input_q_type_id,
    /// Number of elements to copy.
    std::size_t number_of_elements,
    /// Input data, which is the vector data from K, not the pointer
    /// to K itself.
    void* input_data,
    /// Output data.
    T* output_data) {
  // Check if a match found for input_q_type_id
  bool found_q_type_id =
      (CopyDataByQTypeIQAndType<T, NQTypeIds>(
           input_q_type_id, number_of_elements, input_data, output_data) ||
       ...);
  /// Return ok if q_type_id is found, otherwise InvalidQTypeId
  return found_q_type_id ? DataRetrievalResult::Ok
                         : DataRetrievalResult::InvalidQTypeId;
}

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
  if (IsMixedVector(input_vector) ||
      GetQTypeId(input_vector) == q_type_id<std::string>) {
    return DataRetrievalResult::NotNumericalVector;
  }

  // Now copy the data over.
  return TryCopyDataByQTypeIdAndType<
      T, q_boolean_type_id, q_byte_type_id, q_short_type_id, q_int_type_id,
      q_long_type_id, q_real_type_id, q_float_type_id, q_char_type_id,
      q_timestamp_type_id, q_month_type_id, q_date_type_id, q_datetime_type_id,
      q_timespan_type_id, q_minute_type_id, q_second_type_id, q_time_type_id>(
      GetQTypeId(input_vector), GetNumberOfVectorElements(input_vector),
      GetVector(input_vector), output_vector);
}
}  // namespace cpp2kdb
#endif  // CPP2KDB_ACCESSORS_H__
