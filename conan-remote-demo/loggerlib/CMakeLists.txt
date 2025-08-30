cmake_minimum_required(VERSION 3.20)
project(loggerlib LANGUAGES CXX)

add_library(loggerlib src/loggerlib.cpp)
target_include_directories(loggerlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(loggerlib PUBLIC cxx_std_17)
set_target_properties(loggerlib PROPERTIES EXPORT_NAME loggerlib)

# >>> ADD THESE INSTALL RULES <<<
install(TARGETS loggerlib
  EXPORT loggerlibTargets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)
install(DIRECTORY include/ DESTINATION include)

