# examples/BUILD
package(default_visibility = ["//visibility:public"])
load("//tools:my_rules.bzl", "hello_file", "concat_files", "py_transform")

# 1) Simple "hello_file" usage
hello_file(
    name = "hello_out",
    greeting = "Namaste, Bazel!",
    template = ":template.txt",  # optional; demonstrates a label attr
)

# 2) Concatenate two small text files
concat_files(
    name = "joined",
    srcs = [
        "data/a.txt",
        "data/b.txt",
    ],
)

# 3) Transform a file using a Python tool
py_transform(
    name = "upper_a",
    src = "data/a.txt",
    tool = "scripts/upper.py",
)

# Optional: a tiny sh_test that validates 'joined' content contains 'alpha'
#    It runs //examples/app:grep_alpha, which uses main.sh logic
sh_test(
    name = "joined_has_alpha_test",
    srcs = ["grep_alpha_test.sh"],
    data = [":joined"],                             # make the output available at runtime
    args = ["--joined=$(location :joined)"],        # pass the exact path to the test

)

