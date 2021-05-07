package(default_visibility = ["//visibility:public"])

cc_library(
    name = "kdb_l64_unsecured",
    srcs = ["l64/c.o"],
    hdrs = ["c/c/k.h"],
    defines = ["KXVER=3"],
    linkopts = ["-pthread"],
    strip_include_prefix = "c/c",
)

cc_library(
    name = "kdb_l32_unsecured",
    srcs = ["l32/c.o"],
    hdrs = ["c/c/k.h"],
    defines = ["KXVER=3"],
    linkopts = ["-pthread"],
    strip_include_prefix = "c/c",
)

cc_library(
    name = "kdb_l64_secured",
    srcs = ["l64/e.o"],
    hdrs = ["c/c/k.h"],
    defines = ["KXVER=3"],
    linkopts = ["-pthread"],
    strip_include_prefix = "c/c",
)

cc_library(
    name = "kdb_l32_secured",
    srcs = ["l32/e.o"],
    hdrs = ["c/c/k.h"],
    defines = ["KXVER=3"],
    linkopts = ["-pthread"],
    strip_include_prefix = "c/c",
)
