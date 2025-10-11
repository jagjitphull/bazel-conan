# --- Level-1 Headway + FCW/ABW ---

cc_library(
    name = "adas_headway",
    srcs = ["src/adas/headway_fcw.cpp"],
    hdrs = ["src/adas/headway_fcw.h"],
    # This makes "adas/headway_fcw.h" the public include path (no "../src")
    strip_include_prefix = "src",
    include_prefix = "",
    copts = ["-std=c++17"],
)

cc_binary(
    name = "demo_headway",
    srcs = ["src/apps/demo_headway.cpp"],
    deps = [":adas_headway"],
    copts = ["-std=c++17"],
)
