cmake_minimum_required(VERSION 3.15)
project(task4)

set(CMAKE_CXX_STANDARD 17)

include_directories(lib)
add_subdirectory(lib)

set_target_properties(lib PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(tests)
