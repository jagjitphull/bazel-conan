# This Bazel module should be loaded by your WORKSPACE file.
# Add these lines to your WORKSPACE one (assuming that you're using the "bazel_layout"):
# load("@//conan:dependencies.bzl", "load_conan_dependencies")
# load_conan_dependencies()

def load_conan_dependencies():
    native.new_local_repository(
        name="fmt",
        path="/home/ilg/.conan2/p/fmtce0345343b4fd/p",
        build_file="/home/ilg/Nextcloud/bazel_conan_code_eg/examples/cpp-tutorial/bazel-conan-CI-demo8/conan/fmt/BUILD.bazel",
    )
    native.new_local_repository(
        name="nlohmann_json",
        path="/home/ilg/.conan2/p/nlohm0567ffc90cfc1/p",
        build_file="/home/ilg/Nextcloud/bazel_conan_code_eg/examples/cpp-tutorial/bazel-conan-CI-demo8/conan/nlohmann_json/BUILD.bazel",
    )
