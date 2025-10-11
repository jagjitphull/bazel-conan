    //apps/demo_aeb.cpp
    #include "adas/aeb.h"
    #include <iomanip>
    #include <iostream>
    #include <vector>

    using namespace adas;

    int main() {
        // Example scenarios (m/s, m, m/s^2, s)
        std::vector<AebInput> cases = {
            //   ego_v   gap   decel  react
            {   20.0,   80.0,  6.0,   0.4 },   // safe
            {   20.0,   55.0,  6.0,   0.4 },   // borderline
            {   25.0,   60.0,  6.0,   0.5 },   // likely brake
            {   30.0,   70.0,  7.0,   0.3 },   // stronger brakes, maybe safe
            {   22.0,   35.0,  6.0,   0.7 },   // long reaction → brake
        };

        std::cout << "  v(m/s) | gap(m) | a_max | react | d_stop | Recommendation\n";
        std::cout << "---------|--------|-------|-------|--------|-----------------\n";

        for (const auto& in : cases) {
            const double d = stopping_distance(in);
            std::cout << std::fixed << std::setprecision(2)
                    << std::setw(8) << in.ego_speed_mps << " | "
                    << std::setw(6) << in.gap_m         << " | "
                    << std::setw(5) << in.max_decel_mps2<< " | "
                    << std::setw(5) << in.reaction_s    << " | "
                    << std::setw(6) << d                << " | "
                    << aeb_recommendation(in)           << "\n";
        }

        std::cout << "\nNotes:\n"
                    "  d_stop = v*reaction + v^2/(2*a_max). Units are SI (m, s).\n"
                    "  Increase a_max or reduce reaction time to improve safety margin.\n";
        return 0;
    }
