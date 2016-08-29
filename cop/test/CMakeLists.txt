add_executable(copTest testgtest.cpp)
target_link_libraries(copTest gtest gtest_main core cop unify tptp)
add_test(copTest copTest COMMAND "bin/copTest")
include(CTest)
