cmake_minimum_required(VERSION 3.18)
project(app_profiles_demo LANGUAGES CXX)

find_package(fmt REQUIRED)
find_package(greetlib REQUIRED CONFIG)   # <- CONFIG REQUIRED matters

add_executable(app src/main.cpp)
target_link_libraries(app PRIVATE fmt::fmt greetlib::greetlib)

