cmake_minimum_required(VERSION 3.18.0)
project(MARKOVCPP)

set(MARKOV_SRC markov.cpp)

add_library(tinyxml2 include/tinyxml2.cpp include/tinyxml2.h)

add_executable(markov.exe ${MARKOV_SRC})
add_dependencies(markov.exe tinyxml2)
target_link_libraries(markov.exe tinyxml2)
