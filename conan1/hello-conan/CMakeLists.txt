cmake_minimum_required(VERSION 3.21)
project(hello_conan LANGUAGES CXX)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

enable_testing()
find_package(fmt REQUIRED)   # provided by CMakeDeps generator

add_executable(hello src/main.cpp)

target_link_libraries(hello PRIVATE fmt::fmt)

# Only add tests dir if it exists (avoids CMake errors if you later remove tests)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
  add_subdirectory(tests)
endif()