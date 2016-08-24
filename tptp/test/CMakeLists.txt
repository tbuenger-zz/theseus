add_executable(runUnitTests testgtest.cpp)
target_link_libraries(runUnitTests gtest gtest_main tptp)
add_test(runUnitTests runUnitTests COMMAND "bin/runUnitTests")
include(CTest)