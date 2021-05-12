// Copyright (C) 2021, Chao Xu
//
// Part of cpp2kdb, which is released under BSD license. See LICENSE for full
// details.
#ifndef CPP2KDB_ACCESSORS_H__
#define CPP2KDB_ACCESSORS_H__
/// \file cpp2kdb/accessors.h
/// Convenient accessor functions using kdbwrappers.h

#include <cstdint>
#include <string>
#include <type_traits>
#include <vector>

// Including the wrapper
#include "cpp2kdb/kdbwrapper.h"

namespace cpp2kdb {
/// Define the 16 byte GUID type (or U) in KDB.
struct QGuid {
  /// Value of the GUID. 4 4-byte int's should be 16 bytes.
  int value[4];
};

static_assert(sizeof(QGuid) == 16, "QGuid type should be 16");

/// Getting the Q Type Id of the given C++ Type.
/// This will return the vector type of the corresponding atom type.
/// For example, for char in q, it will return 10 (instead of -10).
template <typename T>
class QTypeId {
 public:
  /// Corresponding Q Type ID.
  /// Default to -128 (Error)
  constexpr static const int type_id = -128;
};

/// Helper variable for q_type_id
template <typename T>
inline constexpr const int q_type_id = QTypeId<T>::type_id;

/// Getting the C Type from the given Q Type ID.
/// Reference for the c_type in the class.
template <int NQTypeId>
class CTypeForQTypeId {
 public:
  /// Default c_type is void
  typedef void CType;
};

/// Q Type ID for bool, which is boolean in q, KB=1.
/// Note q uses C type char for this.
template <>
class QTypeId<bool> {
 public:
  /// Type boolean, KB=1
  constexpr static const int type_id = 1;
};
/// C Type for Q Type ID 1, which is bool
template <>
class CTypeForQTypeId<QTypeId<bool>::type_id> {
 public:
  typedef bool CType;
};

/// Q Type ID for QGuid, which is guid in q, UU=2
template <>
class QTypeId<QGuid> {
 public:
  /// Type GUID, UU=2
  constexpr static const int type_id = 2;
};
/// C Type for Q Type ID 2, which is QQuid
template <>
class CTypeForQTypeId<q_type_id<QGuid>> {
 public:
  typedef QGuid CType;
};

/// Q Type ID for std::int8_t, which is byte in q, KG=4.
/// Note q uses C type char for this.
template <>
class QTypeId<std::int8_t> {
 public:
  /// Type byte, KG=4
  constexpr static const int type_id = 4;
};
/// C Type for Q Type ID 4, which is QQuid
template <>
class CTypeForQTypeId<q_type_id<std::int8_t>> {
 public:
  typedef std::int8_t CType;
};

/// Q Type ID for short, which is short in q, KH=5.
template <>
class QTypeId<short> {  // NOLINT
 public:
  /// Type short, KH=5
  constexpr static const int type_id = 5;
};
/// C Type for Q Type ID 5, which is short
template <>
class CTypeForQTypeId<q_type_id<short>> {  // NOLINT
 public:
  typedef short CType;  // NOLINT
};

/// Q Type ID for int, which is int in q, KI=6.
template <>
class QTypeId<int> {
 public:
  /// Type int, KI=6
  constexpr static const int type_id = 6;
};
/// C Type for Q Type ID 6, which is int
template <>
class CTypeForQTypeId<q_type_id<int>> {
 public:
  typedef int CType;
};

/// Q Type ID for int64_t, which is long in q, KJ=7.
template <>
class QTypeId<std::int64_t> {
 public:
  /// Type long, KJ=7
  constexpr static const int type_id = 7;
};
/// C Type for Q Type ID 7, which is std::int64_t
template <>
class CTypeForQTypeId<q_type_id<std::int64_t>> {
 public:
  typedef std::int64_t CType;
};

/// Q Type ID for float, which is real in q, KE=8.
template <>
class QTypeId<float> {
 public:
  /// Type real, KE=8
  constexpr static const int type_id = 8;
};
/// C Type for Q Type ID 8, which is float
template <>
class CTypeForQTypeId<q_type_id<float>> {
 public:
  typedef float CType;
};

/// Q Type ID for double, which is float  in q, KF=9.
/// Notice that in q, float is 8 byte double in C++.
template <>
class QTypeId<double> {
 public:
  /// Type float, KF=9
  constexpr static const int type_id = 9;
};
/// C Type for Q Type ID 9, which is double
template <>
class CTypeForQTypeId<q_type_id<double>> {
 public:
  typedef double CType;
};

/// Q Type ID for char, which is char  in q, KC=10.
template <>
class QTypeId<char> {
 public:
  /// Type char, KC=10
  constexpr static const int type_id = 10;
};
/// C Type for Q Type ID 10, which is char
template <>
class CTypeForQTypeId<q_type_id<char>> {
 public:
  typedef char CType;
};

/// Q Type ID for std::string, which is symbol in q, KS=11
template <>
class QTypeId<std::string> {
 public:
  /// Type symbol, KS=11
  constexpr static const int type_id = 11;
};
/// C Type for Q Type ID 10, which is std::string
template <>
class CTypeForQTypeId<q_type_id<std::string>> {
 public:
  typedef std::string CType;
};

// Some Q Types have no obvious corresponding C Type.
//
/// Q Type Id for Timestamp
constexpr const int q_timestamp_type_id = 12;
/// C Type for Q Type ID 12, which is std::int64_t
template <>
class CTypeForQTypeId<q_timestamp_type_id> {
 public:
  typedef std::int64_t CType;
};

/// Q Type Id for Month
constexpr const int q_month_type_id = 13;
/// C Type for Q Type ID 13, which is int
template <>
class CTypeForQTypeId<q_month_type_id> {
 public:
  typedef int CType;
};

/// Q Type Id for Date
constexpr const int q_date_type_id = 14;
/// C Type for Q Type ID 13, which is int
template <>
class CTypeForQTypeId<q_date_type_id> {
 public:
  typedef int CType;
};

/// Q Type Id for Datetime
constexpr const int q_datetime_type_id = 15;
/// C Type for Q Type ID 13, which is int
template <>
class CTypeForQTypeId<q_datetime_type_id> {
 public:
  typedef double CType;
};

/// Q Type Id for Timespan
constexpr const int q_timespan_type_id = 16;
/// C Type for Q Type ID 13, which is int
template <>
class CTypeForQTypeId<q_timespan_type_id> {
 public:
  typedef std::int64_t CType;
};

/// Q Type Id for Minute
constexpr const int q_minute_type_id = 17;
/// C Type for Q Type ID 13, which is int
template <>
class CTypeForQTypeId<q_minute_type_id> {
 public:
  typedef int CType;
};

/// Q Type Id for Second
constexpr const int q_second_type_id = 18;
/// C Type for Q Type ID 13, which is int
template <>
class CTypeForQTypeId<q_second_type_id> {
 public:
  typedef int CType;
};

/// Q Type Id for Time
constexpr const int q_time_type_id = 19;
/// C Type for Q Type ID 13, which is int
template <>
class CTypeForQTypeId<q_time_type_id> {
 public:
  typedef int CType;
};

/// Q Type Id for Table/Flip
constexpr const int q_table_type_id = 98;

/// Q Type Id for Dict/Keyed Table
constexpr const int q_dict_type_id = 99;

/// Q Type Id for error state
constexpr const int q_error_type_id = -128;

/// Q Type Id for mixed type list
constexpr const int q_mixed_type_id = 0;

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
}  // namespace cpp2kdb
#endif  // CPP2KDB_ACCESSORS_H__
