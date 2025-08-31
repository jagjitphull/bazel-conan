#include "loggerlib/loggerlib.h"
#include <iostream>

namespace loggerlib {
    void log_info(const std::string& msg) {
        std::cout << "[LOGGERLIB] " << msg << std::endl;
    }
    std::string version() {
        return "1.0.0";
    }
}
