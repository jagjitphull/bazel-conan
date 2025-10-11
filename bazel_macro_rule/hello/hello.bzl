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
