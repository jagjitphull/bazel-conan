Plan
Create MODULE.bazel to initialize a Bazel 8+ workspace.
Write hello/hello.bzl:
    hello_binary rule (executable) that writes a Bash script printing a message.
    hello_world macro that calls hello_binary with "Hello, World!".

In BUILD.bazel:
    load rule and macro.
    Define one target using the macro and one using the rule directly.

Run with bazel run //:hello and bazel run //:greet_bob.

Execute the setup.sh script, creates respective structure - all of it in 'Plan' above

./setup.sh  

bazel run //:hello         # prints "Hello, World!"
bazel run //:greet_bob     # prints custom message
bazel build //:hello       # builds the generated script
