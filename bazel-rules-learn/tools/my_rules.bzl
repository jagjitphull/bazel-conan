# tools/my_rules.bzl

#Write your first custom rules (Starlark)

#Create tools/my_rules.bzl and add three rules to learn the basics:
#    hello_file: writes a new file from a string attribute (+ a template).
#    concat_files: concatenates multiple input files using actions.run_shell.
#    py_transform: runs a Python tool to transform an input file to an output (shows runfiles + tools).


def _hello_file_impl(ctx):
    # Rule inputs
    greeting = ctx.attr.greeting
    template = ctx.file.template  # may be None if not provided

    # Declare an output file named "<target>.txt"
    out = ctx.actions.declare_file(ctx.label.name + ".txt")

    # Compose the content
    content_lines = []
    content_lines.append("Hello from Bazel rule!")
    content_lines.append("Greeting: " + greeting)
    if template:
        # Read template at analysis time? -> Not allowed.
        # Instead, we pass file path to a shell action that copies/echoes.
        # For a pure 'write', we can just embed a placeholder line indicating the template path.
        content_lines.append("Template file provided: " + template.path)
    else:
        content_lines.append("No template provided.")
    content = "\n".join(content_lines) + "\n"

    # Write the content
    ctx.actions.write(output = out, content = content)

    return DefaultInfo(files = depset([out]))

hello_file = rule(
    implementation = _hello_file_impl,
    attrs = {
        "greeting": attr.string(mandatory = True),
        "template": attr.label(allow_single_file = True),
    },
    doc = "Writes a text file containing a greeting and (optionally) the path of a template file.",
)

def _concat_files_impl(ctx):
    # Output file
    out = ctx.actions.declare_file(ctx.label.name + ".txt")

    # Build the shell command that concatenates srcs in order
    inputs = [f for f in ctx.files.srcs]
    input_paths = " ".join([f.path for f in inputs]) if inputs else ""
    cmd = """
set -eu
: > "{out}"
for f in {inputs}; do
  cat "$f" >> "{out}"
  echo "" >> "{out}"
done
""".format(out = out.path, inputs = input_paths)

    ctx.actions.run_shell(
        inputs = inputs,
        outputs = [out],
        command = cmd,
        progress_message = "Concatenating files for {}".format(ctx.label),
    )

    return DefaultInfo(files = depset([out]))

concat_files = rule(
    implementation = _concat_files_impl,
    attrs = {
        "srcs": attr.label_list(allow_files = True),
    },
    doc = "Concatenates text files in order into a single output.",
)

def _py_transform_impl(ctx):
    # We will run a Python script (declared in 'tool') on 'src'
    tool = ctx.executable.tool
    src = ctx.file.src
    out = ctx.actions.declare_file(ctx.label.name + ".txt")

    # The tool will read src and write out upper-cased content.
    # We pass paths via arguments.
    ctx.actions.run(
        inputs = [src],
        tools = [tool],
        outputs = [out],
        executable = tool,
        arguments = [src.path, out.path],
        progress_message = "Transforming {} -> {}".format(src.path, out.path),
    )

    return DefaultInfo(files = depset([out]))

py_transform = rule(
    implementation = _py_transform_impl,
    attrs = {
        "src": attr.label(allow_single_file = True, mandatory = True),
        "tool": attr.label(executable = True, cfg = "exec", mandatory = True, allow_single_file = True),
    },
    doc = "Runs a Python 'tool' on input 'src' to produce an output text file.",
)

###############################################################

#Concepts we just used:
#    rule() with attrs → define inputs (strings, labels, lists)
#    ctx.actions.write() → create a file from literal content
#    ctx.actions.run_shell() → invoke inline shell to process files
#    ctx.actions.run() with executable/tools → run a tool (Python script here)
#    Return DefaultInfo with produced outputs so Bazel knows what to build

