ADD_LIBRARY(cop STATIC cop.cpp normalizer.cpp)
target_link_libraries(cop core unify tptp)
target_include_directories(cop PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)