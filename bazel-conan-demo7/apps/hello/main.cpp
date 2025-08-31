#include <iostream>
#include <zlib.h>

int main() {
    std::cout << "Hello from Bazel + Conan + zlib! zlib version: " << zlibVersion() << "\n";
    return 0;
}
