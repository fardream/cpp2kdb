# cpp2kdb

Pure C++ Wrapper To KDB+. KDB+ is a columnar/time series database from KxSystems.

## Why a Pure C++ Wrapper to KDB+

KxSystems, the company behind KDB, actually has a library for `C/C++`. So why reinvent the wheel?

You may have heard that [`q`](https://en.wikipedia.org/w/index.php?title=Q_(programming_language_from_Kx_Systems)&oldid=967026133), the language used by KDB, is famous for its terseness and being cryptic. Now, take a look at [`k.h`](https://github.com/KxSystems/kdb/blob/39b957030bf6a4608f2508ff29894d7fac32a0c2/c/c/k.h), the interface provided by KxSystems for `C/C++`.

Now, take a look at [line 3](https://github.com/KxSystems/kdb/blob/39b957030bf6a4608f2508ff29894d7fac32a0c2/c/c/k.h#L3)

```C++
typedef char*S,C;typedef unsigned char G;typedef short H;typedef int I;typedef long long J;typedef float E;typedef double F;typedef void V;
```

First, notice that it's one line? Second, notice that it `typedef` all the common types to single character. It's not only terse and cryptic, it's almost obstructive.

The **most unfanthomable** ones are from [line 98 to line 106](https://github.com/KxSystems/kdb/blob/39b957030bf6a4608f2508ff29894d7fac32a0c2/c/c/k.h#L98-L106)

```C++
// remove more clutter
#define O printf
#define R return
#define Z static
#define P(x,y) {if(x)R(y);}
#define U(x) P(!(x),0)
#define SW switch
#define CS(n,x)	case n:x;break;
#define CD default
```

The part `// remove more clutter` is from the code itself. It's horrible, and it is **the dealbreaker**: many templates use single upper case characters as `typename`, and including `k.h` will break everything (and see this issue raised two years ago that is still unaddressed: [k.h is unusable in C++ programs](https://github.com/KxSystems/kdb/issues/12)).

## Wrapping `k.h` without exposing `k.h`

To prevent `k.h` from breaking other codes, this wrapper only includes `k.h` in one single file, [`kdb_wrapper.cc`](cpp2kdb/kdb_wrapper.cc), and expose most of kdb's functionalities through functions with common types. [`kdb_wrapper.h`](cpp2kdb/kdb_wrapper.h) and [`kdb_wrapper.cc`](cpp2kdb/kdb_wrapper.cc) don't have any other includes, and the chance that `k.h` would break other code will be minimized.

**This wrapper only focuses on getting data from KDB by a standalone `C++` application.** This is due to the limited experience that I had with KDB and the limited use cases that I encountered. If you need the support for more, just let me know.

## Wrapper

The documentation can be generated by running Doxygen in the root folder of the directory, they are also hosted [here on github pages](http://fardream.github.io/cpp2kdb).

For more details on how to use, see [wrapper_details.md](wrapper_details.md).

## Verify the code

`bazel` and `gcc>8.0.0` are necessary to build the code. KDB+ libraries are pulled in by `bazel`.

The example binary requires a running KDB+ instance on localhost at port 5000. After installing the binaries from KxSystems, run the following

```shell
q -p 5000
```

Now the example can be run by

```shell
bazel run //cpp2kdb:kdb_wrapper_test
```

## Non-bazel build

KDB libraries can be downloaded from [KxSystemx/kdb](https://github.com/kxsystems/kdb). The code requires `k.h`, which is in folder `c/c`. Put the path to `c/c` in the include path. It looks like `-pthread` is required for linker, but I am not sure if this is specific to my installation.
