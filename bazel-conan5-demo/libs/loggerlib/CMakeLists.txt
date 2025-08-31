cmake_minimum_required(VERSION 3.20)
project(loggerlib LANGUAGES CXX)

add_library(loggerlib src/logger.cpp)
target_include_directories(loggerlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(loggerlib PUBLIC cxx_std_17)

include(GNUInstallDirs)

# Install the library and headers into standard dirs
install(TARGETS loggerlib
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
