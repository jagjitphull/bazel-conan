//demo_headway.cpp

#include "adas/headway_fcw.h"
#include <iomanip>
#include <iostream>
#include <vector>

using namespace adas;

int main() {
    // Scenario: same speeds first (no TTC), then ego speeds up (closing),
    // distance shrinks; headway will also get small.
    std::vector<Sample> seq = {
        {18.0, 18.0, 36.0}, // coasting  ~65 km/h, 36 m gap → hw=2.0s, TTC=—
        {18.0, 18.0, 30.0}, // still no TTC; headway trending down
        {20.0, 18.0, 27.0}, // closing slowly → TTC appears
        {22.0, 18.0, 24.0},
        {22.0, 18.0, 20.0},
        {22.0, 18.0, 16.0},
        {22.0, 18.0, 12.0},
        {22.0, 18.0,  9.0},
        {22.0, 18.0,  6.0},
    };

    Thresholds th; // use defaults

    std::cout << " step | dist(m) | v_ego | v_lead |  HW(s) |  TTC(s) | FCW  | HW-W |  ABW\n";
    std::cout << "------|---------|-------|--------|--------|---------|------|------|------\n";
    for (size_t i = 0; i < seq.size(); ++i) {
        const auto& s = seq[i];
        auto hw  = headway(s);
        auto ttc = time_to_collision(s);

        Warn w_ttc = classify_ttc(ttc, th);
        Warn w_hw  = classify_headway(hw, th);
        Warn abw   = aggregate_abw(w_ttc, w_hw);

        auto show = [](const std::optional<double>& v) {
            if (!v) return std::string("  --  ");
            std::ostringstream os;
            os << std::fixed << std::setprecision(2) << std::setw(6) << *v;
            return os.str();
        };

        std::cout << std::setw(5) << i << " | "
                  << std::setw(7) << s.distance_m << " | "
                  << std::setw(5) << s.ego_speed_mps << " | "
                  << std::setw(6) << s.lead_speed_mps << " | "
                  << show(hw)  << " | "
                  << show(ttc) << " | "
                  << std::setw(6) << to_cstr(w_ttc) << " | "
                  << std::setw(6) << to_cstr(w_hw)  << " | "
                  << std::setw(6) << to_cstr(abw)
                  << "\n";

        // Optional: make an obvious "beep" when ABW is CAUTION/IMMINENT
        if (abw != Warn::None) {
            std::cout << "  >>> BEEP (" << to_cstr(abw) << ")\n";
        }
    }

    std::cout << "\nLegend: FCW uses TTC thresholds; HW-W uses headway thresholds; "
                 "ABW is the stricter of the two.\n";
    return 0;
}
