add_library(APTracer "")
add_subdirectory(acceleration)
add_subdirectory(cameras)
add_subdirectory(entities)
add_subdirectory(functions)
add_subdirectory(materials)
add_subdirectory(shapes)
add_subdirectory(skyboxes)

target_include_directories(APTracer PUBLIC 
    $<INSTALL_INTERFACE:include/another_path_tracer/>    
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/>)

option(USE_PNG "Use the PNG library to be able to read/write PNG images." ON)
if (USE_PNG)
    find_package(ZLIB REQUIRED)
    find_package(PNG REQUIRED)
    target_link_libraries(APTracer PUBLIC 
        PNG::PNG)
    target_compile_definitions(APTracer PUBLIC APTRACER_USE_PNG)
    set(ZLIB_DEPENDENCY "find_dependency(ZLIB REQUIRED)")
    set(PNG_DEPENDENCY "find_dependency(PNG REQUIRED)")
endif()

option(USE_JPEG "Use the jpeg library to be able to read/write jpeg images." ON)
if (USE_JPEG)
    find_package(JPEG REQUIRED)
    target_link_libraries(APTracer PUBLIC 
        JPEG::JPEG)
    target_compile_definitions(APTracer PUBLIC APTRACER_USE_JPEG)
    set(JPEG_DEPENDENCY "find_dependency(JPEG REQUIRED)")
endif()

option(USE_TIFF "Use the tiff library to be able to read/write tiff images." ON)
if (USE_TIFF)
    find_package(TIFF REQUIRED)
    target_link_libraries(APTracer PUBLIC 
        TIFF::TIFF)
    target_compile_definitions(APTracer PUBLIC APTRACER_USE_TIFF)
    set(TIFF_DEPENDENCY "find_dependency(TIFF REQUIRED)")
endif()

option(USE_OPENGL "Use OpenGL to display images." ON)
if (USE_OPENGL)
    list(INSERT CMAKE_FRAMEWORK_PATH 0 /System/Library/Frameworks) # Because CMake doesn't find the correct GLUT on macOS 
    find_package(GLUT REQUIRED)
    find_package(OpenGL REQUIRED COMPONENTS OpenGL) 

    add_dependencies(APTracer OpenGL::GL) # Should use OpenGL::OpenGL on newer linux hosts, but doesn't work on windows and android

    target_link_libraries(APTracer PUBLIC 
        OpenGL::GL
        GLUT::GLUT) # Should use OpenGL::OpenGL on newer linux hosts, but doesn't work on windows and android
    target_compile_definitions(APTracer PUBLIC APTRACER_USE_OPENGL)
    set(GLUT_DEPENDENCY "find_dependency(GLUT REQUIRED)")
    set(OPENGL_DEPENDENCY "find_dependency(OpenGL REQUIRED COMPONENTS OpenGL)")
endif()

option(USE_OPENMP "Parallelise using OpenMP." ON)
if (USE_OPENMP)
    find_package(OpenMP REQUIRED)
    target_link_libraries(APTracer PUBLIC OpenMP::OpenMP_CXX)
    target_compile_definitions(APTracer PUBLIC APTRACER_USE_OPENMP)
    set(OPENMP_DEPENDENCY "find_dependency(OpenMP REQUIRED)")
endif()

option(OPTIMIZE_FOR_NATIVE "Build with -march=native." OFF)
if (OPTIMIZE_FOR_NATIVE)
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
    if(COMPILER_SUPPORTS_MARCH_NATIVE)
        target_compile_options(APTracer PUBLIC -march=native)
    endif()
endif()

if(NOT WIN32)
    target_link_libraries(APTracer PUBLIC debug pthread)
endif()

target_compile_features(APTracer PRIVATE cxx_std_14)
if(NOT ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug"))
    set_property(TARGET APTracer PROPERTY INTERPROCEDURAL_OPTIMIZATION True)
endif()

install(TARGETS APTracer 
    EXPORT aptracer-targets
    DESTINATION lib)

if(NOT CMAKE_SOURCE_DIR STREQUAL CMAKE_INSTALL_PREFIX)
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ 
        DESTINATION include/another_path_tracer
        FILES_MATCHING PATTERN "*.h")
endif()

if(WIN32)
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/
        DESTINATION bin
        FILES_MATCHING PATTERN "*.dll")
endif()

install(EXPORT aptracer-targets
  FILE
    APTracerTargets.cmake
  NAMESPACE
    APTracer::
  DESTINATION
    lib/cmake/APTracer
)

#Create a ConfigVersion.cmake file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/APTracerConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${PROJECT_SOURCE_DIR}/cmake/APTracerConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/APTracerConfig.cmake
    INSTALL_DESTINATION lib/cmake/APTracer
)

#Install the config, configversion and custom find modules
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/APTracerConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/APTracerConfigVersion.cmake
    DESTINATION lib/cmake/APTracer
)

export(EXPORT aptracer-targets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/APTracerTargets.cmake
    NAMESPACE APTracer::)

#Register package in user's package registry
export(PACKAGE APTracer)

add_executable(another_path_tracer main.cpp)

target_link_libraries(another_path_tracer PRIVATE APTracer)
target_compile_features(another_path_tracer PRIVATE cxx_std_14)
if(NOT ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug"))
    set_property(TARGET another_path_tracer PROPERTY INTERPROCEDURAL_OPTIMIZATION True)
endif()
if (OPTIMIZE_FOR_NATIVE)
    if(COMPILER_SUPPORTS_MARCH_NATIVE)
        target_compile_options(another_path_tracer PUBLIC -march=native)
    endif()
endif()

install(TARGETS another_path_tracer DESTINATION bin)
