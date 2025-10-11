Overview
This project implements a basic Advanced Driver Assistance System (ADAS) for lane changing using C++ and OpenCV. 
The system uses computer vision to detect lanes, monitors vehicle position, and makes lane changing decisions based on safety criteria.

Learning Objectives

Understand computer vision techniques for lane detection
Implement basic ADAS decision-making algorithms
Learn about automotive safety systems
Practice C++ programming for real-time applications

System Architecture
[Camera Input] → [Lane Detection] → [Position Analysis] → [Decision Engine] → [Lane Change Control]

Prerequisites
Required Libraries

OpenCV 4.x
C++17 or later
CMake 3.10+

sudo apt update
sudo apt install libopencv-dev cmake build-essential

lane_changing_adas/
├── src/
│   ├── main.cpp
│   ├── LaneDetector.hpp
│   ├── LaneDetector.cpp
│   ├── VehicleController.hpp
│   ├── VehicleController.cpp
│   └── SafetyManager.hpp
├── data/
│   └── test_videos/
├── CMakeLists.txt
└── README.md

Building and Running (After creating all sources)
mkdir build
cd build
cmake ..
make

Run the Application
# Using webcam
./lane_adas

# Using video file
./lane_adas ../data/test_video.mp4 ( Python synthetic video generator is given in data dir )

###################################################################
when we run - ./lane_adas ../data/test_video.mp4, what expected output

Expected output when running the lane changing ADAS system with a test video. Let me walk through what the system should do and what the user should see when they run the command.
when the system runs, it will:

Open the video file
Process each frame for lane detection
Run the vehicle controller state machine
Display a window with visual feedback
Accept keyboard commands for lane change requests
Show real-time information overlays

$ ./lane_adas ../data/test_video.mp4
Opening video file: ../data/test_video.mp4
Controls:
  'a' - Request left lane change
  'd' - Request right lane change
  's' - Abort lane change
  'r' - Show ROI
  'q' - Quit

Visual Display Window:
The system opens a window titled "Lane Changing ADAS" showing:
1. Lane Detection Overlay

Green lines: Detected left and right lane markings
Red circle: Vehicle position (center-bottom of frame)
ROI highlight: When 'r' is pressed, shows the region of interest

Real-time Information Display (Text overlays)
Lane Width: 245        (top-left, white text)
Dist to Left: 122      
Dist to Right: 123     
State: Lane Keeping    (yellow text)

Lane Change Status
When changing lanes: "LANE CHANGING" appears in red text (top-right)

Interactive Behavior:
    Normal Operation:
    # When you press 'a' (left lane change)
    Requested left lane change

    # Console shows state transitions:
    State: Lane Keeping → Initiating Left Change → Changing Left → Completing Change → Lane Keeping

    Safety Rejections:
    # If unsafe conditions detected:
    Lane change request rejected - insufficient safety margin