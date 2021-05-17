# Wrapper details

## How to use `k.h` to get data from KDB

Before diving into the wrapper code, let's start to looking at how to use `k.h` to connect to KDB and get some data back. KxSystems' documentation is [here](https://code.kx.com/q/interfaces/c-client-for-q/).

1. Connect to kdb by calling one of the functions `khp`, `khpu`, `khpun`, `khpunc`.

    It looks like the ones with longer names are just setting default parameters for the shorter ones, and we are going to use the one with the longest name

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

    This will get run the query on remote KDB server and return the result as a `K`.

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

1. It can be seen that `K` is a pointer to `struct k0` (why not `k`? remember it's already used by the function `k`).

1. The only data we need is in the union part of the object.
    - The pointer type `k0* k` indicates this `K` is containing another `K`, which only happens for table type.
    - The struct with `n` and `G0` in it indicating a vector of sorts, which `n` indicates the number of elements contained by `G0`, and `G0` is a pointer pointing to that memory location. The type `G` is actually `unsigned char`, which is not directly convertible to other types (unlike `char*`);

1. The type of the `K` object is indicated by `t`. The exact type is listed in [KxSystems' documentation](https://code.kx.com/q/interfaces/c-client-for-q/#overview). The only special case that needs to mention is `symbol`, which is actually a `char*` pointing to a null-terminated `char` array (like a proper `C` string), where as string in KDB sense is an array of `char` that is **NOT null-terminated**.

## How to actually access the data from `K`?

- To get an **atomic** value from `K`, just use the corresponding accessor.
- To get the content of the vector, get the pointer `G0` and cast it to the type indicated by `t`.
