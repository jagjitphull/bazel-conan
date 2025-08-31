cmake_minimum_required(VERSION 3.20)
project(demoapp LANGUAGES CXX)

find_package(fmt REQUIRED CONFIG)           # from CMakeDeps
find_package(loggerlib REQUIRED CONFIG)     # we’ll generate a config for our package too

add_executable(demoapp src/main.cpp)
target_link_libraries(demoapp PRIVATE fmt::fmt loggerlib)
target_compile_features(demoapp PRIVATE cxx_std_17)
