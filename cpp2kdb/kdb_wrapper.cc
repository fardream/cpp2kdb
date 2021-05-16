// Copyright (C) 2021, Chao Xu
//
// Part of cpp2kdb, which is released under BSD license. See LICENSE for full
// details.
#include "cpp2kdb/kdb_wrapper.h"

// Include k.h...
#include <k.h>

namespace cpp2kdb::kdb_wrapper {
namespace {
/// reinterpret_cast a pointer into K
///
k0* GetK(void* input) {
  //
  return reinterpret_cast<k0*>(input);
}
}  // namespace

char* ConvertToNonConst(const char* c) {
  // const_cast
  return const_cast<char*>(c);
}

int OpenConnection(const char* host, int port, const char* username_password,
                   int time_out, int capacity) {
  int result = khpunc(ConvertToNonConst(host), port,
                      ConvertToNonConst(username_password), time_out, capacity);
  return result;
}

void CloseConnection(int connection) {
  // call kclose
  return kclose(connection);
}

void* RunQueryOnConnection(int connection, const char* query) {
  // Call k.
  return k(connection, ConvertToNonConst(query), 0);
}

int GetQTypeId(void* x) {
  // Get K.
  return GetK(x)->t;
}

void* GetValue(void* x) {
  // A simple static cast of the address.
  return static_cast<void*>(&(GetK(x)->s));
}

long long GetNumberOfVectorElements(void* x) {  // NOLINT
  // n
  return GetK(x)->n;
}

void* GetVector(void* x) {
  // Vector is stored in the construct
  return static_cast<void*>(GetK(x)->G0);
}

void DecreaseReferenceCount(void* x) {
  // Call r0
  r0(GetK(x));
}

void* IncreaseReferenceCount(void* x) {
  // Call r1.
  return r1(GetK(x));
}

void* DekeyKeyedTable(void* x) {
  // call ktd
  return static_cast<void*>(ktd(GetK(x)));
}

DecreaseReferenceCountGuard::DecreaseReferenceCountGuard(void* pointer_to_guard)
    : pointer_to_guard(pointer_to_guard) {
  // do nothing here
}
void DecreaseReferenceCountGuard::Unguard() {
  this->pointer_to_guard = nullptr;
}
DecreaseReferenceCountGuard::~DecreaseReferenceCountGuard() {
  // Only call when pointer_to_guard is not nullptr.
  if (this->pointer_to_guard != nullptr) {
    kdb_wrapper::DecreaseReferenceCount(this->pointer_to_guard);
  }
}
}  // namespace cpp2kdb::kdb_wrapper
