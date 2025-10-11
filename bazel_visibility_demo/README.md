A **shell script** to generate the entire Bazel project with all 4 visibility cases in a single command.
It will create the project structure, all required files, and valid content for **Bazel 8.4**.

---

## ✅ Shell Script: `generate_bazel_visibility_demo.sh`

#!/bin/bash

set -e

PROJECT_ROOT="bazel_visibility_demo"

echo "Creating Bazel visibility demo project in: $PROJECT_ROOT"

mkdir -p $PROJECT_ROOT/{lib,tools,app,internal}

# 1. WORKSPACE
touch $PROJECT_ROOT/WORKSPACE

# 2. lib
cat > $PROJECT_ROOT/lib/math_lib.py <<EOF
def add(a, b):
    return a + b
EOF

cat > $PROJECT_ROOT/lib/BUILD.bazel <<EOF
py_library(
    name = "math_lib",
    srcs = ["math_lib.py"],
    visibility = ["//app:__pkg__"],  # Only //app can use this library
)
EOF

# 3. tools
cat > $PROJECT_ROOT/tools/helper.py <<EOF
def print_message(msg):
    print(f"[Helper] {msg}")
EOF

cat > $PROJECT_ROOT/tools/BUILD.bazel <<EOF
package_group(
    name = "app_packages",
    packages = ["//app/..."],  # All packages under //app
)

py_library(
    name = "helper",
    srcs = ["helper.py"],
    visibility = [":app_packages"],
)
EOF

# 4. internal
cat > $PROJECT_ROOT/internal/internal_tool.py <<EOF
def _internal_use_only():
    print("Internal logic.")
EOF

cat > $PROJECT_ROOT/internal/BUILD.bazel <<EOF
py_library(
    name = "internal_tool",
    srcs = ["internal_tool.py"],
    visibility = ["//visibility:private"],  # Only internal package
)
EOF

# 5. app
cat > $PROJECT_ROOT/app/main.py <<EOF
from lib import math_lib
from tools import helper
# from internal import internal_tool  # Uncomment to test private access violation

print("2 + 3 =", math_lib.add(2, 3))
helper.print_message("Main executed")
# internal_tool._internal_use_only()  # Uncomment to test private access
EOF

cat > $PROJECT_ROOT/app/BUILD.bazel <<EOF
py_binary(
    name = "main",
    srcs = ["main.py"],
    deps = [
        "//lib:math_lib",       # ✅ allowed
        "//tools:helper",       # ✅ allowed
        # "//internal:internal_tool",  # ❌ not allowed
    ],
)
EOF

echo "✅ Bazel visibility demo project created successfully!"
echo "👉 To build the main app, run:"
echo "   cd $PROJECT_ROOT && bazel build //app:main"


---

## 🚀 How to Use

1. Save the script to a file:


nano generate_bazel_visibility_demo.sh


2. Paste the script above and save.

3. Make it executable:

chmod +x generate_bazel_visibility_demo.sh


4. Run it:


./generate_bazel_visibility_demo.sh


---

## ✅ Result

You’ll get the full structure:


bazel_visibility_demo/
├── WORKSPACE
├── app/
│   ├── BUILD.bazel
│   └── main.py
├── internal/
│   ├── BUILD.bazel
│   └── internal_tool.py
├── lib/
│   ├── BUILD.bazel
│   └── math_lib.py
└── tools/
    ├── BUILD.bazel
    └── helper.py


Now you can:

cd bazel_visibility_demo
bazel build //app:main


###########################################################################

## 🔥 Negative Tests (Try These)

To test the visibility enforcement, try the following:

---

### ❌ Attempt to use `//internal:internal_tool` in `//app`

Uncomment this line in `main.py`:

from internal import internal_tool
internal_tool._internal_use_only()

And add to `BUILD.bazel`:

"//internal:internal_tool",

Then run:

bazel build //app:main


🔴 Error:

Target '//internal:internal_tool' is not visible from target '//app:main'

---

### ❌ Try to use `math_lib` from `//tools`

Create `tools/tool_main.py`:

from lib import math_lib
print(math_lib.add(1, 2))

And update `tools/BUILD.bazel`:

py_binary(
    name = "tool_main",
    srcs = ["tool_main.py"],
    deps = ["//lib:math_lib"],  # ❌ Not allowed (only visible to //app)
)

Then run:

bazel build //tools:tool_main


🔴 Error:

Target '//lib:math_lib' is not visible from target '//tools:tool_main'

---

## ✅ Summary Table

| Target                     | Visibility Setting            | Who Can Use It            |
| -------------------------- | ----------------------------- | ------------------------- |
| `//lib:math_lib`           | `["//app:__pkg__"]`           | Only `//app` package      |
| `//tools:helper`           | `package_group` → `//app/...` | Any package under `//app` |
| `//internal:internal_tool` | `["//visibility:private"]`    | Only `//internal`         |
| `//app:main`               | (depends on allowed targets)  | Builds successfully       |

---


