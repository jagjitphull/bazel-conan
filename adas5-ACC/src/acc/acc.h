//acc.h
#pragma once
#include <string>
#include <tuple>

namespace acc {

enum class Action { Accelerate, Hold, Brake };

struct AccParams {
    double time_headway = 1.8;   // seconds
    double min_gap      = 2.0;   // meters at standstill
    double accel_max    = 2.0;   // m/s^2 (cap for suggested accel)
    double decel_max    = 3.0;   // m/s^2 (cap for suggested braking magnitude)
    double deadband     = 1.0;   // meters of comfort band around desired gap
    double rel_speed_brake_bias = 0.5; // more conservative if closing fast
};

struct AccInput {
    double ego_speed_mps;   // m/s
    double lead_speed_mps;  // m/s
    double gap_m;           // meters (front bumper to rear bumper)
};

struct AccDecision {
    Action action;
    double suggested_accel_mps2; // positive for accel, negative for brake, ~0 for hold
};

/// Rule-based ACC decision.
/// Returns action + a suggested acceleration (for realism).
AccDecision decide(const AccInput& in, const AccParams& p = {});

/// Utility to stringify Action.
std::string to_string(Action a);

} // namespace acc
