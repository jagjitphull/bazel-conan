#include <iostream>
#include "hello.h"   // comes from the package include/

int main() {
    std::cout << hello::say_hello() << "\n";
    return 0;
}

