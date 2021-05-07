#include "cpp2kdb/kdbwrapper.h"

// Include k.h...
#include <k.h>

namespace cpp2kdb {
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

void* RunQueryOnConnection(int connection, const char* query) {
  // Call k.
  return k(connection, ConvertToNonConst(query), 0);
}

int GetTypeId(void* x) {
  // Get K.
  return GetK(x)->t;
}

void* GetValue(void* x) {
  // A simple static cast of the address.
  return static_cast<void*>(&(GetK(x)->s));
}

long GetNumberOfVectorElements(void* x) {
  // n
  return GetK(x)->n;
}

void* GetVector(void* x) {
  // Vector is stored in the construct
  return static_cast<void*>(GetK(x)->G0);
}
}  // namespace cpp2kdb
