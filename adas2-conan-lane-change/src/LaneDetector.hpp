#ifndef LANE_DETECTOR_HPP
#define LANE_DETECTOR_HPP

#include <opencv2/opencv.hpp>
#include <vector>

struct Lane {
    cv::Vec4i line;
    double slope;
    double intercept;
    bool valid;
    
    Lane() : valid(false) {}
};

struct LaneInfo {
    Lane leftLane;
    Lane rightLane;
    cv::Point2f vehiclePosition;
    double laneWidth;
    bool lanesDetected;
    double distanceToLeftLane;
    double distanceToRightLane;
};

class LaneDetector {
private:
    // Image processing parameters
    int lowThreshold;
    int highThreshold;
    int rho;
    double theta;
    int threshold;
    int minLineLength;
    int maxLineGap;
    
    // Region of Interest (ROI) parameters
    cv::Point2f roiVertices[4];
    
    // Helper methods
    cv::Mat preprocessImage(const cv::Mat& image);
    cv::Mat createROIMask(const cv::Mat& image);
    std::vector<cv::Vec4i> detectLines(const cv::Mat& edges);
    void classifyLanes(const std::vector<cv::Vec4i>& lines, Lane& leftLane, Lane& rightLane);
    double calculateSlope(const cv::Vec4i& line);
    double calculateIntercept(const cv::Vec4i& line, double slope);
    bool isValidLane(const Lane& lane);
    
public:
    LaneDetector();
    ~LaneDetector();
    
    // Main processing method
    LaneInfo detectLanes(const cv::Mat& frame);
    
    // Visualization methods
    cv::Mat drawLanes(const cv::Mat& frame, const LaneInfo& laneInfo);
    cv::Mat drawROI(const cv::Mat& frame);
    
    // Configuration methods
    void setROI(const cv::Point2f vertices[4]);
    void setCannyThresholds(int low, int high);
    void setHoughParameters(int rho, double theta, int threshold, int minLen, int maxGap);
};

#endif // LANE_DETECTOR_HPP