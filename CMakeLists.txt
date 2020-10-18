cmake_minimum_required(VERSION 3.18.0)
project(MARKOVCPP)

set(MARKOV_SRC markov.cpp)

add_executable(markov.exe ${MARKOV_SRC})
