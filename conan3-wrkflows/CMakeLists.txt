cmake_minimum_required(VERSION 3.15)
project(hello CXX)

add_library(hello src/hello.cpp)
target_include_directories(hello PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Optional fmt usage
find_package(fmt QUIET)
if (fmt_FOUND)
  target_link_libraries(hello PUBLIC fmt::fmt)
endif()

install(TARGETS hello)
install(DIRECTORY include/ DESTINATION include)

