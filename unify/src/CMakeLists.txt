ADD_LIBRARY(unify STATIC unify.cpp)
target_link_libraries(unify core)
target_include_directories(unify PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)