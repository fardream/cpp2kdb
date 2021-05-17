# Wrapper details

## How to use `k.h` to get data from KDB

Before diving into the wrapper code, let's start by looking at how to use `k.h` to connect to KDB and get some data back. KxSystems' documentation is [here](https://code.kx.com/q/interfaces/c-client-for-q/).

1. Connect to kdb by calling one of the functions `khp`, `khpu`, `khpun`, `khpunc`.

    It looks like the ones with shorter names are just the ones with longer names with some default parameters set, and we are going to use the one with the longest name

    ```C++
    char* hostname = "hostname";
    char* username_password = "username:password";
    int time_out = 0;
    int capacity = 0;
    int connection = khpunc(hostname, port_number, username_password, time_out, capacity);
    ```

1. Run the query by calling function `k` on the connection.

    ```C++
    char* query = "1";
    K result = k(connection, query, (K)0);
    ```

    This will run the query on remote KDB server and return the result as a `K`.

1. Do something with the result.

1. Normally, the reference count on the `K` object needs to be **manually** reduced by calling a function `r0`.

1. Close the connection.

    ```C++
    kclose(connection);
    ```

One note here: `char*` is used as string in many places because `k.h` expects `S`, which is just `char*`. However, any recent compiler will give out a warning or error.

```sh
warning: ISO C++11 does not allow conversion from string literal to 'char *' [-Wwritable-strings]
```

And `const char*` (the proper type for string literals) should not be passed as type `char*` for obvious reasons.

## Result from KDB, `K` object

How to get data from this `K` object? `K` is actually not a class, it's a pointer type defined on [line 11](https://github.com/KxSystems/kdb/blob/39b957030bf6a4608f2508ff29894d7fac32a0c2/c/c/k.h#L11) for `KXVER>=3`.

```C++
typedef struct k0{signed char m,a,t;C u;I r;union{G g;H h;I i;J j;E e;F f;S s;struct k0*k;struct{J n;G G0[1];};};}*K;
```

Or in proper `C++` style

```C++
typedef struct k0 {
  signed char m, a, t;
  C u;
  I r;
  union {
    G g;
    H h;
    I i;
    J j;
    E e;
    F f;
    S s;
    struct k0* k;
    struct {
      J n;
      G G0[1];
    };
  };
} * K;
```

- `K` is a pointer to `struct k0` (why not `k`? remember it's already used by the function `k`).

- The only data we need is in the `union` part of the object.

  - The struct with `n` and `G0` is a vector of sorts, where `n` is the number of elements contained by `G0`, and `G0` is a pointer pointing to that memory location. The type `G` is actually `unsigned char`, which is not directly convertible to other types (unlike `char*`).

  - Notice that `G G0[1]` is an array of size 1 - which may seem absurd. However, also notice that `G0` is the last member of `struct k0` and the enclosed union, and the memory locations right after `G0` is not part of `k0`, and can be used to store more data (on most of compilers with most common settings anyway...).

- The type of the `K` object is indicated by `t`. The exact type is listed in [KxSystems' documentation](https://code.kx.com/q/interfaces/c-client-for-q/#overview). The only special case that needs mentioning is `symbol`, which is actually a `char*` pointing to a null-terminated `char` array (like a proper `C` string), where as string in KDB sense is an array of `char` that is **NOT null-terminated**.

## How to actually access the data from `K`?

- To get an **atomic** value from `K`, just use the corresponding accessor.

- To get the content of a vector, get the pointer `G0` and cast it to the type indicated by `t`.

- To get the content of a table (type id is 98), get the atomic `K` pointer (member data `k`) which is a dict type 99.

- The content of a dict (type 99) is a `K` vector of size 2 (so pointed by `G0`). The first `K` contains the keys, and the second `K` contains the values. Key `K` and value `K` contain the same number of elements.

- The content of a keyed table is a dict (therefore type 99), but the key and value `K`s are both tables (type 98). Keyed tables can also be converted to a simple table (type 98) by calling `knd`.

## How Wrappers Work

### Use `void*` as `K`

`void*` will be used in place of `K` in the wrapper.

The wrapper contains 3 header files, and two of them with associated source files.

- [**`kdb_wrapper.h`**](cpp2kdb/kdb_wrapper.h) and [**`kdb_wrapper.cc`**](cpp2kdb/kdb_wrapper.cc):ß The only include is in the source, and it includes `k.h`. It provides the functions to many of the same functions defined `k.h`.

- [**`q_types.h`**](cpp2kdb/q_types.h): This single header handles the mapping between a q type indicated by an integer and a c type. This generated from [q_types.h.yml](cpp2kdb/q_types.h.yml) and [q_types.h.template](cpp2kdb/q_types.h.template) with [mustache templating](http://mustache.github.io). It does **NOT** include `kdb_wrapper.h`.

- [**`accessors.h`**](cpp2kdb/accessors.h) and [**`accessors.cc`**](cpp2kdb/accessors.cc): This set of files include both `kdb_wrapper.h` and `q_types.h`, and provides convenient functions to getting data as different types.

## `kdb_wrapper` Wrapping `k.h`

### Getting data

***In all the wrappers, `K` is replaced with `void*`**, so the access to type information `t`, the atomic values, and the vector data is lost. `kdb_wrapper` provides the following functions to get access to those fields:

- `int GetQTypeId(void*)` gets the q type id for `void*`.

- `void* GetValue(void*)` gets the pointer to atomic value from this `void*`. This relies on the fact that all members of a `C/C++` union have the same address in memory, so the atomic value's pointer can be obtained by getting address of any of the union members.

- `long long GetNumberOfVectorElements(void*)` gets the number of elements in the vector (`n`).

- `void* GetVector(void*)` gets the pointer to the first element of the vector (`G0`).

Note all the `Get` functions have **NO** type checks.

### Call `r0` when going out of scope

It is required to call `r0` after consuming the data in `K`. One way to guarantee that is to create a `class`, and call `r0` on the associated pointer in its destructor. `DecreaseReferenceCountGuard` is provided for this purpose.

```C++
{
  void* result = RunQueryOnConnection(connection, ....);
  DecreaseReferenceCountGuard guard(result);
  // do things.
  if(...) {
    // should call r0 here
    return;
  }
  // something else.
  // when the program exit this scope, the destructor will be called and r0 be called.
  // should call r0 here
  return;
}
```

`DecreaseReferenceCountGuard` has only one constructor, accepting `void*` as its input. It has only one member function that is `void Unguard()`, which tells `DecreaseReferenceCountGuard` to forget about the `void*` and not do anything in destructor.

## `q_types` and mapping between `C` type and q type id

`q_types.h` provides the necessary mappings between `C` types and q type ids.

- From q type id `q_type_id`, the corresponding `C` type can be obtained by `cpp2kdb::q_types::CTypeForQTypeId<q_type_id>`.

- For subset of `C` types `T`, q type id can be obtained by `cpp2kdb;:q_types::q_type_id<T>`

There is **NOT** a type defined in `q_types` that can represent a `K` object.

## Use `accessors` to get data out from `K`

`accessors` provides the necessary functions to get data from `K` without worrying too much about the type stored.

- For atomic data, use `cpp2kdb::accessors::GetValue<T>(void*)` to get the data. For arithmetic types, the result is `static_cast` from the type indicated from `K` to the `T` requested. `std::string` and `void*` are also supported.

- For vector data, use `cpp2kdb::accessors::RetrieveVectorData(void* input, T* output)`. Arithmetic types, `std::string` and `void*` are the only supported types. `input` must be a vector (so cannot be dictionary, atomic or table). `output` is required to hold the number of elements in `K`.

- For dictionary, use `cpp2kdb::accessors::GetVector<void*>(void* input)` to get the key list and value list, then use `RetrieveVectorData` to get the data.
- For simple table, use `cpp2kdb::accessors::GetSimpleTable(void* input, void** column_heading, void*** values, std::size_t *number_of_columns, std::size_t *number_of_rows)`. Note that `column_heading` and `values` will be set to propere `K`s in the `input`, so preallocating memory is not necessary.

## Unobstructive Wrapper

The goal of this wrapper is **unobstructive**, or any part of the library can be used indepedently of each other, and can mix with other tools or codes that target `kdb`. For example, a `K` can be obtained from another code base and it will work with any of functions defined in `accessors`.
