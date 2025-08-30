#include "greet.hpp"

std::string greet() {
#ifdef GREET_UPPERCASE
    return "HELLO, CONAN!";
#else
    return "Hello, Conan!";
#endif
}
