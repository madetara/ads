cmake_minimum_required(VERSION 3.15)
project(asd2)

set(CMAKE_CXX_STANDARD 17)

add_executable(asd2 main.cpp LinkedList.h Node.h elem.h elem.cpp utils.h)