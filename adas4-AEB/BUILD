# --- AEB (Automatic Emergency Braking) ---

cc_library(
    name = "adas_aeb",
    srcs = ["src/adas/aeb.cpp"],
    hdrs = ["src/adas/aeb.h"],
    # Expose headers as "adas/aeb.h" instead of "src/adas/aeb.h"
    strip_include_prefix = "src",
    include_prefix = "",
    copts = ["-std=c++17"],
)

cc_binary(
    name = "demo_aeb",
    srcs = ["apps/demo_aeb.cpp"],
    deps = [":adas_aeb"],
    copts = ["-std=c++17"],
)
