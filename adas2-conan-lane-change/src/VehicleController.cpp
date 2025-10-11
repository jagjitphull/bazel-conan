#include "VehicleController.hpp"
#include <algorithm>

VehicleController::VehicleController() {
    currentState = LaneChangeState::LANE_KEEPING;
    stateStartTime = std::chrono::steady_clock::now();
    
    // Initialize safety parameters
    minLaneWidth = 200.0; // pixels
    safeDistanceMargin = 50.0; // pixels
    maxLaneChangeTime = 5.0; // seconds
    
    // Initialize control parameters
    laneKeepingThreshold = 30.0; // pixels
    laneChangeRequested = false;
    requestedChange = LaneChangeCommand::NONE;
}

VehicleController::~VehicleController() {}

bool VehicleController::isSafeToChangeLanes(const LaneInfo& laneInfo, LaneChangeCommand direction) {
    if (!laneInfo.lanesDetected) return false;
    
    // Check minimum lane width
    if (laneInfo.laneWidth < minLaneWidth) return false;
    
    // Check safe distance margins
    if (direction == LaneChangeCommand::CHANGE_LEFT) {
        return laneInfo.distanceToLeftLane > safeDistanceMargin;
    } else if (direction == LaneChangeCommand::CHANGE_RIGHT) {
        return laneInfo.distanceToRightLane > safeDistanceMargin;
    }
    
    return false;
}

bool VehicleController::isLaneChangeComplete(const LaneInfo& laneInfo) {
    if (!laneInfo.lanesDetected) return false;
    
    // Check if vehicle is reasonably centered in the lane
    double leftDiff = std::abs(laneInfo.distanceToLeftLane - laneInfo.laneWidth/2);
    double rightDiff = std::abs(laneInfo.distanceToRightLane - laneInfo.laneWidth/2);
    
    return (leftDiff < laneKeepingThreshold) && (rightDiff < laneKeepingThreshold);
}

double VehicleController::getElapsedTime() {
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - stateStartTime);
    return duration.count() / 1000.0;
}

void VehicleController::resetStateTimer() {
    stateStartTime = std::chrono::steady_clock::now();
}

LaneChangeState VehicleController::processFrame(const LaneInfo& laneInfo) {
    LaneChangeState nextState = currentState;
    
    switch (currentState) {
        case LaneChangeState::LANE_KEEPING:
            if (laneChangeRequested && requestedChange != LaneChangeCommand::NONE) {
                if (isSafeToChangeLanes(laneInfo, requestedChange)) {
                    if (requestedChange == LaneChangeCommand::CHANGE_LEFT) {
                        nextState = LaneChangeState::INITIATING_LEFT_CHANGE;
                    } else if (requestedChange == LaneChangeCommand::CHANGE_RIGHT) {
                        nextState = LaneChangeState::INITIATING_RIGHT_CHANGE;
                    }
                    laneChangeRequested = false;
                    resetStateTimer();
                }
            }
            break;
            
        case LaneChangeState::INITIATING_LEFT_CHANGE:
            // Brief initialization period
            if (getElapsedTime() > 0.5) {
                nextState = LaneChangeState::CHANGING_LEFT;
                resetStateTimer();
            }
            break;
            
        case LaneChangeState::INITIATING_RIGHT_CHANGE:
            // Brief initialization period
            if (getElapsedTime() > 0.5) {
                nextState = LaneChangeState::CHANGING_RIGHT;
                resetStateTimer();
            }
            break;
            
        case LaneChangeState::CHANGING_LEFT:
        case LaneChangeState::CHANGING_RIGHT:
            // Check for completion or timeout
            if (isLaneChangeComplete(laneInfo)) {
                nextState = LaneChangeState::COMPLETING_CHANGE;
                resetStateTimer();
            } else if (getElapsedTime() > maxLaneChangeTime) {
                // Timeout - abort change
                nextState = LaneChangeState::LANE_KEEPING;
                resetStateTimer();
            }
            break;
            
        case LaneChangeState::COMPLETING_CHANGE:
            // Brief completion period
            if (getElapsedTime() > 1.0) {
                nextState = LaneChangeState::LANE_KEEPING;
                resetStateTimer();
            }
            break;
    }
    
    currentState = nextState;
    return currentState;
}

void VehicleController::requestLaneChange(LaneChangeCommand command) {
    if (currentState == LaneChangeState::LANE_KEEPING) {
        laneChangeRequested = true;
        requestedChange = command;
    }
}

void VehicleController::abortLaneChange() {
    currentState = LaneChangeState::LANE_KEEPING;
    laneChangeRequested = false;
    requestedChange = LaneChangeCommand::NONE;
    resetStateTimer();
}

LaneChangeState VehicleController::getCurrentState() const {
    return currentState;
}

std::string VehicleController::getStateString() const {
    switch (currentState) {
        case LaneChangeState::LANE_KEEPING: return "Lane Keeping";
        case LaneChangeState::INITIATING_LEFT_CHANGE: return "Initiating Left Change";
        case LaneChangeState::CHANGING_LEFT: return "Changing Left";
        case LaneChangeState::INITIATING_RIGHT_CHANGE: return "Initiating Right Change";
        case LaneChangeState::CHANGING_RIGHT: return "Changing Right";
        case LaneChangeState::COMPLETING_CHANGE: return "Completing Change";
        default: return "Unknown";
    }
}

bool VehicleController::isChangingLanes() const {
    return currentState != LaneChangeState::LANE_KEEPING;
}

void VehicleController::setSafetyParameters(double minWidth, double safeDistance, double maxTime) {
    minLaneWidth = minWidth;
    safeDistanceMargin = safeDistance;
    maxLaneChangeTime = maxTime;
}

void VehicleController::setControlParameters(double keepingThreshold) {
    laneKeepingThreshold = keepingThreshold;
}