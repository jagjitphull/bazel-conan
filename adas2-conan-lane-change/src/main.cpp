#include <opencv2/opencv.hpp>
#include <iostream>
#include "LaneDetector.hpp"
#include "VehicleController.hpp"

int main(int argc, char* argv[]) {
    // Initialize components
    LaneDetector laneDetector;
    VehicleController vehicleController;
    
    // Open video capture (0 for webcam, or provide video file path)
    cv::VideoCapture cap;
    
    if (argc > 1) {
        // Use provided video file
        cap.open(argv[1]);
        std::cout << "Opening video file: " << argv[1] << std::endl;
    } else {
        // Use webcam
        cap.open(0);
        std::cout << "Using webcam input" << std::endl;
    }
    
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open video source" << std::endl;
        return -1;
    }
    
    // Configure lane detector for better performance
    laneDetector.setCannyThresholds(50, 150);
    laneDetector.setHoughParameters(1, CV_PI/180, 50, 50, 150);
    
    // Configure vehicle controller
    vehicleController.setSafetyParameters(200.0, 50.0, 5.0);
    vehicleController.setControlParameters(30.0);
    
    cv::Mat frame;
    char key = 0;
    
    std::cout << "Controls:" << std::endl;
    std::cout << "  'a' - Request left lane change" << std::endl;
    std::cout << "  'd' - Request right lane change" << std::endl;
    std::cout << "  's' - Abort lane change" << std::endl;
    std::cout << "  'r' - Show ROI" << std::endl;
    std::cout << "  'q' - Quit" << std::endl;
    
    bool showROI = false;
    
    while (key != 'q' && key != 27) { // 27 is ESC key
        cap >> frame;
        if (frame.empty()) {
            std::cout << "End of video or error reading frame" << std::endl;
            break;
        }
        
        // Process frame for lane detection
        LaneInfo laneInfo = laneDetector.detectLanes(frame);
        
        // Update vehicle controller
        LaneChangeState state = vehicleController.processFrame(laneInfo);
        
        // Create visualization
        cv::Mat display;
        if (showROI) {
            display = laneDetector.drawROI(frame);
        } else {
            display = laneDetector.drawLanes(frame, laneInfo);
        }
        
        // Draw state information
        cv::putText(display, "State: " + vehicleController.getStateString(), 
                   cv::Point(10, 120), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 255), 2);
        
        // Draw lane change status
        if (vehicleController.isChangingLanes()) {
            cv::putText(display, "LANE CHANGING", 
                       cv::Point(display.cols - 200, 30), 
                       cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 0, 255), 2);
        }
        
        // Display frame
        cv::imshow("Lane Changing ADAS", display);
        
        // Handle keyboard input
        key = cv::waitKey(30) & 0xFF;
        
        switch (key) {
            case 'a':
                vehicleController.requestLaneChange(LaneChangeCommand::CHANGE_LEFT);
                std::cout << "Requested left lane change" << std::endl;
                break;
            case 'd':
                vehicleController.requestLaneChange(LaneChangeCommand::CHANGE_RIGHT);
                std::cout << "Requested right lane change" << std::endl;
                break;
            case 's':
                vehicleController.abortLaneChange();
                std::cout << "Aborted lane change" << std::endl;
                break;
            case 'r':
                showROI = !showROI;
                std::cout << "ROI display: " << (showROI ? "ON" : "OFF") << std::endl;
                break;
        }
    }
    
    cap.release();
    cv::destroyAllWindows();
    
    return 0;
}