# Bazel Query Training Project

A comprehensive training project for learning `bazel query`, `bazel cquery`, and `bazel aquery` commands using Bazel 8.4.

## Project Structure

```
bazel-query-demo/
├── MODULE.bazel
├── .bazelversion
├── lib/
│   ├── BUILD.bazel
│   ├── math.cc
│   ├── math.h
│   ├── string_utils.cc
│   └── string_utils.h
├── app/
│   ├── BUILD.bazel
│   └── main.cc
├── utils/
│   ├── BUILD.bazel
│   ├── logger.cc
│   └── logger.h
└── tools/
    ├── BUILD.bazel
    └── helper.cc
```

## Setup Files

### `.bazelversion`
```
8.4.0
```

### `MODULE.bazel`
```python
module(
    name = "query_training",
    version = "1.0",
)
```

## Package: lib/

### `lib/BUILD.bazel`
```python
cc_library(
    name = "math",
    srcs = ["math.cc"],
    hdrs = ["math.h"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "string_utils",
    srcs = ["string_utils.cc"],
    hdrs = ["string_utils.h"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "all_libs",
    deps = [
        ":math",
        ":string_utils",
    ],
    visibility = ["//visibility:public"],
)
```

### `lib/math.h`
```cpp
#ifndef LIB_MATH_H_
#define LIB_MATH_H_

int add(int a, int b);
int multiply(int a, int b);

#endif  // LIB_MATH_H_
```

### `lib/math.cc`
```cpp
#include "lib/math.h"

int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}
```

### `lib/string_utils.h`
```cpp
#ifndef LIB_STRING_UTILS_H_
#define LIB_STRING_UTILS_H_

#include <string>

std::string uppercase(const std::string& str);
std::string lowercase(const std::string& str);

#endif  // LIB_STRING_UTILS_H_
```

### `lib/string_utils.cc`
```cpp
#include "lib/string_utils.h"
#include <algorithm>

std::string uppercase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

std::string lowercase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}
```

## Package: utils/

### `utils/BUILD.bazel`
```python
cc_library(
    name = "logger",
    srcs = ["logger.cc"],
    hdrs = ["logger.h"],
    visibility = ["//visibility:public"],
)
```

### `utils/logger.h`
```cpp
#ifndef UTILS_LOGGER_H_
#define UTILS_LOGGER_H_

#include <string>

void log(const std::string& message);

#endif  // UTILS_LOGGER_H_
```

### `utils/logger.cc`
```cpp
#include "utils/logger.h"
#include <iostream>

void log(const std::string& message) {
    std::cout << "[LOG] " << message << std::endl;
}
```

## Package: app/

### `app/BUILD.bazel`
```python
cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = [
        "//lib:math",
        "//lib:string_utils",
        "//utils:logger",
    ],
)

cc_test(
    name = "main_test",
    srcs = ["main.cc"],
    deps = [
        "//lib:math",
         "//lib:string_utils",
        "//utils:logger",
    ],
)
```

### `app/main.cc`
```cpp
#include <iostream>
#include "lib/math.h"
#include "lib/string_utils.h"
#include "utils/logger.h"

int main() {
    log("Application started");
    
    int result = add(5, 3);
    std::cout << "5 + 3 = " << result << std::endl;
    
    std::string text = "Hello Bazel";
    std::cout << "Uppercase: " << uppercase(text) << std::endl;
    
    log("Application finished");
    return 0;
}
```

## Package: tools/

### `tools/BUILD.bazel`
```python
cc_binary(
    name = "helper",
    srcs = ["helper.cc"],
    deps = ["//lib:all_libs"],
)
```

### `tools/helper.cc`
```cpp
#include <iostream>
#include "lib/math.h"
#include "lib/string_utils.h"

int main() {
    std::cout << "Helper tool: 10 * 20 = " << multiply(10, 20) << std::endl;
    return 0;
}
```

---

## Query Commands Reference

### 1. `bazel query` - Build Graph Queries

Query the build graph without configuration. Useful for understanding target relationships and dependencies.

#### Basic Queries

**List all targets in a package:**
```bash
bazel query //lib/...
```

**Find all targets in the workspace:**
```bash
bazel query //...
```

**Show dependencies of a target:**
```bash
bazel query "deps(//app:main)"
```

**Show reverse dependencies (who depends on this target):**
```bash
bazel query "rdeps(//..., //lib:math)"
```

