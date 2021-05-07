# cpp2kdb

Pure C++ Wrapper To KDB+ by KxSystems

## Why a Pure C++ Wrapper to KDB+ by KxSystems

Take a look at KxSystems' library for kdb ([here](https://github.com/KxSystems/kdb/blob/39b957030bf6a4608f2508ff29894d7fac32a0c2/c/c/k.h) is the `C/C++` header) - this is taking terseness and being cryptic to an extreme. For example [line 3](https://github.com/KxSystems/kdb/blob/39b957030bf6a4608f2508ff29894d7fac32a0c2/c/c/k.h#L3)

```C++
typedef char*S,C;typedef unsigned char G;typedef short H;typedef int I;typedef long long J;typedef float E;typedef double F;typedef void V;
```

Why on one line? Why single upper case character defines? On smaller screens this needs scrolling and cannot be shown at the same time.

The **most unfanthomable** ones are from [line 99 to line 106](https://github.com/KxSystems/kdb/blob/39b957030bf6a4608f2508ff29894d7fac32a0c2/c/c/k.h#L99-L106)

```C++
#define O printf
#define R return
#define Z static
#define P(x,y) {if(x)R(y);}
#define U(x) P(!(x),0)
#define SW switch
#define CS(n,x)	case n:x;break;
#define CD default
```

**The dealbreaker**: many templates use single upper case characters as `typename`, and including `k.h` will break everything - and see this issue [k.h is unusable in C++ programs](https://github.com/KxSystems/kdb/issues/12).

## Wrapping `k.h` without exposing `k.h`

To prevent `k.h` from breaking other codes, this wrapper only includes `k.h` in one single file, [`kdbwrapper.cc`](cpp2kdb/kdbwrapper.cc), and expose most of kdb's functionalities through functions with common types. [`kdbwrapper.h`](cpp2kdb/kdbwrapper.h) and [`kdbwrapper.cc`](cpp2kdb/kdbwrapper.cc) don't have any other includes, and the chance that `k.h` would break other code will be minimized.

Most importantly, the `K` type, which is a pointer to `k0` struct, will be `void*`.

[`accessors.h`](cpp2kdb/accessor.h) provides convenient accessor functions by calling functions in [`kdbwrapper.h`](cpp2kdb/kdbwrapper.h).
