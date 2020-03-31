cmake_minimum_required(VERSION 3.12)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "Do not build in-source. Please remove CMakeCache.txt and the CMakeFiles/ directory. Then build out-of-source.")
endif()

project(another_path_tracer VERSION 1.1.0 LANGUAGES C CXX)

add_library(APTracer
    src/acceleration/AccelerationGrid_t.cpp
    src/acceleration/AccelerationGridArray_t.cpp
    src/acceleration/AccelerationGridVector_t.cpp
    src/acceleration/AccelerationMultiGrid_t.cpp
    src/acceleration/AccelerationMultiGridArray_t.cpp
    src/acceleration/AccelerationMultiGridVector_t.cpp
    src/acceleration/GridCell_t.cpp
    src/acceleration/GridCellArray_t.cpp
    src/acceleration/GridCellVector_t.cpp

    src/cameras/Cam_t.cpp
    src/cameras/Cam3D_t.cpp
    src/cameras/Cam3DAperture_t.cpp
    src/cameras/Cam3DMotionblur_t.cpp
    src/cameras/Cam3DMotionblurAperture_t.cpp
    src/cameras/CamAperture_t.cpp
    src/cameras/CamMotionblur_t.cpp
    src/cameras/CamMotionblurAperture_t.cpp
    src/cameras/IsoCam_t.cpp
    src/cameras/IsoCamAperture_t.cpp
    src/cameras/IsoCamMotionblur_t.cpp
    src/cameras/IsoCamMotionblurAperture_t.cpp
    src/cameras/RecCam_t.cpp
    src/cameras/RecCamAperture_t.cpp
    src/cameras/RecCamMotionblur_t.cpp
    src/cameras/RecCamMotionblurAperture_t.cpp

    src/entities/Camera_t.cpp
    src/entities/DirectionalLight_t.cpp
    src/entities/ImgBuffer_t.cpp
    src/entities/ImgBufferOpenGL_t.cpp
    src/entities/MaterialMap_t.cpp
    src/entities/MeshGeometry_t.cpp
    src/entities/OpenGLRenderer_t.cpp
    src/entities/RandomGenerator_t.cpp
    src/entities/Ray_t.cpp
    src/entities/Scene_t.cpp
    src/entities/SceneContext_t.cpp
    src/entities/Texture_t.cpp
    src/entities/TransformMatrix_t.cpp
    src/entities/Vec3f.cpp

    src/functions/Colours.cpp
    src/functions/NextFilename.cpp
    src/functions/Slerp.cpp
    src/functions/tinyxml2.cpp

    src/materials/Absorber_t.cpp
    src/materials/BounceMaterial_t.cpp
    src/materials/Diffuse_t.cpp
    src/materials/DiffuseFull_t.cpp
    src/materials/DiffuseNormal_t.cpp
    src/materials/DiffuseTex_t.cpp
    src/materials/DiffuseTexNormal_t.cpp
    src/materials/DistanceMaterial_t.cpp
    src/materials/FresnelMix_t.cpp
    src/materials/FresnelMixIn_t.cpp
    src/materials/FresnelMixNormal_t.cpp
    src/materials/NonAbsorber_t.cpp
    src/materials/NormalMaterial_t.cpp
    src/materials/Portal_t.cpp
    src/materials/PortalScatterer_t.cpp
    src/materials/PortalScattererTop_t.cpp
    src/materials/PortalTop_t.cpp
    src/materials/RandomMix_t.cpp
    src/materials/RandomMixIn_t.cpp
    src/materials/Reflective_t.cpp
    src/materials/ReflectiveFuzz_t.cpp
    src/materials/ReflectiveFuzzNormal_t.cpp
    src/materials/ReflectiveFuzzTex_t.cpp
    src/materials/ReflectiveFuzzTexNormal_t.cpp
    src/materials/ReflectiveNormal_t.cpp
    src/materials/ReflectiveRefractive_t.cpp
    src/materials/ReflectiveRefractiveFuzz_t.cpp
    src/materials/ReflectiveRefractiveNormal_t.cpp
    src/materials/Refractive_t.cpp
    src/materials/RefractiveFuzz_t.cpp
    src/materials/Scatterer_t.cpp
    src/materials/ScattererExp_t.cpp
    src/materials/ScattererExpFull_t.cpp
    src/materials/ScattererFull_t.cpp
    src/materials/Toon_t.cpp
    src/materials/Transparent_t.cpp

    src/shapes/Box_t.cpp
    src/shapes/Mesh_t.cpp
    src/shapes/MeshMotionblur_t.cpp
    src/shapes/MeshTop_t.cpp
    src/shapes/Sphere_t.cpp
    src/shapes/SphereMotionblur_t.cpp
    src/shapes/Triangle_t.cpp
    src/shapes/TriangleMesh_t.cpp
    src/shapes/TriangleMeshMotionblur_t.cpp
    src/shapes/TriangleMotionblur_t.cpp

    src/skyboxes/SkyboxFlat_t.cpp
    src/skyboxes/SkyboxFlatSun_t.cpp
    src/skyboxes/SkyboxTexture_t.cpp
    src/skyboxes/SkyboxTextureSun_t.cpp
    src/skyboxes/SkyboxTextureTransformation_t.cpp
    src/skyboxes/SkyboxTextureTransformationSun_t.cpp

    include/another_path_tracer.h

    include/acceleration/acceleration.h
    include/acceleration/AccelerationGrid_t.h
    include/acceleration/AccelerationGridArray_t.h
    include/acceleration/AccelerationGridVector_t.h
    include/acceleration/AccelerationMultiGrid_t.h
    include/acceleration/AccelerationMultiGridArray_t.h
    include/acceleration/AccelerationMultiGridVector_t.h
    include/acceleration/GridCell_t.h
    include/acceleration/GridCellArray_t.h
    include/acceleration/GridCellVector_t.h

    include/cameras/cameras.h
    include/cameras/Cam_t.h
    include/cameras/Cam3D_t.h
    include/cameras/Cam3DAperture_t.h
    include/cameras/Cam3DMotionblur_t.h
    include/cameras/Cam3DMotionblurAperture_t.h
    include/cameras/CamAperture_t.h
    include/cameras/CamMotionblur_t.h
    include/cameras/CamMotionblurAperture_t.h
    include/cameras/IsoCam_t.h
    include/cameras/IsoCamAperture_t.h
    include/cameras/IsoCamMotionblur_t.h
    include/cameras/IsoCamMotionblurAperture_t.h
    include/cameras/RecCam_t.h
    include/cameras/RecCamAperture_t.h
    include/cameras/RecCamMotionblur_t.h
    include/cameras/RecCamMotionblurAperture_t.h

    include/entities/entities.h
    include/entities/AccelerationStructure_t.h
    include/entities/Camera_t.h
    include/entities/DirectionalLight_t.h
    include/entities/ImgBuffer_t.h
    include/entities/ImgBufferOpenGL_t.h
    include/entities/Material_t.h
    include/entities/MaterialMap_t.h
    include/entities/MaterialMix_t.h
    include/entities/Medium_t.h
    include/entities/MeshGeometry_t.h
    include/entities/OpenGLRenderer_t.h
    include/entities/RandomGenerator_t.h
    include/entities/Ray_t.h
    include/entities/ScatteringFunction_t.h
    include/entities/Scene_t.h
    include/entities/SceneContext_t.h
    include/entities/Shape_t.h
    include/entities/Skybox_t.h
    include/entities/Texture_t.h
    include/entities/TransformMatrix_t.h
    include/entities/Vec3f.h

    include/functions/functions.h
    include/functions/CImg.h
    include/functions/Colours.h
    include/functions/NextFilename.h
    include/functions/Slerp.h
    include/functions/tinyxml2.h

    include/materials/materials.h
    include/materials/Absorber_t.h
    include/materials/BounceMaterial_t.h
    include/materials/Diffuse_t.h
    include/materials/DiffuseFull_t.h
    include/materials/DiffuseNormal_t.h
    include/materials/DiffuseTex_t.h
    include/materials/DiffuseTexNormal_t.h
    include/materials/DistanceMaterial_t.h
    include/materials/FresnelMix_t.h
    include/materials/FresnelMixIn_t.h
    include/materials/FresnelMixNormal_t.h
    include/materials/NonAbsorber_t.h
    include/materials/NormalMaterial_t.h
    include/materials/Portal_t.h
    include/materials/PortalScatterer_t.h
    include/materials/PortalScattererTop_t.h
    include/materials/PortalTop_t.h
    include/materials/RandomMix_t.h
    include/materials/RandomMixIn_t.h
    include/materials/Reflective_t.h
    include/materials/ReflectiveFuzz_t.h
    include/materials/ReflectiveFuzzNormal_t.h
    include/materials/ReflectiveFuzzTex_t.h
    include/materials/ReflectiveFuzzTexNormal_t.h
    include/materials/ReflectiveNormal_t.h
    include/materials/ReflectiveRefractive_t.h
    include/materials/ReflectiveRefractiveFuzz_t.h
    include/materials/ReflectiveRefractiveNormal_t.h
    include/materials/Refractive_t.h
    include/materials/RefractiveFuzz_t.h
    include/materials/Scatterer_t.h
    include/materials/ScattererExp_t.h
    include/materials/ScattererExpFull_t.h
    include/materials/ScattererFull_t.h
    include/materials/Toon_t.h
    include/materials/Transparent_t.h

    include/shapes/shapes.h
    include/shapes/Box_t.h
    include/shapes/Mesh_t.h
    include/shapes/MeshMotionblur_t.h
    include/shapes/MeshTop_t.h
    include/shapes/Sphere_t.h
    include/shapes/SphereMotionblur_t.h
    include/shapes/Triangle_t.h
    include/shapes/TriangleMesh_t.h
    include/shapes/TriangleMeshMotionblur_t.h
    include/shapes/TriangleMotionblur_t.h

    include/skyboxes/skyboxes.h
    include/skyboxes/SkyboxFlat_t.h
    include/skyboxes/SkyboxFlatSun_t.h
    include/skyboxes/SkyboxTexture_t.h
    include/skyboxes/SkyboxTextureSun_t.h
    include/skyboxes/SkyboxTextureTransformation_t.h
    include/skyboxes/SkyboxTextureTransformationSun_t.h
)

