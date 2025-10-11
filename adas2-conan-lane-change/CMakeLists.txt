cmake_minimum_required(VERSION 3.10)
project(LaneChangingADAS)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(lane_adas 
    src/main.cpp
    src/LaneDetector.cpp
    src/VehicleController.cpp
)

target_link_libraries(lane_adas ${OpenCV_LIBS})