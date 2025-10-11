#include "logger.h"
#include <iostream>

void log(const std::string& message) {
    std::cout << "[LOG] " << message << std::endl;
}