// Copyright (C) 2021, Chao Xu
//
// Part of cpp2kdb, which is released under BSD license. See LICENSE for full
// details.
#include "cpp2kdb/accessors.h"

namespace cpp2kdb {
bool IsError(void* x) { return GetTypeId(x) == q_error_type_id; }

bool IsAtomic(void* x) {
  int type_id = GetTypeId(x);
  return type_id < 0 && type_id != q_error_type_id;
}

bool IsVector(void* x) {
  int type_id = GetTypeId(x);
  return type_id >= 0 && type_id != q_dict_type_id &&
         type_id != q_table_type_id;
}

bool IsMixedVector(void* x) { return GetTypeId(x) == q_mixed_type_id; }

bool IsDict(void* x) { return GetTypeId(x) == q_dict_type_id; }

bool IsTable(void* x) { return GetTypeId(x) == q_table_type_id; }
}  // namespace cpp2kdb
