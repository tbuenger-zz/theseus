project("tptp")

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}/src
)

enable_testing()
add_subdirectory(src)
add_subdirectory(test)