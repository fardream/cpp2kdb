#ifndef CPP2KDB_KDBWRAPPER_H__
#define CPP2KDB_KDBWRAPPER_H__

/// Wrappers for kx/kdb+'s k.h
namespace cpp2kdb {
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
    const char* username_password,
    /// Timeout in milliseconds. Default = 5000
    int time_out = 5000,
    /// Capacity. Per kdb doc, this is bit, 1 = 1 TB limit, 2 = use TLS. 3 is
    /// using both.
    int capacity = 0);

/// Run query on a connection.
void* RunQueryOnConnection(int connection, const char* query);

int GetTypeId(void* x);

/// Get Pointer to the atom value
///
/// Per KDB documentation, the value stored in a k0 is a union.
/// Therefore, the pointer to the value can be obtained by taking the address of
/// any of member. In this case, we are going to use 's', which is char.
void* GetValue(void* x);

/// Check if size of long is 8 or 64bit. Make sure this is compatible with
/// int64_t.
static_assert(sizeof(long) == 8, "type long must be 8 bytes");

/// Get number of elements in vector
///
/// If this is a vector, get the number of elements in the vector.
/// Per documentation, this should be int64_t, which is not in the type system.
/// Therefore, long is used here. Notice the static_assert before.
long GetNumberOfVectorElements(void* x);

/// Get the pointer to the first elemnt of vector.
///
/// If this is a vector, get the pointer to the first element of vector.
void* GetVector(void* x);
}  // namespace cpp2kdb
#endif  // CPP2KDB_KDBWRAPPER_H__
