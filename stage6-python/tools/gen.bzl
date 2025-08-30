# //tools:gen.bzl

def _py_template_impl(ctx):
    """Implementation of the py_template rule."""
    output = ctx.actions.declare_file(ctx.attr.output)
    
    # Define a shell command action to run
    ctx.actions.expand_template(
        output = output,
        template = ctx.file.template,
        substitutions = ctx.attr.substitutions,
    )
    
    return [DefaultInfo(files = depset([output]))]

py_template = rule(
    implementation = _py_template_impl,
    attrs = {
        "template": attr.label(
            mandatory = True,
            allow_single_file = [".template"],
            doc = "The template file to expand."
        ),
        "output": attr.string(
            mandatory = True,
            doc = "The name of the generated Python file."
        ),
        "substitutions": attr.string_dict(
            mandatory = False,
            doc = "A dictionary of key-value pairs for substitution."
        ),
    },
)