# examples/app/BUILD
sh_binary(
    name = "main",
    srcs = ["main.sh"],
    data = [
        "//examples:joined",
        "//examples:hello_out",
        "//examples:upper_a",
    ],
)

# A helper script for the test in ../BUILD
sh_binary(
    name = "grep_alpha.sh",
    srcs = ["main.sh"],  # reuse same interpreter shebang; we’ll branch by argv
)

