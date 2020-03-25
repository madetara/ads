configure_file(CMakeLists.txt.benchmark google-benchmark-download/CMakeLists.txt)

execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/google-benchmark-download)
if (result)
    message(FATAL_ERROR "CMake step for google benchmark failed: ${result}")
endif ()

execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/google-benchmark-download)

add_subdirectory(
        ${CMAKE_CURRENT_BINARY_DIR}/google-benchmark-src
        ${CMAKE_CURRENT_BINARY_DIR}/google-benchmark-build
        EXCLUDE_FROM_ALL)

add_executable(benchmarks p_queue_benchmark.cpp)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data.txt
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(benchmarks benchmark::benchmark)
target_link_libraries(benchmarks lib)