// Copyright (C) 2021, Chao Xu
//
// Part of cpp2kdb, which is released under BSD license. See LICENSE for full
// details.
#include "cpp2kdb/accessors.h"

namespace cpp2kdb {
bool IsError(void* x) { return GetQTypeId(x) == q_error_type_id; }

bool IsAtomic(void* x) { return IsQTypeIdError(GetQTypeId(x)); }

bool IsVector(void* x) { return IsQTypeIdVector(GetQTypeId(x)); }

bool IsMixedVector(void* x) { return IsQTypeIdMixedVector(GetQTypeId(x)); }

bool IsDict(void* x) { return IsQTypeIdDict(GetQTypeId(x)); }

bool IsTable(void* x) { return IsQTypeIdTable(GetQTypeId(x)); }

DataRetrievalResult CheckVectorForVectorDataRetrieval(void* input_vector) {
  // First, check if this pointer is null
  if (input_vector == nullptr) {
    return DataRetrievalResult::NullInput;
  }
  // Check if this is an error
  if (IsError(input_vector)) {
    return DataRetrievalResult::ValueError;
  }
  // Check if this is a vector
  if (!IsVector(input_vector)) {
    return DataRetrievalResult::NotVector;
  }

  // Return OK.
  return DataRetrievalResult::Ok;
}

DataRetrievalResult RetrieveVectorData(void* input_vector,
                                       std::string* output_vector) {
  return DataRetrievalResult::Ok;
}

DataRetrievalResult RetrieveVectorData(void* input_vector,
                                       void** output_vector) {
  return DataRetrievalResult::Ok;
}
}  // namespace cpp2kdb
