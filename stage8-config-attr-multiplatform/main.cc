#include <iostream>
#include "os.h"

int main() {
#if USE_JSON
    std::cout << "[feature] JSON enabled\n";
#else
    std::cout << "[feature] JSON disabled\n";
#endif

#ifdef DEBUG
    std::cout << "[mode]    DEBUG build\n";
#elif defined(NDEBUG)
    std::cout << "[mode]    OPT build\n";
#else
    std::cout << "[mode]    default build\n";
#endif

    std::cout << "[platform] " << os_name() << "\n";
    return 0;
}
