#include <fmt/core.h>
#include <string>
#include <cassert>

int main() {
    std::string s = fmt::format("sum={} ", 2 + 3);
    assert(s.find("5") != std::string::npos);
    return 0;
}
