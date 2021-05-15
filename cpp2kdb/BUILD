# Copyright (C) 2021, Chao Xu
#
# Part of cpp2kdb, which is released under BSD license. See LICENSE for full
# details.
cc_binary(
    name = "kdb_wrapper_test",
    srcs = ["kdb_wrapper_test.cc"],
    deps = [":kdb_wrapper"],
)

cc_library(
    name = "kdb_wrapper",
    srcs = ["kdb_wrapper.cc"],
    hdrs = ["kdb_wrapper.h"],
    deps = ["@kdb"],
)

cc_library(
    name = "accessors",
    srcs = ["accessors.cc"],
    hdrs = [
        "accessors.h",
        "q_types.h",
    ],
    deps = [":kdb_wrapper"],
)

cc_binary(
    name = "accessors_test",
    srcs = ["accessors_test.cc"],
    deps = [":accessors"],
)
