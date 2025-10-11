#include "LaneDetector.hpp"
#include <algorithm>
#include <cmath>

LaneDetector::LaneDetector() {
    // Initialize Canny edge detection parameters
    lowThreshold = 50;
    highThreshold = 150;
    
    // Initialize Hough transform parameters
    rho = 1;
    theta = CV_PI / 180;
    threshold = 50;
    minLineLength = 50;
    maxLineGap = 150;
    
    // Default ROI (will be adjusted based on image size)
    roiVertices[0] = cv::Point2f(0, 0);
    roiVertices[1] = cv::Point2f(0, 0);
    roiVertices[2] = cv::Point2f(0, 0);
    roiVertices[3] = cv::Point2f(0, 0);
}

LaneDetector::~LaneDetector() {}

cv::Mat LaneDetector::preprocessImage(const cv::Mat& image) {
    cv::Mat gray, blur, edges;
    
    // Convert to grayscale
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    
    // Apply Gaussian blur to reduce noise
    cv::GaussianBlur(gray, blur, cv::Size(5, 5), 0);
    
    // Apply Canny edge detection
    cv::Canny(blur, edges, lowThreshold, highThreshold);
    
    return edges;
}

cv::Mat LaneDetector::createROIMask(const cv::Mat& image) {
    cv::Mat mask = cv::Mat::zeros(image.size(), CV_8UC1);
    
    // If ROI not set, use default values based on image size
    if (roiVertices[0].x == 0 && roiVertices[0].y == 0) {
        int height = image.rows;
        int width = image.cols;
        
        roiVertices[0] = cv::Point2f(width * 0.1, height);
        roiVertices[1] = cv::Point2f(width * 0.4, height * 0.6);
        roiVertices[2] = cv::Point2f(width * 0.6, height * 0.6);
        roiVertices[3] = cv::Point2f(width * 0.9, height);
    }
    
    // Create polygon mask
    std::vector<cv::Point> roi_points;
    for (int i = 0; i < 4; i++) {
        roi_points.push_back(cv::Point(roiVertices[i].x, roiVertices[i].y));
    }
    
    cv::fillPoly(mask, std::vector<std::vector<cv::Point>>{roi_points}, cv::Scalar(255));
    
    return mask;
}

std::vector<cv::Vec4i> LaneDetector::detectLines(const cv::Mat& edges) {
    std::vector<cv::Vec4i> lines;
    
    // Apply ROI mask
    cv::Mat maskedEdges;
    cv::Mat mask = createROIMask(edges);
    cv::bitwise_and(edges, mask, maskedEdges);
    
    // Detect lines using Hough transform
    cv::HoughLinesP(maskedEdges, lines, rho, theta, threshold, minLineLength, maxLineGap);
    
    return lines;
}

double LaneDetector::calculateSlope(const cv::Vec4i& line) {
    if (line[2] - line[0] == 0) return 0; // Avoid division by zero
    return static_cast<double>(line[3] - line[1]) / (line[2] - line[0]);
}

double LaneDetector::calculateIntercept(const cv::Vec4i& line, double slope) {
    return line[1] - slope * line[0];
}

void LaneDetector::classifyLanes(const std::vector<cv::Vec4i>& lines, Lane& leftLane, Lane& rightLane) {
    std::vector<Lane> leftCandidates, rightCandidates;
    
    for (const auto& line : lines) {
        double slope = calculateSlope(line);
        
        // Filter out horizontal lines and very steep lines
        if (std::abs(slope) < 0.3 || std::abs(slope) > 3.0) continue;
        
        Lane lane;
        lane.line = line;
        lane.slope = slope;
        lane.intercept = calculateIntercept(line, slope);
        lane.valid = true;
        
        // Classify based on slope
        if (slope < 0) {
            leftCandidates.push_back(lane);
        } else {
            rightCandidates.push_back(lane);
        }
    }
    
    // Select best lane from candidates (longest line)
    auto selectBestLane = [](const std::vector<Lane>& candidates) -> Lane {
        if (candidates.empty()) return Lane();
        
        Lane best = candidates[0];
        double maxLength = 0;
        
        for (const auto& candidate : candidates) {
            double length = cv::norm(cv::Point2f(candidate.line[0], candidate.line[1]) - 
                                   cv::Point2f(candidate.line[2], candidate.line[3]));
            if (length > maxLength) {
                maxLength = length;
                best = candidate;
            }
        }
        return best;
    };
    
    leftLane = selectBestLane(leftCandidates);
    rightLane = selectBestLane(rightCandidates);
}

