cmake_minimum_required(VERSION 3.18)
project(greetlib LANGUAGES CXX)

option(UPPERCASE "Uppercase greeting" OFF)
option(BUILD_SHARED_LIBS "Build shared libs" OFF)

add_library(greet src/greet.cpp)  # STATIC/SHARED by BUILD_SHARED_LIBS
target_include_directories(greet PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(greet PUBLIC cxx_std_17)
target_compile_definitions(greet PRIVATE $<$<BOOL:${UPPERCASE}>:GREET_UPPERCASE>)

install(TARGETS greet
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include)


