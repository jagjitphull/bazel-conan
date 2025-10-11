This `tools/my_rules.bzl` file defines **three custom Bazel rules** using **Starlark**, Bazel’s extension language. These rules demonstrate how to:

* write a file (`hello_file`)
* concatenate multiple files (`concat_files`)
* run a transformation with a Python tool (`py_transform`)

---

I'll break it down **line by line** (and block by block) for full clarity.

---

### File Header (Lines 1–6)

```python
# tools/my_rules.bzl
# Write your first custom rules (Starlark)

# Create tools/my_rules.bzl and add three rules to learn the basics:
#     hello_file: writes a new file from a string attribute (+ a template).
#     concat_files: concatenates multiple input files using actions.run_shell.
#     py_transform: runs a Python tool to transform an input file to an output (shows runfiles + tools).
```

These are **comments** describing the purpose of the file:

* It's a Bazel `.bzl` file written in **Starlark**.
* It defines **three beginner-friendly custom rules**:

  1. `hello_file`
  2. `concat_files`
  3. `py_transform`

---

## 1. `hello_file`: Create a Text File with a Greeting

### Rule Implementation Function

```python
def _hello_file_impl(ctx):
```

Defines a **private rule implementation** (by convention, underscore prefix).

---

```python
    greeting = ctx.attr.greeting
    template = ctx.file.template  # may be None if not provided
```

* `ctx.attr.greeting`: Gets the value of the `greeting` string attribute.
* `ctx.file.template`: Gets the file passed to the `template` attribute (or `None`).

---

```python
    out = ctx.actions.declare_file(ctx.label.name + ".txt")
```

* Declares the **output file** to be named `<target name>.txt`.

---

```python
    content_lines = []
    content_lines.append("Hello from Bazel rule!")
    content_lines.append("Greeting: " + greeting)
```

* Creates the content line-by-line.
* Adds a greeting message and the user-provided greeting string.

---

```python
    if template:
        content_lines.append("Template file provided: " + template.path)
    else:
        content_lines.append("No template provided.")
```

* Conditionally adds a line indicating whether a template file was provided.
* `template.path` is a string path to the file (for logging/reference only, not read).

---

```python
    content = "\n".join(content_lines) + "\n"
```

* Joins the lines into one string with newline characters.

---

```python
    ctx.actions.write(output = out, content = content)
```

* Writes the content to the output file.

---

```python
    return DefaultInfo(files = depset([out]))
```

* Returns the output file to Bazel via `DefaultInfo`.

---

### Rule Declaration

```python
hello_file = rule(
    implementation = _hello_file_impl,
    attrs = {
        "greeting": attr.string(mandatory = True),
        "template": attr.label(allow_single_file = True),
    },
    doc = "Writes a text file containing a greeting and (optionally) the path of a template file.",
)
```

* Declares the `hello_file` rule.
* Requires:

  * `greeting`: a mandatory string
  * `template`: an optional file label
* The rule uses `_hello_file_impl` as its logic.

---

## 2. `concat_files`: Concatenate Multiple Files into One

### Rule Implementation Function

```python
def _concat_files_impl(ctx):
```

* Begins implementation of a rule that combines input files.

---

```python
    out = ctx.actions.declare_file(ctx.label.name + ".txt")
```

* Declares the output file.

---

```python
    inputs = [f for f in ctx.files.srcs]
    input_paths = " ".join([f.path for f in inputs]) if inputs else ""
```

* Gets the list of files passed to the `srcs` attribute.
* Joins their paths into a single space-separated string.

---

```python
    cmd = """
set -eu
: > "{out}"
for f in {inputs}; do
  cat "$f" >> "{out}"
  echo "" >> "{out}"
done
""".format(out = out.path, inputs = input_paths)
```

* Defines a shell command:

  * `set -eu`: strict error handling
  * `: > "{out}"`: clears the output file
  * Loops over input files:

    * Appends each to the output
    * Adds a newline after each

---

```python
    ctx.actions.run_shell(
        inputs = inputs,
        outputs = [out],
        command = cmd,
        progress_message = "Concatenating files for {}".format(ctx.label),
    )
```

* Executes the shell command in a Bazel action.

---

```python
    return DefaultInfo(files = depset([out]))
```

* Returns the output file as the result.

---

### Rule Declaration

```python
concat_files = rule(
    implementation = _concat_files_impl,
    attrs = {
        "srcs": attr.label_list(allow_files = True),
    },
    doc = "Concatenates text files in order into a single output.",
)
```

* Declares the `concat_files` rule.
* Takes:

  * `srcs`: a list of input files to concatenate

---

## 3. `py_transform`: Use a Python Tool to Transform a File

### Rule Implementation Function

```python
def _py_transform_impl(ctx):
```

* Begins implementation for a Python-based transformation.

---

```python
    tool = ctx.executable.tool
    src = ctx.file.src
    out = ctx.actions.declare_file(ctx.label.name + ".txt")
```

* Gets:

  * `tool`: the Python executable (declared in attrs)
  * `src`: the input file
  * Declares an output file

---

```python
    ctx.actions.run(
        inputs = [src],
        tools = [tool],
        outputs = [out],
        executable = tool,
        arguments = [src.path, out.path],
        progress_message = "Transforming {} -> {}".format(src.path, out.path),
    )
```

* Runs the Python tool with:

  * Input: `src`
  * Output: `out`
  * `arguments`: passed on command line

---

```python
    return DefaultInfo(files = depset([out]))
```

* Returns the output file.

---

### Rule Declaration

```python
py_transform = rule(
    implementation = _py_transform_impl,
    attrs = {
        "src": attr.label(allow_single_file = True, mandatory = True),
        "tool": attr.label(executable = True, cfg = "exec", mandatory = True, allow_single_file = True),
    },
    doc = "Runs a Python 'tool' on input 'src' to produce an output text file.",
)
```

* Declares the `py_transform` rule.
* Requires:

  * `src`: a single input file
  * `tool`: a Python executable (must be marked as executable)

---

### Final Comments

```python
# Concepts we just used:
#     rule() with attrs → define inputs (strings, labels, lists)
#     ctx.actions.write() → create a file from literal content
#     ctx.actions.run_shell() → invoke inline shell to process files
#     ctx.actions.run() with executable/tools → run a tool (Python script here)
#     Return DefaultInfo with produced outputs so Bazel knows what to build
```

A recap of Bazel/Skylark concepts demonstrated in the examples.

---

### Summary

You now have:

* `hello_file`: writes a file from a greeting + optional template
* `concat_files`: merges input files into one
* `py_transform`: runs a Python script to transform an input

These examples cover most **core Starlark rule patterns**:

* Writing files
* Running shell commands
* Using external tools
* Handling inputs/outputs
* Declaring rule attributes