bool LaneDetector::isValidLane(const Lane& lane) {
    return lane.valid && std::abs(lane.slope) > 0.3;
}

LaneInfo LaneDetector::detectLanes(const cv::Mat& frame) {
    LaneInfo laneInfo;
    
    // Preprocess image
    cv::Mat edges = preprocessImage(frame);
    
    // Detect lines
    std::vector<cv::Vec4i> lines = detectLines(edges);
    
    // Classify lanes
    classifyLanes(lines, laneInfo.leftLane, laneInfo.rightLane);
    
    // Calculate vehicle position and lane information
    laneInfo.lanesDetected = isValidLane(laneInfo.leftLane) && isValidLane(laneInfo.rightLane);
    
    if (laneInfo.lanesDetected) {
        // Assume vehicle is in center of image horizontally
        int vehicleX = frame.cols / 2;
        int vehicleY = frame.rows - 50; // Near bottom of image
        laneInfo.vehiclePosition = cv::Point2f(vehicleX, vehicleY);
        
        // Calculate intersection points at vehicle Y position
        double leftX = (vehicleY - laneInfo.leftLane.intercept) / laneInfo.leftLane.slope;
        double rightX = (vehicleY - laneInfo.rightLane.intercept) / laneInfo.rightLane.slope;
        
        laneInfo.laneWidth = rightX - leftX;
        laneInfo.distanceToLeftLane = vehicleX - leftX;
        laneInfo.distanceToRightLane = rightX - vehicleX;
    }
    
    return laneInfo;
}

cv::Mat LaneDetector::drawLanes(const cv::Mat& frame, const LaneInfo& laneInfo) {
    cv::Mat result = frame.clone();
    
    // Draw detected lanes
    if (isValidLane(laneInfo.leftLane)) {
        cv::line(result, 
                cv::Point(laneInfo.leftLane.line[0], laneInfo.leftLane.line[1]),
                cv::Point(laneInfo.leftLane.line[2], laneInfo.leftLane.line[3]),
                cv::Scalar(0, 255, 0), 3);
    }
    
    if (isValidLane(laneInfo.rightLane)) {
        cv::line(result, 
                cv::Point(laneInfo.rightLane.line[0], laneInfo.rightLane.line[1]),
                cv::Point(laneInfo.rightLane.line[2], laneInfo.rightLane.line[3]),
                cv::Scalar(0, 255, 0), 3);
    }
    
    // Draw vehicle position
    cv::circle(result, laneInfo.vehiclePosition, 10, cv::Scalar(0, 0, 255), -1);
    
    // Draw lane information text
    if (laneInfo.lanesDetected) {
        cv::putText(result, "Lane Width: " + std::to_string(static_cast<int>(laneInfo.laneWidth)), 
                   cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 255, 255), 2);
        cv::putText(result, "Dist to Left: " + std::to_string(static_cast<int>(laneInfo.distanceToLeftLane)), 
                   cv::Point(10, 60), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 255, 255), 2);
        cv::putText(result, "Dist to Right: " + std::to_string(static_cast<int>(laneInfo.distanceToRightLane)), 
                   cv::Point(10, 90), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 255, 255), 2);
    }
    
    return result;
}

cv::Mat LaneDetector::drawROI(const cv::Mat& frame) {
    cv::Mat result = frame.clone();
    cv::Mat mask = createROIMask(frame);
    
    // Draw ROI overlay
    cv::Mat colorMask;
    cv::cvtColor(mask, colorMask, cv::COLOR_GRAY2BGR);
    cv::addWeighted(result, 0.8, colorMask, 0.2, 0, result);
    
    return result;
}

// Configuration methods
void LaneDetector::setROI(const cv::Point2f vertices[4]) {
    for (int i = 0; i < 4; i++) {
        roiVertices[i] = vertices[i];
    }
}

void LaneDetector::setCannyThresholds(int low, int high) {
    lowThreshold = low;
    highThreshold = high;
}

void LaneDetector::setHoughParameters(int rho_val, double theta_val, int threshold_val, int minLen, int maxGap) {
    rho = rho_val;
    theta = theta_val;
    threshold = threshold_val;
    minLineLength = minLen;
    maxLineGap = maxGap;
}