#include "logger.hpp"
#include <iostream>

namespace loggerlib {
void log_info(const std::string& msg) {
    std::cout << "[INFO] " << msg << std::endl;
}
}
