add_executable(smoketest smoketest.cpp)
target_link_libraries(smoketest PRIVATE fmt::fmt)
add_test(NAME smoketest COMMAND smoketest)
