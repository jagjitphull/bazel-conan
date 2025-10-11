cmake_minimum_required(VERSION 3.15)
project(demoapp LANGUAGES CXX)

find_package(ZLIB REQUIRED)

add_executable(demoapp main.cpp)
target_link_libraries(demoapp PRIVATE ZLIB::ZLIB)

