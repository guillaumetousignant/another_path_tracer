cmake_minimum_required(VERSION 3.12)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "Do not build in-source. Please remove CMakeCache.txt and the CMakeFiles/ directory. Then build out-of-source.")
endif()

project(another_path_tracer VERSION 1.1.0 LANGUAGES C CXX)

add_subdirectory(src)

option(BUILD_TESTING "Build all tests." OFF)
if (BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
