cmake_minimum_required(VERSION 3.15)
project(lib CXX)

set(CMAKE_CXX_STANDARD 17)

set(HEADER_FILES avl.h)

add_library(lib SHARED ${HEADER_FILES})
