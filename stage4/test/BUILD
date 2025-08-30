cc_test(
    name = "hello-test",
    srcs = ["hello-test.cc"],
    copts = [
        "-Iexternal/gtest/googletest/include",
        "-Iexternal/gtest/googletest",
    ],
    deps = [
        "@googletest//:gtest_main",   # External dependency
        "//main:hello-greet",         # Local code under test
    ],
)
