# setup.sh — creates the suggested structure + files
# Run from an empty repo root:  bash setup.sh

set -euo pipefail

# Directories
mkdir -p hello

# hello/BUILD.bazel (empty) — why: makes //hello a real Bazel package
: > hello/BUILD.bazel

# .bazelversion — pin to Bazel 8.4
cat > .bazelversion <<'EOF'
8.4.0
EOF

# MODULE.bazel — minimal Bzlmod module
cat > MODULE.bazel <<'EOF'
module(
    name = "hello_demo",
    version = "0.1.0",
)
EOF

# hello/hello.bzl — rule + macro
cat > hello/hello.bzl <<'EOF'
def _hello_impl(ctx):
    out = ctx.outputs.executable
    msg = ctx.attr.message
    script = """#!/usr/bin/env bash
set -euo pipefail
cat <<'__MSG__'
{msg}
__MSG__
""".format(msg = msg)
    ctx.actions.write(output = out, content = script, is_executable = True)
    return DefaultInfo(executable = out, files = depset([out]))

hello_binary = rule(
    implementation = _hello_impl,
    attrs = {"message": attr.string(mandatory = True)},
    executable = True,
    doc = "Writes an executable that prints the given message.",
)

def hello_world(name, visibility = None):
    hello_binary(
        name = name,
        message = "Hello, World!",
        visibility = visibility,
    )
EOF

# BUILD.bazel — use the macro + rule
cat > BUILD.bazel <<'EOF'
load("//hello:hello.bzl", "hello_binary", "hello_world")

hello_world(
    name = "hello",
    visibility = ["//visibility:public"],
)

hello_binary(
    name = "greet_bob",
    message = "Hello, Bob from a custom rule!",
)
EOF

printf "\nRepo ready.\nTry:\n  bazel run //:hello\n  bazel run //:greet_bob\n"
