project("core")


set(SOURCES
    src/formula.cpp
    src/identifier.cpp
    src/literal.cpp
    src/term.cpp
    src/printer.cpp
)

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/src)

set(LIBRARY_NAME core)

add_library(${LIBRARY_NAME} STATIC ${SOURCES})
target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

enable_testing()
add_subdirectory(test)
