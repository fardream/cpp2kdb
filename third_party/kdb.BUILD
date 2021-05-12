# KBD library
package(default_visibility = ["//visibility:public"])

# Support for macOS on x86_64.
config_setting(
    name = "darwin",
    values = {"cpu": "darwin"},
)

# Default without support for SSL.
cc_library(
    name = "kdb",
    srcs = select({
        "darwin": ["m64/c.o"],
        "//conditions:default": ["l64/c.o"],
    }),
    hdrs = ["c/c/k.h"],
    defines = ["KXVER=3"],
    linkopts = ["-pthread"],
    strip_include_prefix = "c/c",
)

cc_library(
    name = "kdb_secured",
    srcs = select({
        "darwin": ["m64/e.o"],
        "//conditions:default": ["l64/e.o"],
    }),
    hdrs = ["c/c/k.h"],
    defines = ["KXVER=3"],
    linkopts = ["-pthread"],
    strip_include_prefix = "c/c",
)
