"""A Starlark rule to configure the C++ toolchain."""

load(
    "@bazel_tools//tools/cpp:cc_toolchain_config_lib.bzl",
    "feature",
    "flag_group",
    "flag_set",
    "tool_path",
)

def _impl(ctx):
    # Get tool paths from the local environment.
    # Replace the values with the actual paths to your compiler and linker.
    cc_tool_paths = [
        tool_path(name = "gcc", path = "/usr/bin/gcc"),
        tool_path(name = "g++", path = "/usr/bin/g++"),
        tool_path(name = "ld", path = "/usr/bin/ld"),
        tool_path(name = "ar", path = "/usr/bin/ar"),
        tool_path(name = "cpp", path = "/usr/bin/cpp"),
        tool_path(name = "gcov", path = "/usr/bin/gcov"),
        tool_path(name = "nm", path = "/usr/bin/nm"),
        tool_path(name = "objdump", path = "/usr/bin/objdump"),
        tool_path(name = "strip", path = "/usr/bin/strip"),
    ]

    # Define features for the toolchain.
    # This example adds a feature to always use C++17.
    features = [
        feature(
            name = "default_compiler_flags",
            enabled = True,
            flags = [
                flag_set(
                    actions = ["c++-compile"],
                    flag_groups = [
                        flag_group(
                            flags = ["-std=c++17"],
                        ),
                    ],
                ),
            ],
        ),
    ]

    return cc_common.create_cc_toolchain_config_info(
        ctx = ctx,
        toolchain_identifier = "my-custom-toolchain",
        host_system_name = "local",
        target_system_name = "local",
        target_cpu = "k8", # This is the generic x86_64 identifier
        target_libc = "unknown",
        compiler = "gcc",
        abi_version = "unknown",
        abi_libc_version = "unknown",
        tool_paths = cc_tool_paths,
        features = features,
    )

my_cc_toolchain_config = rule(
    implementation = _impl,
    attrs = {},
    provides = [CcToolchainConfigInfo],
)