cmake_minimum_required(VERSION 3.20)
project(test_loggerlib LANGUAGES CXX)

find_package(loggerlib REQUIRED)

add_executable(tp src/example.cpp)
target_link_libraries(tp PRIVATE loggerlib::loggerlib)
target_compile_features(tp PRIVATE cxx_std_17)
