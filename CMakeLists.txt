cmake_minimum_required(VERSION 3.13)

project(another_path_tracer VERSION 1.1.0 LANGUAGES C CXX)

add_library(another_path_tracer
    src/acceleration/AccelerationGrid_t.cpp
    src/acceleration/AccelerationGridArray_t
    src/acceleration/AccelerationGridVector_t
    src/acceleration/AccelerationMultigrid_t
    src/acceleration/AccelerationMultigridArray_t
    src/acceleration/AccelerationMultigridVector_t
    src/acceleration/GridCell_t.cpp
    src/acceleration/GridCellArray_t.cpp
    src/acceleration/GridCellVector_t.cpp
)