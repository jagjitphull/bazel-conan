cmake_minimum_required(VERSION 3.20)
project(conan_remotes_demo_app LANGUAGES CXX)

find_package(fmt REQUIRED)
find_package(loggerlib REQUIRED)

add_executable(app main.cpp)
target_link_libraries(app PRIVATE fmt::fmt loggerlib::loggerlib)
target_compile_features(app PRIVATE cxx_std_17)

