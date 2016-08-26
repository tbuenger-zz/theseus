add_executable(tptpTest testgtest.cpp)
target_link_libraries(tptpTest gtest gtest_main tptp)
add_test(tptpTest tptpTest COMMAND "bin/tptpTest")
include(CTest)

configure_file(input/BOO109+1.p input/BOO109+1.p COPYONLY)
configure_file(input/COM003+1.p input/COM003+1.p COPYONLY)
configure_file(input/CSR031+1.p input/CSR031+1.p COPYONLY)