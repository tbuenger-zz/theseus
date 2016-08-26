add_executable(unifyTest testgtest.cpp)
target_link_libraries(unifyTest gtest gtest_main tptp unify)
add_test(unifyTest unifyTest COMMAND "bin/unifyTest")
include(CTest)