#### Advanced Queries

**Find all cc_library targets:**
```bash
bazel query "kind(cc_library, //...)"
```

**Find all cc_binary targets:**
```bash
bazel query "kind(cc_binary, //...)"
```

**Show direct dependencies only:**
```bash
bazel query "deps(//app:main, 1)"
```

**Find all dependencies except a specific one:**
```bash
bazel query "deps(//app:main) except //lib:math"
```

**Find targets that depend on both libraries:**
```bash
bazel query "rdeps(//..., //lib:math) intersect rdeps(//..., //lib:string_utils)"
```

**Show build file path for a target:**
```bash
bazel query "buildfiles(//app:main)"
```

**List all tests:**
```bash
bazel query "kind(test, //...)"
```

**Show dependency graph in different formats:**
```bash
# Graph output
bazel query "deps(//app:main)" --output=graph

# Labeled output with rule classes
bazel query "deps(//app:main)" --output=label_kind

# Build proto output
bazel query "deps(//app:main)" --output=proto
```

**Find common dependencies:**
```bash
bazel query "somepath(//app:main, //lib:math)"
```

**Find targets visible to a specific package:**
```bash
bazel query "visible(//app:main, //...)"
```

### 2. `bazel cquery` - Configured Target Graph Queries

Query the configured target graph with build configuration information. Shows how targets are actually built.

#### Basic Cqueries

**Show configured dependencies:**
```bash
bazel cquery "deps(//app:main)"
```

**Show configuration information:**
```bash
bazel cquery "//app:main" --output=transitions
```

**Find dependencies with specific configuration:**
```bash
bazel cquery "deps(//app:main)" --output=label_kind
```

#### Advanced Cqueries

**Show build configuration details:**
```bash
bazel cquery "//app:main" --output=build
```

**Query with specific platform:**
```bash
bazel cquery "//app:main" --platforms=//path/to:platform
```

**Show file paths that will be built:**
```bash
bazel cquery "deps(//app:main)" --output=files
```

**Find targets built in a specific configuration:**
```bash
bazel cquery "kind(cc_library, deps(//app:main))"
```

**Show Starlark representation:**
```bash
bazel cquery "//app:main" --output=starlark --starlark:expr="target.label"
```

**Compare configurations:**
```bash
# Show compilation mode effect
bazel cquery "//app:main" -c opt
bazel cquery "//app:main" -c dbg
```

**Find all transitive dependencies:**
```bash
bazel cquery "kind(cc_.*, deps(//app:main))"
```

### 3. `bazel aquery` - Action Graph Queries

Query the action graph to see actual build actions (compilation, linking, etc.).

#### Basic Aqueries

**Show all actions for a target:**
```bash
bazel aquery "//app:main"
```

**Show actions with specific mnemonic:**
```bash
bazel aquery "mnemonic('CppCompile', //app:main)"
```

**Show link actions:**
```bash
bazel aquery "mnemonic('CppLink', //app:main)"
```

#### Advanced Aqueries

**Show inputs to actions:**
```bash
bazel aquery "//app:main" --output=text --include_artifacts
```

**Show command lines:**
```bash
bazel aquery "//app:main" --output=textproto
```

**Filter by input file:**
```bash
bazel aquery "inputs('.*main\\.cc', //app:main)"
```

**Filter by output file:**
```bash
bazel aquery "outputs('.*\\.o$', //app:main)"
```

**Show actions in JSON format:**
```bash
bazel aquery "//app:main" --output=jsonproto
```

**Find all compile actions:**
```bash
bazel aquery "mnemonic('CppCompile', //...)"
```

**Show detailed action information:**
```bash
bazel aquery "//app:main" --output=text --include_commandline --include_artifacts
```

**Find actions that produce specific outputs:**
```bash
bazel aquery "outputs('.*main$', //app:main)"
```

---

## Practical Training Exercises

### Exercise 1: Dependency Analysis
1. Find all targets that depend on `//lib:math`
2. Determine if `//app:main` directly or transitively depends on `//utils:logger`
3. List all libraries used by `//tools:helper`

### Exercise 2: Build Investigation
1. Use `cquery` to see the actual configuration used for `//app:main`
2. Compare the outputs when building with `-c opt` vs `-c dbg`
3. Find all files that will be created when building `//app:main`

### Exercise 3: Action Analysis
1. Count how many C++ compilation actions occur for `//app:main`
2. Find the link command used to create the `main` binary
3. Identify which source files trigger compilation actions

