#!/usr/bin/env python3
import sys, os, glob, textwrap

out = sys.argv[1]
inc = os.path.join(out, "include")
lib = os.path.join(out, "lib")

static_lib = os.path.join(lib, "libz.a")
shared_candidates = glob.glob(os.path.join(lib, "libz.so*")) + [
    os.path.join(lib, "libz.dylib"),
    os.path.join(lib, "z.lib"),
]
shared_lib = next((c for c in shared_candidates if os.path.exists(c)), None)

rule = """
package(default_visibility = ["//visibility:public"])
"""

if os.path.exists(static_lib):
    rule += f"""
cc_library(
    name = "zlib",
    hdrs = glob(["include/**/*.h"], allow_empty = True),
    includes = ["include"],
    srcs = ["{os.path.relpath(static_lib, out)}"],
    linkstatic = 1,
)
"""
elif shared_lib:
    # If you switch to shared in install.sh, this path will be used.
    rpath = "-Wl,-rpath,$ORIGIN/../lib"
    rule += f"""
cc_library(
    name = "zlib",
    hdrs = glob(["include/**/*.h"], allow_empty = True),
    includes = ["include"],
    srcs = [],
    linkopts = ["{rpath}", "{os.path.relpath(shared_lib, out)}"],
    linkstatic = 0,
)
"""
else:
    rule += f"""
cc_library(
    name = "zlib",
    hdrs = glob(["include/**/*.h"], allow_empty = True),
    includes = ["include"],
    # No library found under {lib}. Run tools/conan/install.sh again.
)
"""

with open(os.path.join(out, "BUILD.bazel"), "w") as f:
    f.write(textwrap.dedent(rule).lstrip())

print("Wrote", os.path.join(out, "BUILD.bazel"))
