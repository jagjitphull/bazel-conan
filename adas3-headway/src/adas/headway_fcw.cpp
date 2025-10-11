//headway_fcw.cpp
#include "headway_fcw.h"
#include <algorithm>

namespace adas {

// --- Metrics ---

std::optional<double> time_to_collision(const Sample& s) {
    if (s.distance_m <= 0.0) return std::nullopt;
    const double v_rel = s.ego_speed_mps - s.lead_speed_mps; // +ve => closing
    if (v_rel <= 0.0) return std::nullopt;
    return s.distance_m / v_rel;
}

std::optional<double> headway(const Sample& s) {
    if (s.distance_m <= 0.0) return std::nullopt;
    if (s.ego_speed_mps <= 0.0) return std::nullopt;
    return s.distance_m / s.ego_speed_mps;
}

Warn classify_ttc(const std::optional<double>& ttc, const Thresholds& th) {
    if (!ttc.has_value()) return Warn::None;
    const double t = *ttc;
    if (t <= th.ttc_imminent_s) return Warn::Imminent;
    if (t <= th.ttc_caution_s)  return Warn::Caution;
    return Warn::None;
}

Warn classify_headway(const std::optional<double>& hw, const Thresholds& th) {
    if (!hw.has_value()) return Warn::None;
    const double tgap = *hw;
    // shorter headway => more dangerous
    if (tgap <= th.headway_imminent_s) return Warn::Imminent;
    if (tgap <= th.headway_caution_s)  return Warn::Caution;
    return Warn::None;
}

Warn aggregate_abw(Warn a, Warn b) {
    // Imminent > Caution > None
    const int rank[] = {0, 1, 2}; // None=0, Caution=1, Imminent=2 (matches enum order)
    auto score = [&](Warn w){ return (w==Warn::None?0:(w==Warn::Caution?1:2)); };
    return (score(a) >= score(b)) ? a : b;
}

const char* to_cstr(Warn w) {
    switch (w) {
        case Warn::None:     return "NONE";
        case Warn::Caution:  return "CAUTION";
        case Warn::Imminent: return "IMMINENT";
        default:             return "UNKNOWN";
    }
}

} // namespace adas
