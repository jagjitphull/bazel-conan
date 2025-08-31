#include "loggerlib/loggerlib.h"

int main() {
    loggerlib::log_info("hello from test_package");
    return loggerlib::version().empty(); // return 0 if non-empty
}
