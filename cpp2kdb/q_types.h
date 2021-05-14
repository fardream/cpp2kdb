// Copyright (C) 2021, Chao Xu
//
// Part of cpp2kdb, which is released under BSD license. See LICENSE for full
// details.
#ifndef CPP2KDB_Q_TYPES_H__
#define CPP2KDB_Q_TYPES_H__
/// \file cpp2kdb/q_types.h

#include <cstdint>
#include <string>

namespace cpp2kdb {
/// Define the 16 byte GUID type (or U) in KDB.
struct QGuid {
  /// Value of the GUID. 4 4-byte int's should be 16 bytes.
  int value[4];
};

// Make sure QGuid is 16 bytes
static_assert(sizeof(QGuid) == 16, "QGuid type should be 16");

/// Getting the Q Type Id of the given C++ Type.
/// This will return the vector type of the corresponding atom type.
/// For example, for char in q, it will return 10 (instead of -10).
/// \tparam T C Type
template <typename T>
class QTypeId {};
/// Helper variable for q_type_id
template <typename T>
inline constexpr const int q_type_id = QTypeId<T>::type_id;

/// Getting the C Type from the given Q Type ID.
/// Reference for the c_type in the class.
/// \tparam NQTypeId Input Q Type Id
template <int NQTypeId>
class CTypeForQTypeIdHolder {};
/// C Type for the corresponding Q Type Id
template <int NQTypeId>
using CTypeForQTypeId = typename CTypeForQTypeIdHolder<NQTypeId>::CType;

/////////////////////////////////////////////////////////////////////////
/// Q Type Id for C Type bool, which is boolean in q, KB=1
/// Note in q, this is supposed to be mapped to char.
template <>
class QTypeId<bool> {
 public:
  /// Q Type boolean, KB=1
  constexpr static const int type_id = 1;
};
/// Helper variable for Q Type Id for boolean
constexpr const int q_boolean_type_id = q_type_id<bool>;

/// C Type for Q Type boolean with KB=1
template <>
class CTypeForQTypeIdHolder<q_boolean_type_id> {
 public:
  /// Map boolean/1/KB to bool
  typedef bool CType;
};

/////////////////////////////////////////////////////////////////////////
/// Q Type Id for C Type QGuid, which is guid in q, UU=2
///
template <>
class QTypeId<QGuid> {
 public:
  /// Q Type guid, UU=2
  constexpr static const int type_id = 2;
};
/// Helper variable for Q Type Id for guid
constexpr const int q_guid_type_id = q_type_id<QGuid>;

/// C Type for Q Type guid with UU=2
template <>
class CTypeForQTypeIdHolder<q_guid_type_id> {
 public:
  /// Map guid/2/UU to QGuid
  typedef QGuid CType;
};

/////////////////////////////////////////////////////////////////////////
/// Q Type Id for C Type std::int8_t, which is byte in q, KG=4
///
template <>
class QTypeId<std::int8_t> {
 public:
  /// Q Type byte, KG=4
  constexpr static const int type_id = 4;
};
/// Helper variable for Q Type Id for byte
constexpr const int q_byte_type_id = q_type_id<std::int8_t>;

/// C Type for Q Type byte with KG=4
template <>
class CTypeForQTypeIdHolder<q_byte_type_id> {
 public:
  /// Map byte/4/KG to std::int8_t
  typedef std::int8_t CType;
};

/////////////////////////////////////////////////////////////////////////
/// Q Type Id for C Type short, which is short in q, KH=5
///
template <>
class QTypeId<short> {  // NOLINT
 public:
  /// Q Type short, KH=5
  constexpr static const int type_id = 5;  // NOLINT
};
/// Helper variable for Q Type Id for short
constexpr const int q_short_type_id = q_type_id<short>;

/// C Type for Q Type short with KH=5
template <>
class CTypeForQTypeIdHolder<q_short_type_id> {
 public:
  /// Map short/5/KH to short
  typedef short CType;
};

/////////////////////////////////////////////////////////////////////////
/// Q Type Id for C Type int, which is int in q, KH=6
///
template <>
class QTypeId<int> {
 public:
  /// Q Type int, KH=6
  constexpr static const int type_id = 6;
};
/// Helper variable for Q Type Id for int
constexpr const int q_int_type_id = q_type_id<int>;

/// C Type for Q Type int with KH=6
template <>
class CTypeForQTypeIdHolder<q_int_type_id> {
 public:
  /// Map int/6/KH to int
  typedef int CType;
};

/////////////////////////////////////////////////////////////////////////
/// Q Type Id for C Type std::int64_t, which is long in q, KJ=7
///
template <>
class QTypeId<std::int64_t> {
 public:
  /// Q Type long, KJ=7
  constexpr static const int type_id = 7;
};
/// Helper variable for Q Type Id for long
constexpr const int q_long_type_id = q_type_id<std::int64_t>;

/// C Type for Q Type long with KJ=7
template <>
class CTypeForQTypeIdHolder<q_long_type_id> {
 public:
  /// Map long/7/KJ to std::int64_t
  typedef std::int64_t CType;
};

/////////////////////////////////////////////////////////////////////////
/// Q Type Id for C Type float, which is real in q, KE=8
/// Note q type float is double in C type.
template <>
class QTypeId<float> {
 public:
  /// Q Type real, KE=8
  constexpr static const int type_id = 8;
};
/// Helper variable for Q Type Id for real
constexpr const int q_real_type_id = q_type_id<float>;

/// C Type for Q Type real with KE=8
template <>
class CTypeForQTypeIdHolder<q_real_type_id> {
 public:
  /// Map real/8/KE to float
  typedef float CType;
};

/////////////////////////////////////////////////////////////////////////
/// Q Type Id for C Type double, which is float in q, KF=9
/// Note q type float is double in C type.
template <>
class QTypeId<double> {
 public:
  /// Q Type float, KF=9
  constexpr static const int type_id = 9;
};
/// Helper variable for Q Type Id for float
constexpr const int q_float_type_id = q_type_id<double>;

/// C Type for Q Type float with KF=9
template <>
class CTypeForQTypeIdHolder<q_float_type_id> {
 public:
  /// Map float/9/KF to double
  typedef double CType;
};

/////////////////////////////////////////////////////////////////////////
/// Q Type Id for C Type char, which is char in q, KC=10
///
template <>
class QTypeId<char> {
 public:
  /// Q Type char, KC=10
  constexpr static const int type_id = 10;
};
/// Helper variable for Q Type Id for char
constexpr const int q_char_type_id = q_type_id<char>;

/// C Type for Q Type char with KC=10
template <>
class CTypeForQTypeIdHolder<q_char_type_id> {
 public:
  /// Map char/10/KC to char
  typedef char CType;
};

/////////////////////////////////////////////////////////////////////////
/// Q Type Id for C Type std::string, which is symbol in q, KS=11
///
template <>
class QTypeId<std::string> {
 public:
  /// Q Type symbol, KS=11
  constexpr static const int type_id = 11;
};
/// Helper variable for Q Type Id for symbol
constexpr const int q_symbol_type_id = q_type_id<std::string>;

/// C Type for Q Type symbol with KS=11
template <>
class CTypeForQTypeIdHolder<q_symbol_type_id> {
 public:
  /// Map symbol/11/KS to std::string
  typedef std::string CType;
};

/////////////////////////////////////////////////////////////////////////
// No C Type is mapped to q type timestamp
/// Helper variable for Q Type Id for timestamp
constexpr const int q_timestamp_type_id = 12;

/// C Type for Q Type timestamp with KP=12
template <>
class CTypeForQTypeIdHolder<q_timestamp_type_id> {
 public:
  /// Map timestamp/12/KP to std::int64_t
  typedef std::int64_t CType;
};

/////////////////////////////////////////////////////////////////////////
// No C Type is mapped to q type month
/// Helper variable for Q Type Id for month
constexpr const int q_month_type_id = 13;

/// C Type for Q Type month with KM=13
template <>
class CTypeForQTypeIdHolder<q_month_type_id> {
 public:
  /// Map month/13/KM to int
  typedef int CType;
};

/////////////////////////////////////////////////////////////////////////
// No C Type is mapped to q type date
/// Helper variable for Q Type Id for date
constexpr const int q_date_type_id = 14;

/// C Type for Q Type date with KD=14
template <>
class CTypeForQTypeIdHolder<q_date_type_id> {
 public:
  /// Map date/14/KD to int
  typedef int CType;
};

/////////////////////////////////////////////////////////////////////////
// No C Type is mapped to q type datetime
/// Helper variable for Q Type Id for datetime
constexpr const int q_datetime_type_id = 15;

/// C Type for Q Type datetime with KZ=15
template <>
class CTypeForQTypeIdHolder<q_datetime_type_id> {
 public:
  /// Map datetime/15/KZ to double
  typedef double CType;
};

/////////////////////////////////////////////////////////////////////////
// No C Type is mapped to q type timespan
/// Helper variable for Q Type Id for timespan
constexpr const int q_timespan_type_id = 16;

/// C Type for Q Type timespan with KN=16
template <>
class CTypeForQTypeIdHolder<q_timespan_type_id> {
 public:
  /// Map timespan/16/KN to std::int64_t
  typedef std::int64_t CType;
};

/////////////////////////////////////////////////////////////////////////
// No C Type is mapped to q type minute
/// Helper variable for Q Type Id for minute
constexpr const int q_minute_type_id = 17;

/// C Type for Q Type minute with KU=17
template <>
class CTypeForQTypeIdHolder<q_minute_type_id> {
 public:
  /// Map minute/17/KU to int
  typedef int CType;
};

/////////////////////////////////////////////////////////////////////////
// No C Type is mapped to q type second
/// Helper variable for Q Type Id for second
constexpr const int q_second_type_id = 18;

/// C Type for Q Type second with KV=18
template <>
class CTypeForQTypeIdHolder<q_second_type_id> {
 public:
  /// Map second/18/KV to int
  typedef int CType;
};

/////////////////////////////////////////////////////////////////////////
// No C Type is mapped to q type time
/// Helper variable for Q Type Id for time
constexpr const int q_time_type_id = 19;

/// C Type for Q Type time with KT=19
template <>
class CTypeForQTypeIdHolder<q_time_type_id> {
 public:
  /// Map time/19/KT to int
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
}  // namespace cpp2kdb
#endif  // CPP2KDB_Q_TYPES_H__
