//src/adas/aeb.cpp
#include "aeb.h"
#include <algorithm>
#include <cmath>

namespace adas {

double stopping_distance(const AebInput& in) {
    const double v   = std::max(0.0, in.ego_speed_mps);
    const double a   = std::max(1e-6, in.max_decel_mps2); // avoid div-by-zero
    const double tr  = std::max(0.0, in.reaction_s);
    const double d_react = v * tr;
    const double d_brake = (v * v) / (2.0 * a);
    return d_react + d_brake;
}

bool should_brake(const AebInput& in) {
    const double gap = std::max(0.0, in.gap_m);
    return stopping_distance(in) >= gap;
}

const char* aeb_recommendation(const AebInput& in) {
    return should_brake(in) ? "BRAKE NOW" : "HOLD";
}

} // namespace adas
