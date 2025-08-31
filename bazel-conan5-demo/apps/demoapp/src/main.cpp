#include "logger.hpp"
#include <iostream>

#if __has_include(<fmt/core.h>)
  #include <fmt/core.h>
  #define HAVE_FMT 1
#endif

int main() {
    loggerlib::log_info("hello from loggerlib");

#ifdef HAVE_FMT
    fmt::print("fmt says: {}\n", 42);
#else
    std::cout << "fmt not available; value = " << 42 << "\n";
#endif
    return 0;
}

