// Copyright (C) 2021, Chao Xu
//
// Part of cpp2kdb, which is released under BSD license. See LICENSE for full
// details.
#ifndef CPP2KDB_KDB_WRAPPER_H__
#define CPP2KDB_KDB_WRAPPER_H__
/// \file cpp2kdb/kdb_wrapper.h
/// Wrapping functions for k.h
///
/// Header file has no includes, and source file has no includes besides k.h.
/// Wrapping functions defined in k.h

/// Wrappers for kx/kdb+'s k.h
namespace cpp2kdb::kdb_wrapper {
/// Convert a const char* to char*
///
/// char strings look to be char* in KDB. Use this function to convert const
/// char* to a char*. Note, this is through const_cast<char*>.
char* ConvertToNonConst(const char* c);

/// Open a connection to KDB
///
/// Open a connection to KDB by calling khpunc (connect with capacity)
///
/// \returns Return the value returned by khpunc.
///          0: Authentication Error.
///          -1: Connection error.
///          -2: Timeout Error.
///          -3: OpenSSL initialization failed.
int OpenConnection(
    /// host name to connect to.
    const char* host,
    /// port number to connect to.
    int port,
    /// username and password in the format of "username:password"
    const char* username_password = "",
    /// Timeout in milliseconds. Default = 5000
    int time_out = 0,
    /// Capacity. Per kdb doc, this is bit, 1 = 1 TB limit, 2 = use TLS. 3 is
    /// using both.
    int capacity = 0);

/// Close the connection to KDB
///
void CloseConnection(
    /// Handle to close.
    int connection);
/// Run query on a connection.
void* RunQueryOnConnection(
    /// Handle
    int connection,
    /// Query
    const char* query);

/// Run query on a connection.
void* RunQueryOnConnection(
    /// Handle
    int connection,
    /// Query
    const char* query,
    /// Argument
    void* arg);

/// Run query on a connection.
void* RunQueryOnConnection(
    /// Handle
    int connection,
    /// Query
    const char* query,
    /// Argument 1
    void* arg1,
    /// Argument 2
    void* arg2);

/// Run query on a connection.
void* RunQueryOnConnection(
    /// Handle
    int connection,
    /// Query
    const char* query,
    /// Argument 1
    void* arg1,
    /// Argument 2
    void* arg2,
    /// Argument 3
    void* arg3);

/// Run query on a connection.
void* RunQueryOnConnection(
    /// Handle
    int connection,
    /// Query
    const char* query,
    /// Argument 1
    void* arg1,
    /// Argument 2
    void* arg2,
    /// Argument 3
    void* arg3,
    /// Argument 4
    void* arg4);

/// Run query on a connection.
void* RunQueryOnConnection(
    /// Handle
    int connection,
    /// Query
    const char* query,
    /// Argument 1
    void* arg1,
    /// Argument 2
    void* arg2,
    /// Argument 3
    void* arg3,
    /// Argument 4
    void* arg4,
    /// Argument 5
    void* arg5);

/// Obtain type id of this K pointer.
///
/// Get the type id of the data contained in this k.
/// It returns the ->t of the struct.
int GetQTypeId(void* x);

/// Get Pointer to the atom value
///
/// Per KDB documentation, the value stored in a k0 is a union.
/// Therefore, the pointer to the value can be obtained by taking the address of
/// any of member. In this case, we are going to use 's', which is char.
void* GetValue(void* x);

// Check if size of long long is 8 or 64bit. Make sure this is compatible with
// int64_t. In kx's documentation, it actually claims the type is int64_t,
// however, in k.h, it actually typedef long long to J and doens't include the
// proper headers for int64_t
static_assert(sizeof(long long) == 8, "type long must be 8 bytes");  // NOLINT

/// Get number of elements in vector
///
/// If this is a vector, get the number of elements in the vector.
/// Per documentation, this should be int64_t, which is not in the type system.
/// Therefore, long is used here. Notice the static_assert before.
long long GetNumberOfVectorElements(void* x);  // NOLINT

/// Get the pointer to the first elemnt of vector.
///
/// If this is a vector, get the pointer to the first element of vector.
void* GetVector(void* x);

/// Decrease reference count by calling `r0` function.
///
/// There are many restrictions on how memory (or threading) is used in kdb.
/// Check with their documentation for more details.
void DecreaseReferenceCount(void* x);

/// Increase reference count by calling `r1` function.
///
/// There are many restrictions on how memory (or threading) is used in kdb.
/// Check with their documentation for more details.
void* IncreaseReferenceCount(void* x);

/// Call ktd (Convert keyed table to a simple table) on x.
/// \returns the result from ktd function. nullptr if the operation failed.
void* DekeyKeyedTable(void* x);

/// Call r0 when this class is destructed.
///
/// After getting a pointer to a K object, just construct
/// DecreaseReferenceCountGuard with that void*.
class DecreaseReferenceCountGuard {
 public:
  /// Create DecreaseReferenceCountGuard to guard pointer_to_guard.
  /// Will call r0 when destructor is called.
  explicit DecreaseReferenceCountGuard(void* pointer_to_guard);
  /// Default constructor is deleted.
  DecreaseReferenceCountGuard() = delete;
  /// Copy constructor is deleted.
  DecreaseReferenceCountGuard(const DecreaseReferenceCountGuard&) = delete;
  /// Move constructor is deleted.
  DecreaseReferenceCountGuard(DecreaseReferenceCountGuard&&) = delete;
  /// assignment operator is deleted.
  DecreaseReferenceCountGuard& operator=(const DecreaseReferenceCountGuard&) =
      delete;
  /// Unguard this pointer by setting the pointer_to_guard to nullptr.
  void Unguard();

  /// Destructor, calling r0 on pointer_to_guard if it's not nullptr
  ~DecreaseReferenceCountGuard();

 private:
  void* pointer_to_guard;
};
}  // namespace cpp2kdb::kdb_wrapper
#endif  // CPP2KDB_KDB_WRAPPER_H__
