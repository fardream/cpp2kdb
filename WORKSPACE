workspace(name = "cpp2kdb")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "new_git_repository")

new_git_repository(
    name = "kdb",
    build_file = "//third_party:kdb.BUILD",
    commit = "39b957030bf6a4608f2508ff29894d7fac32a0c2",
    remote = "https://github.com/KxSystems/kdb.git",
    shallow_since = "1619547440 +0100",
)
