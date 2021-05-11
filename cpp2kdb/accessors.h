// Copyright (C) 2021, Chao Xu
//
// Part of cpp2kdb, which is released under BSD license. See LICENSE for full
// details.
#ifndef CPP2KDB_ACCESSORS_H__
#define CPP2KDB_ACCESSORS_H__
/// \file cpp2kdb/accessors.h
/// Convenient accessor functions using kdbwrappers.h

#include <cstdint>

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

/// Q Type ID for bool, which is boolean in q, KB=1.
/// Note q uses C type char for this.
template <>
class QTypeId<bool> {
 public:
  /// Type boolean, KB=1
  constexpr static const int type_id = 1;
};

/// Q Type ID for QGuid, which is guid in q, UU=2
template <>
class QTypeId<QGuid> {
 public:
  /// Type GUID, UU=2
  constexpr static const int type_id = 2;
};

/// Q Type ID for std::uint8_t, which is byte in q, KG=4.
/// Note q uses C type char for this.
template <>
class QTypeId<std::uint8_t> {
 public:
  /// Type byte, KG=4
  constexpr static const int type_id = 4;
};

/// Q Type ID for short, which is short in q, KH=5.
template <>
class QTypeId<short> {  // NOLINT
 public:
  /// Type short, KH=5
  constexpr static const int type_id = 5;
};

/// Q Type ID for int, which is int in q, KI=6.
template <>
class QTypeId<int> {
 public:
  /// Type int, KI=6
  constexpr static const int type_id = 6;
};

/// Q Type ID for int64_t, which is long in q, KJ=7.
template <>
class QTypeId<std::int64_t> {
 public:
  /// Type long, KJ=7
  constexpr static const int type_id = 7;
};

/// Q Type ID for float, which is real in q, KE=8.
template <>
class QTypeId<float> {
 public:
  /// Type real, KE=8
  constexpr static const int type_id = 8;
};

/// Q Type ID for double, which is float  in q, KF=9.
/// Notice that in q, float is 8 byte double in C++.
template <>
class QTypeId<double> {
 public:
  /// Type float, KF=9
  constexpr static const int type_id = 9;
};

/// Q Type ID for char, which is char  in q, KC=10.
template <>
class QTypeId<char> {
 public:
  /// Type char, KC=10
  constexpr static const int type_id = 10;
};
}  // namespace cpp2kdb
#endif  // CPP2KDB_ACCESSORS_H__
