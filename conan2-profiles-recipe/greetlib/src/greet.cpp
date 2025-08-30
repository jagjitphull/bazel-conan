#include "greet.hpp"

std::string greet() {
#ifdef GREET_UPPERCASE
    return "HELLO JP, CONAN v2!";
#else
    return "Hello jp, Conan v2!";
#endif
}

