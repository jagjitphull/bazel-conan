load("@bazel_skylib//rules:common_settings.bzl", "bool_flag")
# --- Feature flag: choose dataset (small vs full) ---
bool_flag(
    name = "full_data",
    build_setting_default = False,
)
config_setting(
    name = "with_full",
    flag_values = {":full_data": "true"},
)

py_library(
    name = "dq_lib",
    srcs = [
        "dq/stats.py",
        "dq/__init__.py",
    ],
)
py_binary(
    name = "dq_app",
    srcs = ["dq/main.py"],
    main = "dq/main.py",
    deps = [":dq_lib"],
    # Include both CSVs in runfiles; we pass the right one via args
    data = [
        "data/sales_small.csv",
        "data/sales_full.csv",
    ],
    args = select({
        ":with_full": ["--csv=$(rootpath data/sales_full.csv)"],
        "//conditions:default": ["--csv=$(rootpath data/sales_small.csv)"],
    }),
)

py_test(
    name = "dq_test",
    srcs = ["tests/dq_test.py"],
    main = "tests/dq_test.py",
    deps = [":dq_lib"],
    data = ["data/sales_small.csv"],
    args = ["--csv=$(rootpath data/sales_small.csv)"],
)
