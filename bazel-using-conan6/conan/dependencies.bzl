# This Bazel module should be loaded by your WORKSPACE file.
# Add these lines to your WORKSPACE one (assuming that you're using the "bazel_layout"):
# load("@//conan:dependencies.bzl", "load_conan_dependencies")
# load_conan_dependencies()

def load_conan_dependencies():
    native.new_local_repository(
        name="fmt",
        path="/home/ilg/.conan2/p/fmt0c92ad97b75de/p",
        build_file="/home/ilg/bazel-conan/Bazel-using-conan6/conan/fmt/BUILD.bazel",
    )
