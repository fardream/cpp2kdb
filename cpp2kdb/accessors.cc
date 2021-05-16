// Copyright (C) 2021, Chao Xu
//
// Part of cpp2kdb, which is released under BSD license. See LICENSE for full
// details.
#include "cpp2kdb/accessors.h"

#include <algorithm>

namespace cpp2kdb::accessors {
namespace {
/// Copy Symbol List to std::string.
///
DataRetrievalResult CopySymbolListToString(void* input_vector,
                                           std::string* output_vector) {
  // Data is just an array of pointers to \0 terminated strings.
  char** symbols = GetVector<char*>(input_vector);

  std::size_t number_of_elements =
      kdb_wrapper::GetNumberOfVectorElements(input_vector);
  // Copy them over.
  for (std::size_t i = 0; i < number_of_elements; i++) {
    *(output_vector + i) = std::string(symbols[i]);
  }

  return DataRetrievalResult::Ok;
}

DataRetrievalResult CopyMixedVectorAsString(void* input_vector,
                                            std::string* output_vector) {
  // Get thet list of Ks
  void** charlistlist = GetVector<void*>(input_vector);

  std::size_t number_of_elements =
      kdb_wrapper::GetNumberOfVectorElements(input_vector);

  DataRetrievalResult result = DataRetrievalResult::Ok;
  // Copy them over.
  for (std::size_t i = 0; i < number_of_elements; i++) {
    // Take this k.
    void* this_k = *(charlistlist + i);
    // Make sure this is a char vector.
    int this_q_type_id = kdb_wrapper::GetQTypeId(this_k);
    if (this_q_type_id != q_types::q_char_type_id) {
      result = DataRetrievalResult::NotCharVectorInMixedVector;
      break;
    }
    // Construct the string. Note the string is **NOT** \0 terminated.
    *(output_vector + i) = GetStringFromCharVector(this_k);
  }
  return result;
}
}  // namespace
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

const char* GetDataRetrievalResultName(DataRetrievalResult result) {
  // Cast result to an int.
  int temp_result = static_cast<int>(result);
  // Make sure it's valid.
  if (temp_result >= 0 && temp_result < number_of_data_retrieval_result_names) {
    return DataRetrievalResultNames[temp_result];
  } else {
    return "Invalid";
  }
}

std::ostream& operator<<(std::ostream& output, DataRetrievalResult result) {
  return output << GetDataRetrievalResultName(result);
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

std::string GetStringFromCharVector(void* input_char_vector) {
  return std::string(GetVector<char>(input_char_vector),
                     kdb_wrapper::GetNumberOfVectorElements(input_char_vector));
}

DataRetrievalResult RetrieveVectorData(void* input_vector,
                                       std::string* output_vector) {
  DataRetrievalResult check_result =
      CheckVectorForVectorDataRetrieval(input_vector);
  if (check_result != DataRetrievalResult::Ok) {
    return check_result;
  }
  // Get the q type id
  int q_type_id = kdb_wrapper::GetQTypeId(input_vector);
  // If this is a mixed vector, go through each element to make sure it's char
  // vector.
  if (q_types::IsQTypeIdMixedVector(q_type_id)) {
    return CopyMixedVectorAsString(input_vector, output_vector);
  } else if (q_type_id == q_types::q_symbol_type_id) {
    // Symbol vector.
    return CopySymbolListToString(input_vector, output_vector);
  } else {
    // Not a string...
    return DataRetrievalResult::NotStringVector;
  }
}

DataRetrievalResult RetrieveVectorData(void* input_vector,
                                       void** output_vector) {
  DataRetrievalResult check_result =
      CheckVectorForVectorDataRetrieval(input_vector);
  if (check_result != DataRetrievalResult::Ok) {
    return check_result;
  }
  // If this is not mixed vector, return right way.
  if (!IsMixedVector(input_vector)) {
    return DataRetrievalResult::NotMixedVector;
  }

  // Copy is simple, since the data should be void** anyway
  std::copy_n(GetVector<void*>(input_vector),
              kdb_wrapper::GetNumberOfVectorElements(input_vector),
              output_vector);

  return DataRetrievalResult::Ok;
}

DataRetrievalResult GetSimpleTable(void* simple_table, void** column_heading,
                                   void*** values,
                                   std::size_t* number_of_columns,
                                   std::size_t* number_of_rows) {
  // Underlying dictionary.
  void* underlying_dict = GetValue<void*>(simple_table);

  // Key list and value list of the dictionary.
  void** key_value_list = GetVector<void*>(underlying_dict);

  // Separate keys and values
  *column_heading = key_value_list[0];
  void* value_list = key_value_list[1];

  // Get number of columns first
  *number_of_columns = kdb_wrapper::GetNumberOfVectorElements(*column_heading);
  // If the number of columns is zero, return rightway.
  if (number_of_columns == 0) {
    return DataRetrievalResult::Ok;
  }

  // get values.
  *values = GetVector<void*>(value_list);
  *number_of_rows = kdb_wrapper::GetNumberOfVectorElements(**values);

  return DataRetrievalResult::Ok;
}
}  // namespace cpp2kdb::accessors
