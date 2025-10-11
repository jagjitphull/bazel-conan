#include "acc.h"
#include <cmath>
#include <sstream>

namespace adas {

std::optional<double> time_to_collision(const SensorSample& s) {
    if (s.distance_m <= 0.0) return std::nullopt;
    const double rel = s.rel_speed_mps(); // positive => closing
    if (rel <= 0.0) return std::nullopt;
    return s.distance_m / rel;
}

WarningLevel classify_ttc(std::optional<double> ttc, double t1, double t2) {
    if (!ttc.has_value()) return WarningLevel::None;
    const double t = *ttc;
    if (t <= t2) return WarningLevel::Imminent;
    if (t <= t1) return WarningLevel::Caution;
    return WarningLevel::None;
}

CvKalman1D::CvKalman1D(double dt) : dt_(dt) {
    F_ << 1.0, dt_, 0.0, 1.0;
    H_ << 1.0, 0.0;
    // Reasonable defaults (tunable)
    Q_ << 0.5, 0.0,
          0.0, 1.0;     // process noise
    R_ << 2.0;          // measurement noise (m^2)
    P_.setIdentity();
    x_.setZero();
}

void CvKalman1D::init(double distance_m) {
    x_ << distance_m, 0.0;
    P_.setIdentity();
    initialized_ = true;
}

std::pair<double,double> CvKalman1D::update(double z_meas) {
    if (!initialized_) init(z_meas);

    // Predict
    x_ = F_ * x_;
    P_ = F_ * P_ * F_.transpose() + Q_;

    // Update
    Eigen::Matrix<double,1,1> y;
    y(0,0) = z_meas - (H_ * x_)(0,0);
    Eigen::Matrix<double,1,1> S = H_ * P_ * H_.transpose() + R_;
    Eigen::Matrix<double,2,1> K = P_ * H_.transpose() * S.inverse();

    x_ = x_ + K * y(0,0);
    Eigen::Matrix2d I = Eigen::Matrix2d::Identity();
    P_ = (I - K * H_) * P_;

    return {x_(0), x_(1)};
}

std::string to_string(WarningLevel w) {
    switch (w) {
        case WarningLevel::None:     return "NONE";
        case WarningLevel::Caution:  return "CAUTION";
        case WarningLevel::Imminent: return "IMMINENT";
        default: return "UNKNOWN";
    }
}

} // namespace adas
