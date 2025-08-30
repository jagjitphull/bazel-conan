cc_library(
    name = "hello_lib",
    hdrs = ["hello.h"],
    srcs = ["hello.cc"],
)

cc_binary(
    name = "hello",
    srcs = ["main.cc"],
    deps = [":hello_lib"],
)

sh_test(
    name = "hello_sh_test",
    srcs = ["hello_test.sh"],
    data = [":hello"],
    # rootpath gives the runfiles-relative path; we will prepend $TEST_SRCDIR in the script
    args = ["$(location :hello)"],
)

