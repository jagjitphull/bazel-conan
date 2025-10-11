//aeb.h
#pragma once
#include <string>

namespace adas {

// Simple AEB inputs
struct AebInput {
    double ego_speed_mps;   // ego speed (m/s)
    double gap_m;           // distance to obstacle/lead car (m)
    double max_decel_mps2;  // positive magnitude, e.g. 6.0 m/s^2
    double reaction_s;      // driver/system reaction time (s), e.g. 0.3–0.7
};

// Kinematic stopping distance with reaction delay:
// d_stop = v * reaction + v^2 / (2 * a_max)
// (Assumes constant decel after reaction delay. a_max is positive magnitude.)
double stopping_distance(const AebInput& in);

// True if we should brake now (i.e., stopping distance >= gap)
bool should_brake(const AebInput& in);

// Convenience: textual recommendation ("BRAKE NOW" / "HOLD")
const char* aeb_recommendation(const AebInput& in);

} // namespace adas
