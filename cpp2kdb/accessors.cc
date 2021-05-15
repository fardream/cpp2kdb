// Copyright (C) 2021, Chao Xu
//
// Part of cpp2kdb, which is released under BSD license. See LICENSE for full
// details.
#include "cpp2kdb/accessors.h"

namespace cpp2kdb::accessors {
bool IsError(void* x) {
  return kdb_wrapper::GetQTypeId(x) == q_types::q_error_type_id;
}

bool IsAtomic(void* x) {
  return q_types::IsQTypeIdError(kdb_wrapper::GetQTypeId(x));
}

bool IsVector(void* x) {
  return q_types::IsQTypeIdVector(kdb_wrapper::GetQTypeId(x));
}

bool IsMixedVector(void* x) {
  return q_types::IsQTypeIdMixedVector(kdb_wrapper::GetQTypeId(x));
}

bool IsDict(void* x) {
  return q_types::IsQTypeIdDict(kdb_wrapper::GetQTypeId(x));
}

bool IsTable(void* x) {
  return q_types::IsQTypeIdTable(kdb_wrapper::GetQTypeId(x));
}

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
}  // namespace cpp2kdb::accessors