target_include_directories(APTracer PUBLIC 
    $<INSTALL_INTERFACE:include>    
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

if(WIN32)
    find_library(PNG REQUIRED
        NAMES png libpng libpng_a libpng_debug libpng_a_debug
        HINTS external_windows/lib) # CHECK make image libraries optional
    find_library(JPEG REQUIRED
        NAMES jpeg libjpeg jpeg_a libjpeg_a
        HINTS external_windows/lib) # CHECK make image libraries optional
    find_library(TIFF REQUIRED
        NAMES tiff libtiff
        HINTS external_windows/lib) # CHECK make image libraries optional
    find_library(GLUT REQUIRED
        NAMES freeglut libfreeglut glut libglut
        HINTS external_windows/lib) # CHECK make image libraries optional

    target_include_directories(APTracer PUBLIC 
        $<INSTALL_INTERFACE:external_windows/include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external_windows/include>)

    SET(PNG_LIBRARY ${PNG})
    SET(JPEG_LIBRARY ${JPEG})
    SET(TIFF_LIBRARY ${TIFF})
    SET(GLUT_LIBRARY ${GLUT})
else()
    find_package(PNG REQUIRED) # CHECK make image libraries optional
    find_package(JPEG REQUIRED)
    find_package(TIFF REQUIRED)
    find_package(GLUT REQUIRED)

    target_include_directories(APTracer PUBLIC 
        ${PNG_INCLUDE_DIR}
        ${JPEG_INCLUDE_DIR}
        ${TIFF_INCLUDE_DIR}
        ${GLUT_INCLUDE_DIRS})
endif()

find_package(OpenMP)
find_package(OpenGL REQUIRED COMPONENTS OpenGL) # CHECK make optional

add_dependencies(APTracer OpenGL::GL) # Should use OpenGL::OpenGL on newer linux hosts, but doesn't work on windows and android

target_link_libraries(APTracer PUBLIC 
    ${PNG_LIBRARY}
    ${JPEG_LIBRARY}
    ${TIFF_LIBRARY}
    OpenGL::GL
    ${GLUT_LIBRARY}) # Should use OpenGL::OpenGL on newer linux hosts, but doesn't work on windows and android

if(OpenMP_CXX_FOUND)
    target_link_libraries(APTracer PUBLIC optimized OpenMP::OpenMP_CXX)
endif()

target_link_libraries(APTracer PUBLIC debug pthread)
target_compile_features(APTracer PRIVATE cxx_std_11)

add_executable(another_path_tracer src/main.cpp)

target_link_libraries(another_path_tracer PRIVATE APTracer)
target_compile_features(another_path_tracer PRIVATE cxx_std_11)

option(BUILD_TESTS "Build all tests." OFF)

if (BUILD_TESTS)
    enable_testing()
    add_executable(unit_tests 
        tests/main_test.cpp
        tests/catch.hpp)
    target_link_libraries(unit_tests PRIVATE APTracer)
    target_compile_features(unit_tests PRIVATE cxx_std_11)

    add_test(unit_tests unit_tests)
endif()

install(TARGETS APTracer 
    EXPORT aptracer-export
    DESTINATION lib)

if(NOT CMAKE_SOURCE_DIR STREQUAL CMAKE_INSTALL_PREFIX)
    install(DIRECTORY include/ 
        DESTINATION include
        FILES_MATCHING PATTERN "*.h")
    
    if(WIN32)
        install(DIRECTORY external_windows/ 
            DESTINATION external_windows
            FILES_MATCHING 
                PATTERN "*.h"
                PATTERN "*.hxx")
    endif()
endif()

if(WIN32)
    install(DIRECTORY external_windows/bin
        DESTINATION .
        FILES_MATCHING 
            PATTERN "freeglut.dll"
            PATTERN "libpng.dll"
            PATTERN "tiff.dll")
endif()

install(EXPORT aptracer-export
  FILE
    APTracerTargets.cmake
  NAMESPACE
    APTracer::
  DESTINATION
    lib/cmake/APTracer
)

install(TARGETS another_path_tracer DESTINATION bin)