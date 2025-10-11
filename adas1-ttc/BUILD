package(default_visibility = ["//visibility:public"])

cc_library(
    name = "eigen",
    hdrs = glob(["third_party/eigen_vendor/include/**"]),
    includes = ["third_party/eigen_vendor/include"],
)

cc_library(
    name = "adas",
    srcs = ["src/adas/acc.cpp"],
    hdrs = ["src/adas/acc.h"],
    deps = [":eigen"],
    copts = ["-std=c++17"],
)

cc_binary(
    name = "adas_demo",
    srcs = ["src/main.cpp"],
    deps = [":adas", ":eigen"],
    copts = ["-std=c++17"],
)
