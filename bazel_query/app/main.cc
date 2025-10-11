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