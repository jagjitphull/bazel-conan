#pragma once
#include <Eigen/Dense>
#include <cstdint>
#include <optional>
#include <string>

namespace adas {

// Simple sensor sample: ego + lead vehicle along a straight lane.
struct SensorSample {
    double ego_speed_mps;   // ego vehicle speed
    double lead_speed_mps;  // lead vehicle speed
    double distance_m;      // range to lead vehicle (front bumper to rear bumper)

    double rel_speed_mps() const { return ego_speed_mps - lead_speed_mps; }
};

// Compute Time-To-Collision (TTC = distance / relative closing speed).
// Returns nullopt if not closing (rel_speed <= 0) or distance <= 0.
std::optional<double> time_to_collision(const SensorSample& s);

// Warning levels based on TTC thresholds.
enum class WarningLevel : uint8_t { None, Caution, Imminent };

// Map TTC to warning level (t1 mild, t2 severe).
WarningLevel classify_ttc(std::optional<double> ttc,
                          double t1_sec = 3.0,
                          double t2_sec = 1.5);

// A tiny constant-velocity Kalman filter in 1D to smooth distance & relative speed.
// State x = [distance; rel_speed]. Discrete model:
//  x_k+1 = [1 dt; 0 1] x_k + w_k
//  z_k   = [1 0] x_k + v_k
class CvKalman1D {
public:
    explicit CvKalman1D(double dt);

    // Initialize with first measurement (range meters).
    void init(double distance_m);

    // Feed a new range measurement; returns smoothed (distance, rel_speed).
    std::pair<double,double> update(double measured_distance_m);

private:
    bool initialized_ = false;
    double dt_;
    Eigen::Vector2d x_;   // state
    Eigen::Matrix2d P_;   // covariance
    Eigen::Matrix2d F_;   // dynamics
    Eigen::Matrix<double,1,2> H_; // measurement
    Eigen::Matrix2d Q_;   // process noise
    Eigen::Matrix<double,1,1> R_; // measurement noise
};

std::string to_string(WarningLevel w);

} // namespace adas
