cmake_minimum_required(VERSION 3.15)
project(lib)

set(CMAKE_CXX_STANDARD 17)

add_library(lib p_queue.h p_queue_arr.h)
