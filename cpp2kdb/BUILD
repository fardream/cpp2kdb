# Copyright (C) 2021, Chao Xu
#
# Part of cpp2kdb, which is released under BSD license. See LICENSE for full
# details.
cc_binary(
    name = "kdbwrapper_test",
    srcs = ["kdbwrapper_test.cc"],
    deps = [":kdbwrapper"],
)

cc_library(
    name = "kdbwrapper",
    srcs = ["kdbwrapper.cc"],
    hdrs = ["kdbwrapper.h"],
    deps = ["@kdb"],
)

cc_library(
    name = "accessors",
    srcs = ["accessors.cc"],
    hdrs = [
        "accessors.h",
        "q_types.h",
    ],
    deps = [":kdbwrapper"],
)

cc_binary(
    name = "accessors_test",
    srcs = ["accessors_test.cc"],
    deps = [":accessors"],
)
