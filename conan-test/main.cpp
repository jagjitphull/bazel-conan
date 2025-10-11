#include <iostream>
#include <zlib.h>

int main() {
    std::cout << "Hello from demoapp!\n";
    std::cout << "Using zlib version: " << zlibVersion() << std::endl;
    return 0;
}

