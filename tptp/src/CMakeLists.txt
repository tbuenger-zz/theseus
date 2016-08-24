FIND_PACKAGE(BISON 3 REQUIRED)
FIND_PACKAGE(FLEX REQUIRED)

BISON_TARGET(TPTPParser parser.ypp ${CMAKE_CURRENT_BINARY_DIR}/parser.cxx DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.hxx)

FLEX_TARGET(TPTPScanner scanner.lpp ${CMAKE_CURRENT_BINARY_DIR}/scanner.cxx)

ADD_FLEX_BISON_DEPENDENCY(TPTPScanner TPTPParser)

ADD_LIBRARY(tptp STATIC problem.cpp ${BISON_TPTPParser_OUTPUTS}  ${FLEX_TPTPScanner_OUTPUTS})
target_compile_options(tptp PRIVATE -Wno-deprecated-register)
target_link_libraries(tptp core)
target_include_directories(tptp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)