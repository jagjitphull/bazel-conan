cmake_minimum_required(VERSION 3.15)
project(hello_test CXX)
enable_testing()

find_package(hello REQUIRED CONFIG)

add_executable(example src/example.cpp)
target_link_libraries(example PRIVATE hello::hello)

add_test(NAME run_example COMMAND example)

