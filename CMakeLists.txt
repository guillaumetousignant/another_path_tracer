cmake_minimum_required(VERSION 3.14)
# Need 3.14 for FetchContent_MakeAvailable

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "Do not build in-source. Please remove CMakeCache.txt and the CMakeFiles/ directory. Then build out-of-source.")
endif()

project(another_path_tracer VERSION 1.2.0 LANGUAGES C CXX)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

add_subdirectory(src)

option(BUILD_DOC "Build documentation." OFF)
if (BUILD_DOC)
    add_subdirectory(docs)
endif()

option(BUILD_TESTING "Build all tests." OFF)
if (BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
