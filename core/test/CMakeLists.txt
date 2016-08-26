add_executable(coreTest testgtest.cpp)
target_link_libraries(coreTest gtest gtest_main core)
add_test(coreTest coreTest COMMAND "bin/coreTest")
include(CTest)