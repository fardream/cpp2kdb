cc_binary(
    name = "kdbwrapper_test",
    srcs = ["kdbwrapper_test.cc"],
    deps = [":kdbwrapper"],
)

cc_library(
    name = "kdbwrapper",
    srcs = ["kdbwrapper.cc"],
    hdrs = ["kdbwrapper.h"],
    deps = ["@kdb//:kdb_l64_unsecured"],
)