### Exercise 4: Complex Queries
1. Find all targets that depend on both `//lib:math` AND `//lib:string_utils`
2. List all binary targets in the workspace
3. Find the shortest path from `//app:main` to `//lib:math`

---

## Tips for Using Queries

1. **Performance**: Use `query` for fast structural queries, `cquery` when you need configuration info, and `aquery` when analyzing actual build actions.

2. **Debugging**: Start with simple queries and gradually add filters to narrow results.

3. **Output formats**: Try different `--output` flags (text, proto, graph, json) for different use cases.

4. **Combining queries**: Use operators like `union`, `intersect`, `except` to combine query results.

5. **Regular expressions**: Many query functions support regex patterns (e.g., `attr("name", ".*test.*", //...)`)

---

## Building the Project

```bash
# Build everything
bazel build //...

# Build specific target
bazel build //app:main

# Run the application
bazel run //app:main

# Clean
bazel clean
```

---

## Additional Resources

- [Bazel Query Documentation](https://bazel.build/query/language)
- [Bazel Cquery Documentation](https://bazel.build/query/cquery)
- [Bazel Aquery Documentation](https://bazel.build/query/aquery)

## Query Cheat Sheet

| Command | Purpose | When to Use |
|---------|---------|-------------|
| `bazel query` | Build graph structure | Understanding dependencies, target relationships |
| `bazel cquery` | Configured build graph | Understanding how targets are actually built with config |
| `bazel aquery` | Action graph | Understanding build actions, compiler commands, file I/O |

#############################################################

# Install graphviz and xdot
sudo apt-get install graphviz xdot  # Ubuntu/Debian
# or
sudo yum install graphviz python3-xdot  # CentOS/RHEL
# or
brew install graphviz  # macOS
pip install xdot



Example 1: Visualize Dependencies of //app:main

# Generate the graph and view with xdot (interactive)
bazel query "deps(//app:main)" --output=graph | xdot -

# Or save to a file first, then view
bazel query "deps(//app:main)" --output=graph > app_deps.dot
xdot app_deps.dot

# Generate PNG image
bazel query "deps(//app:main)" --output=graph | dot -Tpng -o app_deps.png
xdg-open app_deps.png  # Linux
# open app_deps.png    # macOS

# Generate SVG (scalable)
bazel query "deps(//app:main)" --output=graph | dot -Tsvg -o app_deps.svg


Example 2: Reverse Dependencies - Who Depends on //lib:math?

# Show all targets that depend on //lib:math
bazel query "rdeps(//..., //lib:math)" --output=graph | xdot -

# Save as PDF
bazel query "rdeps(//..., //lib:math)" --output=graph | dot -Tpdf -o math_rdeps.pdf

# With better layout for complex graphs
bazel query "rdeps(//..., //lib:math)" --output=graph | dot -Tpng -Grankdir=LR -o math_rdeps.png


Example 3: Complete Workspace Overview

# Visualize all targets in the workspace
bazel query "//..." --output=graph | xdot -

# Filter to show only rules (no source files)
bazel query "kind(rule, //...)" --output=graph | xdot -

# Show only libraries and binaries (cleaner view)
bazel query "kind('cc_(library|binary|test)', //...)" --output=graph | xdot -

# Save with horizontal layout (left-to-right)
bazel query "kind('cc_(library|binary)', //...)" --output=graph | \
  dot -Tpng -Grankdir=LR -Nshape=box -Nfontsize=10 -o workspace.png


  Advanced Graph Customization
Example 4: Custom Styled Graph

# Create a custom dot file with better styling
cat > generate_graph.sh << 'EOF'
#!/bin/bash

bazel query "deps(//app:main)" --output=graph | \
  dot -Tpng \
    -Grankdir=TB \
    -Nshape=box \
    -Nstyle=filled \
    -Nfillcolor=lightblue \
    -Nfontsize=12 \
    -Efontsize=10 \
    -Gsplines=ortho \
    -o custom_graph.png

echo "Graph saved to custom_graph.png"
EOF

chmod +x generate_graph.sh
./generate_graph.sh

Example 5: Compare Two Targets

# Show paths between two targets
bazel query "somepath(//app:main, //lib:math)" --output=graph | xdot -

# Show all paths
bazel query "allpaths(//app:main, //lib:math)" --output=graph | xdot -