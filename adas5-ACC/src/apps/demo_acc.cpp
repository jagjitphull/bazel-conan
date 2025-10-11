//src/apps/demo_acc.cpp

#include <iostream>
#include <sstream>
#include <vector>
#include "acc/acc.h"

using acc::AccInput;
using acc::AccParams;
using acc::AccDecision;
using acc::Action;

int main(int argc, char** argv) {
    std::cout << "ACC demo. Enter lines as: ego_mps lead_mps gap_m\n";
    std::cout << "Example: 20 18 25\n";
    std::cout << "Ctrl+D to end.\n\n";

    AccParams params; // default values; trainees can tweak via code

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        AccInput in{};
        if (!(iss >> in.ego_speed_mps >> in.lead_speed_mps >> in.gap_m)) {
            std::cerr << "Parse error. Use: ego_mps lead_mps gap_m\n";
            continue;
        }

        AccDecision d = acc::decide(in, params);
        std::cout
            << "Input  ego=" << in.ego_speed_mps << " m/s, lead=" << in.lead_speed_mps
            << " m/s, gap=" << in.gap_m << " m  ->  "
            << acc::to_string(d.action)
            << " (suggested " << d.suggested_accel_mps2 << " m/s^2)\n";
    }
    return 0;
}
