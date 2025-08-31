#include <fmt/core.h>
#include "loggerlib/loggerlib.h"

int main() {
    loggerlib::log_info("Demo app starting...");
    fmt::print("fmt says: {}\n", "Hello from ConanCenter!");
    fmt::print("loggerlib version: {}\n", loggerlib::version());
    return 0;
}
