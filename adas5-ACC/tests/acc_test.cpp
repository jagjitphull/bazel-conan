#include "acc/acc.h"
#include <cassert>
#include <cmath>

int main() {
    using namespace acc;

    AccParams p; // defaults

    // Case 1: Too close -> Brake
    {
        AccInput in{25.0, 25.0, 5.0}; // fast, tiny gap
        auto d = decide(in, p);
        assert(d.action == Action::Brake);
        assert(d.suggested_accel_mps2 <= 0.0);
    }

    // Case 2: Plenty of room and lead similar speed -> Accelerate
    {
        AccInput in{15.0, 15.0, 60.0};
        auto d = decide(in, p);
        assert(d.action == Action::Accelerate);
        assert(d.suggested_accel_mps2 >= 0.0);
    }

    // Case 3: Within deadband -> Hold
    {
        double s_des = p.min_gap + 20.0 * p.time_headway; // 2 + 36 = 38
        AccInput in{20.0, 19.5, s_des}; // exactly at desired -> Hold
        auto d = decide(in, p);
        assert(d.action == Action::Hold);
        assert(std::abs(d.suggested_accel_mps2) < 1e-9);
    }

    return 0;
}
