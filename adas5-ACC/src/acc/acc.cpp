#include "acc.h"
#include <algorithm>
#include <cmath>

namespace acc {

static double clamp(double v, double lo, double hi) {
    return std::max(lo, std::min(v, hi));
}

std::string to_string(Action a) {
    switch (a) {
        case Action::Accelerate: return "Accelerate";
        case Action::Hold:       return "Hold";
        case Action::Brake:      return "Brake";
    }
    return "Hold";
}

AccDecision decide(const AccInput& in, const AccParams& p) {
    // Desired gap grows with ego speed (time headway model)
    const double s_des = p.min_gap + in.ego_speed_mps * p.time_headway;
    const double err   = in.gap_m - s_des;               // +ve means we have extra room
    const double dV    = in.lead_speed_mps - in.ego_speed_mps; // +ve if lead is pulling away

    // Closing fast? bias toward braking sooner
    double bias = 0.0;
    if (dV < 0.0) {
        bias = p.rel_speed_brake_bias * std::abs(dV); // m/s -> shift threshold a bit
    }

    AccDecision out{Action::Hold, 0.0};

    if (err < -(p.deadband + bias)) {
        // Too close: brake proportionally to shortage, capped
        double need = -(err) / (p.time_headway + 1e-6); // simple proportional term
        out.action = Action::Brake;
        out.suggested_accel_mps2 = -clamp(need, 0.0, p.decel_max);
    } else if (err > (p.deadband) && dV >= -0.5) {
        // Plenty of room and lead not much slower: accelerate
        double want = err / (p.time_headway + 1e-6);
        out.action = Action::Accelerate;
        out.suggested_accel_mps2 = clamp(want, 0.0, p.accel_max);
    } else {
        // Sit tight
        out.action = Action::Hold;
        out.suggested_accel_mps2 = 0.0;
    }

    return out;
}

} // namespace acc
