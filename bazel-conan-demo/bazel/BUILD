load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "demoapp_with_conan",
    srcs = ["//apps/demoapp/src:main.cpp"],
    deps = ["//bazel/third_party:loggerlib_bazel"],
)

