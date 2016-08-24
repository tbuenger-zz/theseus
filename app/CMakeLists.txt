set(SOURCES
  src/main.cpp
)

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/src)

set(APPLICATION_NAME TheseusRunner)
add_executable(${APPLICATION_NAME} ${SOURCES})
set_target_properties(${APPLICATION_NAME} PROPERTIES
    OUTPUT_NAME theseus)

target_link_libraries(${APPLICATION_NAME} core tptp)