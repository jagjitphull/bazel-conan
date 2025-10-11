"""Custom text processing rules for Bazel."""

def _text_processor_impl(ctx):
    """Implementation function for the text_processor rule.
    
    This function is called by Bazel when the rule is executed.
    It defines what actions to perform to generate outputs.
    
    Args:
        ctx: The rule context object containing all rule information
        
    Returns:
        DefaultInfo provider with the output file
    """
    # Step 1: Get the input file from the rule's 'src' attribute
    input_file = ctx.file.src
    
    # Step 2: Declare the output file that will be generated
    # The output file name is based on the target name
    output_file = ctx.actions.declare_file(
        ctx.label.name + "_processed.txt"
    )
    
    # Step 3: Get configuration values from rule attributes
    header = ctx.attr.header
    uppercase = ctx.attr.uppercase
    add_timestamp = ctx.attr.add_timestamp
    
    # Step 4: Build the shell command to process the file
    cmd = ""
    
    # Add header if specified
    if header:
        cmd += 'echo "%s" > %s\n' % (header, output_file.path)
        cmd += 'echo "---" >> %s\n' % output_file.path
    
    # Add timestamp if requested
    if add_timestamp:
        cmd += 'echo "Processed: $(date)" >> %s\n' % output_file.path
        cmd += 'echo "" >> %s\n' % output_file.path
    
    # Process the actual content
    if uppercase:
        cmd += 'cat %s | tr "[:lower:]" "[:upper:]" >> %s' % (
            input_file.path,
            output_file.path
        )
    else:
        if header or add_timestamp:
            cmd += 'cat %s >> %s' % (input_file.path, output_file.path)
        else:
            cmd += 'cat %s > %s' % (input_file.path, output_file.path)
    
    # Step 5: Execute the shell command as a Bazel action
    ctx.actions.run_shell(
        inputs = [input_file],
        outputs = [output_file],
        command = cmd,
        mnemonic = "TextProcessing",
        progress_message = "Processing %s" % input_file.short_path,
    )
    
    # Step 6: Return the output file(s) to Bazel
    return [DefaultInfo(files = depset([output_file]))]


# Define the rule with its attributes
text_processor = rule(
    implementation = _text_processor_impl,
    attrs = {
        "src": attr.label(
            mandatory = True,
            allow_single_file = [".txt"],
            doc = "The input text file to process",
        ),
        "header": attr.string(
            default = "",
            doc = "Optional header text to add at the beginning",
        ),
        "uppercase": attr.bool(
            default = False,
            doc = "Convert text to uppercase",
        ),
        "add_timestamp": attr.bool(
            default = False,
            doc = "Add processing timestamp",
        ),
    },
    doc = """Processes a text file with various transformations.
    
    Example:
        text_processor(
            name = "processed_data",
            src = "input.txt",
            header = "Processed Data File",
            uppercase = True,
            add_timestamp = True,
        )
    """,
)

def _text_merger_impl(ctx):
    """Merges multiple text files into one.
    
    Args:
        ctx: The rule context object
        
    Returns:
        DefaultInfo with merged output file
    """
    # Get all input files (can be multiple)
    input_files = ctx.files.srcs
    
    # Declare the output file
    output_file = ctx.actions.declare_file(ctx.attr.output_name)
    
    # Build the merge command
    separator = ctx.attr.separator
    cmd = ""
    
    for i, f in enumerate(input_files):
        # Add separator between files (except before first)
        if i > 0 and separator:
            cmd += 'echo "%s" >> %s\n' % (separator, output_file.path)
        # Append file content
        cmd += 'cat %s >> %s\n' % (f.path, output_file.path)
    
    # Execute the merge action
    ctx.actions.run_shell(
        inputs = input_files,
        outputs = [output_file],
        command = cmd,
        mnemonic = "TextMerging",
        progress_message = "Merging %d files" % len(input_files),
    )
    
    return [DefaultInfo(files = depset([output_file]))]


# Define the text_merger rule
text_merger = rule(
    implementation = _text_merger_impl,
    attrs = {
        "srcs": attr.label_list(
            mandatory = True,
            allow_files = [".txt"],
            doc = "List of text files to merge",
        ),
        "output_name": attr.string(
            mandatory = True,
            doc = "Name of the output file",
        ),
        "separator": attr.string(
            default = "---",
            doc = "Separator between merged files",
        ),
    },
    doc = "Merges multiple text files into a single file.",
)
