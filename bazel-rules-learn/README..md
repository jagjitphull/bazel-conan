refere on Dropbox

#bazel_rules_tutorial_step_by_step.html

# From the repo root
# Discover targets
bazel query //...

# Build each rule output
bazel build //examples:hello_out
cat bazel-bin/examples/hello_out.txt

bazel build //examples:joined
cat bazel-bin/examples/joined.txt

bazel build //examples:upper_a
cat bazel-bin/examples/upper_a.txt

# Run the app to print all three
bazel run //examples/app:main -- show

# Run the test (uses $(location) to get the exact path)
bazel test //examples:joined_has_alpha_test