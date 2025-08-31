#include <fmt/core.h>
#include <nlohmann/json.hpp>
#include <iostream>

int main() {
    using nlohmann::json;

    json j = {
        {"app", "hello"},
        {"version", "1.0"},
        {"features", {"bazel", "conan", "ci"}}
    };

    std::string msg = fmt::format("Hello from {} v{} with {} features!",
                                  j["app"].get<std::string>(),
                                  j["version"].get<std::string>(),
                                  j["features"].size());

    std::cout << msg << "\n";
    std::cout << "JSON: " << j.dump() << "\n";
    return 0;
}

