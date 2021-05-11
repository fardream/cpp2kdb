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
}  // namespace cpp2kdb
#endif  // CPP2KDB_ACCESSORS_H__
