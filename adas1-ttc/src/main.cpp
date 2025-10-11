#include "adas/acc.h"
#include <iostream>
#include <vector>
#include <iomanip>

using namespace adas;

int main() {
    // Simulated scenario: ego is faster than lead car, distance is shrinking.
    std::vector<SensorSample> samples = {
        {22.0, 18.0, 40.0},  // speeds in m/s (~79 vs ~65 km/h), 40 m gap
        {22.0, 18.0, 36.0},
        {22.0, 18.0, 31.0},
        {22.0, 18.0, 27.0},
        {22.0, 18.0, 24.0},
        {22.0, 18.0, 20.0},
        {22.0, 18.0, 16.0},
        {22.0, 18.0, 12.0},
        {22.0, 18.0,  9.0},
        {22.0, 18.0,  6.0},
    };

    // Kalman filter with dt=0.5s
    CvKalman1D kf(0.5);
    bool first = true;

    std::cout << " step | dist_meas | dist_filt | rel_v_filt | TTC(s) | WARN\n";
    std::cout << "------|-----------|-----------|------------|--------|------\n";

    for (size_t i = 0; i < samples.size(); ++i) {
        const auto& s = samples[i];

        // Filter only the distance; rel speed emerges from the model
        if (first) { kf.init(s.distance_m); first = false; }
        auto [d_filt, v_rel_filt] = kf.update(s.distance_m);

        // Compute TTC using *measured* rel speed for simplicity here
        auto ttc = time_to_collision(s);
        auto warn = classify_ttc(ttc, /*t1=*/3.0, /*t2=*/1.5);

        std::cout << std::setw(5) << i
                  << " | " << std::setw(9) << std::fixed << std::setprecision(2) << s.distance_m
                  << " | " << std::setw(9) << d_filt
                  << " | " << std::setw(10) << v_rel_filt
                  << " | " << std::setw(6)
                  << (ttc.has_value() ? std::to_string(*ttc) : std::string("--"))
                  << " | " << to_string(warn)
                  << "\n";
    }

    std::cout << "\nLegend: TTC thresholds -> Caution <= 3.0s, Imminent <= 1.5s\n";
    return 0;
}
