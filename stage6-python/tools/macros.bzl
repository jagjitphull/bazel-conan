# //tools:macros.bzl

load(":gen.bzl", "py_template")
load("@rules_python//python:py_binary.bzl", "py_binary")

def create_python_service(name, template, srcs = [], substitutions = {}, **kwargs):
    """Macro to create a Python service from a template.

    Args:
        name: The name of the service.
        template: The template file.
        srcs: A list of source files to include in the service.
        substitutions: A dictionary of substitutions for the template.
        **kwargs: Passed to the underlying py_binary rule.
    """
    
    # 1. Instantiate our custom rule to generate the Python file.
    generated_file_name = name + "_generated_file"
    py_template(
        name = generated_file_name,
        template = template,
        output = name + "_service.py",
        substitutions = substitutions,
    )
    
    # 2. Combine the user's srcs with our generated file.
    all_srcs = srcs + [":" + generated_file_name]
    
    # 3. Instantiate a native py_binary rule to create the executable.
    py_binary(
        name = name,
        srcs = all_srcs,
        main = name + "_service.py",
        **kwargs
    )