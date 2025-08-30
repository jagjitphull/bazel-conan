load(":cc_toolchain_config.bzl", "my_cc_toolchain_config")

package(default_visibility = ["//visibility:public"])

# This rule creates the CcToolchainConfigInfo provider.
my_cc_toolchain_config(name = "my_toolchain_config")

# This rule represents the C++ toolchain.
cc_toolchain(
    name = "my_cc_toolchain",
    toolchain_identifier = "my-custom-toolchain",
    toolchain_config = ":my_toolchain_config",
    all_files = "@bazel_tools//tools/cpp:all_files",
    compiler_files = "@bazel_tools//tools/cpp:compiler_files",
    dwp_files = "@bazel_tools//tools/cpp:dwp_files",
    linker_files = "@bazel_tools//tools/cpp:linker_files",
    objcopy_files = "@bazel_tools//tools/cpp:objcopy_files",
    strip_files = "@bazel_tools//tools/cpp:strip_files",
    supports_param_files = 1,
)

# This rule registers the toolchain with Bazel.
toolchain(
    name = "my_toolchain_for_linux_x86_64",
    toolchain = ":my_cc_toolchain",
    toolchain_type = "@bazel_tools//tools/cpp:toolchain_type",
    target_compatible_with = [
        "@platforms//cpu:x86_64",
        "@platforms//os:linux",
    ],
)