//headway_fcw.h
#pragma once
#include <cstdint>
#include <optional>
#include <string>

namespace adas {

// Input sample along a straight lane
struct Sample {
    double ego_speed_mps;   // ego vehicle speed (>0 for headway)
    double lead_speed_mps;  // lead vehicle speed
    double distance_m;      // bumper-to-bumper range
};

// Basic warning levels
enum class Warn : uint8_t { None, Caution, Imminent };

// Configuration thresholds (tunable at runtime if you like)
struct Thresholds {
    // FCW: Time-To-Collision thresholds (closing only)
    double ttc_caution_s  = 3.0;
    double ttc_imminent_s = 1.5;

    // Headway (time gap) thresholds (regardless of closing)
    double headway_caution_s  = 1.6;   // ~“follow at ≥1.6 s”
    double headway_imminent_s = 0.9;
};

// --- Metrics ---

// Returns TTC (distance / closing_speed) only if closing (ego > lead) and distance>0
std::optional<double> time_to_collision(const Sample& s);

// Returns headway (distance / ego_speed) only if ego_speed > 0 and distance>0
std::optional<double> headway(const Sample& s);

// Classify TTC into a warning level
Warn classify_ttc(const std::optional<double>& ttc, const Thresholds& th);

// Classify headway into a warning level (shorter time gap => higher risk)
Warn classify_headway(const std::optional<double>& hw, const Thresholds& th);

// Aggregate rule for an “audible beep warning” (ABW):
// - Trigger at the worst (max severity) of TTC and Headway warnings.
Warn aggregate_abw(Warn from_ttc, Warn from_headway);

// Pretty string
const char* to_cstr(Warn w);

} // namespace adas
