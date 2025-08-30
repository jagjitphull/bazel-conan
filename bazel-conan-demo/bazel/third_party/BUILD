load("@rules_cc//cc:defs.bzl", "cc_import", "cc_library")

# Keep this as-is (points to the .a or .so you imported)
cc_import(
    name = "loggerlib_import",
    # no hdrs here; just the binary
    static_library = "loggerlib/lib/libloggerlib.a",   # OR shared_library = ".../libloggerlib.so"
    visibility = ["//visibility:public"],
)

# Expose headers + include path to dependents
cc_library(
    name = "loggerlib_bazel",
    hdrs = glob(["loggerlib/include/**/*.hpp"]),
    strip_include_prefix = "loggerlib/include",   # makes includes look like "logger.hpp"
    include_prefix = "",                          # (no extra prefix)
    deps = [":loggerlib_import"],
    visibility = ["//visibility:public"],
)

