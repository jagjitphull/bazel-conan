load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "loggerlib",
    hdrs = ["include/logger.hpp"],
    srcs = ["src/logger.cpp"],
    includes = ["include"],           # adds -I<repo>/libs/loggerlib/include
    visibility = ["//visibility:public"],
)

