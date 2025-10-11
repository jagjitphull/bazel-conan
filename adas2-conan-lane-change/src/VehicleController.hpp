#ifndef VEHICLE_CONTROLLER_HPP
#define VEHICLE_CONTROLLER_HPP

#include "LaneDetector.hpp"
#include <chrono>

enum class LaneChangeState {
    LANE_KEEPING,
    INITIATING_LEFT_CHANGE,
    CHANGING_LEFT,
    INITIATING_RIGHT_CHANGE,
    CHANGING_RIGHT,
    COMPLETING_CHANGE
};

enum class LaneChangeCommand {
    NONE,
    CHANGE_LEFT,
    CHANGE_RIGHT,
    ABORT_CHANGE
};

class VehicleController {
private:
    LaneChangeState currentState;
    std::chrono::steady_clock::time_point stateStartTime;
    
    // Safety parameters
    double minLaneWidth;
    double safeDistanceMargin;
    double maxLaneChangeTime; // seconds
    
    // Control parameters
    double laneKeepingThreshold;
    bool laneChangeRequested;
    LaneChangeCommand requestedChange;
    
    // Helper methods
    bool isSafeToChangeLanes(const LaneInfo& laneInfo, LaneChangeCommand direction);
    bool isLaneChangeComplete(const LaneInfo& laneInfo);
    double getElapsedTime();
    void resetStateTimer();
    
public:
    VehicleController();
    ~VehicleController();
    
    // Main control method
    LaneChangeState processFrame(const LaneInfo& laneInfo);
    
    // Command interface
    void requestLaneChange(LaneChangeCommand command);
    void abortLaneChange();
    
    // Status methods
    LaneChangeState getCurrentState() const;
    std::string getStateString() const;
    bool isChangingLanes() const;
    
    // Configuration
    void setSafetyParameters(double minWidth, double safeDistance, double maxTime);
    void setControlParameters(double keepingThreshold);
};

#endif // VEHICLE_CONTROLLER_HPP