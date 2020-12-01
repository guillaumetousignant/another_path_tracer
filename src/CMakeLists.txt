add_library(APTracer
    acceleration/AccelerationGrid_t.cpp
    acceleration/AccelerationGridArray_t.cpp
    acceleration/AccelerationGridVector_t.cpp
    acceleration/AccelerationMultiGrid_t.cpp
    acceleration/AccelerationMultiGridArray_t.cpp
    acceleration/AccelerationMultiGridVector_t.cpp
    acceleration/GridCell_t.cpp
    acceleration/GridCellArray_t.cpp
    acceleration/GridCellVector_t.cpp

    cameras/Cam_t.cpp
    cameras/Cam3D_t.cpp
    cameras/Cam3DAperture_t.cpp
    cameras/Cam3DMotionblur_t.cpp
    cameras/Cam3DMotionblurAperture_t.cpp
    cameras/CamAperture_t.cpp
    cameras/CamMotionblur_t.cpp
    cameras/CamMotionblurAperture_t.cpp
    cameras/IsoCam_t.cpp
    cameras/IsoCamAperture_t.cpp
    cameras/IsoCamMotionblur_t.cpp
    cameras/IsoCamMotionblurAperture_t.cpp
    cameras/RecCam_t.cpp
    cameras/RecCamAperture_t.cpp
    cameras/RecCamMotionblur_t.cpp
    cameras/RecCamMotionblurAperture_t.cpp

    entities/Camera_t.cpp
    entities/DirectionalLight_t.cpp
    entities/ImgBuffer_t.cpp
    entities/ImgBufferOpenGL_t.cpp
    entities/MaterialMap_t.cpp
    entities/MeshGeometry_t.cpp
    entities/OpenGLRenderer_t.cpp
    entities/RandomGenerator_t.cpp
    entities/Ray_t.cpp
    entities/Scene_t.cpp
    entities/SceneContext_t.cpp
    entities/Texture_t.cpp
    entities/TransformMatrix_t.cpp
    entities/Vec3f.cpp

    functions/Colours.cpp
    functions/NextFilename.cpp
    functions/Slerp.cpp
    functions/tinyxml2.cpp

    materials/Absorber_t.cpp
    materials/BounceMaterial_t.cpp
    materials/Diffuse_t.cpp
    materials/DiffuseFull_t.cpp
    materials/DiffuseNormal_t.cpp
    materials/DiffuseTex_t.cpp
    materials/DiffuseTexNormal_t.cpp
    materials/DistanceMaterial_t.cpp
    materials/FresnelMix_t.cpp
    materials/FresnelMixIn_t.cpp
    materials/FresnelMixNormal_t.cpp
    materials/NonAbsorber_t.cpp
    materials/NormalMaterial_t.cpp
    materials/NormalDiffuseMaterial_t.cpp
    materials/Portal_t.cpp
    materials/PortalScatterer_t.cpp
    materials/PortalScattererTop_t.cpp
    materials/PortalTop_t.cpp
    materials/RandomMix_t.cpp
    materials/RandomMixIn_t.cpp
    materials/Reflective_t.cpp
    materials/ReflectiveFuzz_t.cpp
    materials/ReflectiveFuzzNormal_t.cpp
    materials/ReflectiveFuzzTex_t.cpp
    materials/ReflectiveFuzzTexNormal_t.cpp
    materials/ReflectiveNormal_t.cpp
    materials/ReflectiveRefractive_t.cpp
    materials/ReflectiveRefractiveFuzz_t.cpp
    materials/ReflectiveRefractiveNormal_t.cpp
    materials/Refractive_t.cpp
    materials/RefractiveFuzz_t.cpp
    materials/Scatterer_t.cpp
    materials/ScattererExp_t.cpp
    materials/ScattererExpFull_t.cpp
    materials/ScattererFull_t.cpp
    materials/Toon_t.cpp
    materials/Transparent_t.cpp

    shapes/Box_t.cpp
    shapes/Mesh_t.cpp
    shapes/MeshMotionblur_t.cpp
    shapes/MeshTop_t.cpp
    shapes/Sphere_t.cpp
    shapes/SphereMotionblur_t.cpp
    shapes/Triangle_t.cpp
    shapes/TriangleMesh_t.cpp
    shapes/TriangleMeshMotionblur_t.cpp
    shapes/TriangleMotionblur_t.cpp

    skyboxes/SkyboxFlat_t.cpp
    skyboxes/SkyboxFlatSun_t.cpp
    skyboxes/SkyboxTexture_t.cpp
    skyboxes/SkyboxTextureSun_t.cpp
    skyboxes/SkyboxTextureTransformation_t.cpp
    skyboxes/SkyboxTextureTransformationSun_t.cpp
)

find_package(ZLIB REQUIRED)
find_package(PNG REQUIRED) # CHECK make image libraries optional
find_package(JPEG REQUIRED)
find_package(TIFF REQUIRED)
find_package(GLUT REQUIRED)

target_include_directories(APTracer PUBLIC 
    $<INSTALL_INTERFACE:include/another_path_tracer/>    
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/>
    ${ZLIB_INCLUDE_DIR}
    ${PNG_INCLUDE_DIR}
    ${JPEG_INCLUDE_DIR}
    ${TIFF_INCLUDE_DIR}
    ${GLUT_INCLUDE_DIRS})

find_package(OpenMP)
find_package(OpenGL REQUIRED COMPONENTS OpenGL) # CHECK make optional
add_dependencies(APTracer OpenGL::GL) # Should use OpenGL::OpenGL on newer linux hosts, but doesn't work on windows and android

target_link_libraries(APTracer PUBLIC 
    ${ZLIB_LIBRARY}
    ${PNG_LIBRARY}
    ${JPEG_LIBRARY}
    ${TIFF_LIBRARY}
    OpenGL::GL
    ${GLUT_LIBRARY}) # Should use OpenGL::OpenGL on newer linux hosts, but doesn't work on windows and android

if(OpenMP_CXX_FOUND)
    target_link_libraries(APTracer PUBLIC optimized OpenMP::OpenMP_CXX)
endif()

if(NOT WIN32)
    target_link_libraries(APTracer PUBLIC debug pthread)
endif()

target_compile_features(APTracer PRIVATE cxx_std_11)
if(NOT ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug"))
    set_property(TARGET APTracer PROPERTY INTERPROCEDURAL_OPTIMIZATION True)
endif()

add_executable(another_path_tracer main.cpp)

target_link_libraries(another_path_tracer PRIVATE APTracer)
target_compile_features(another_path_tracer PRIVATE cxx_std_11)
if(NOT ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug"))
    set_property(TARGET another_path_tracer PROPERTY INTERPROCEDURAL_OPTIMIZATION True)
endif()

install(TARGETS APTracer 
    EXPORT aptracer-export
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

install(EXPORT aptracer-export
  FILE
    APTracerConfig.cmake
  NAMESPACE
    APTracer::
  DESTINATION
    lib/cmake/APTracer
)

install(TARGETS another_path_tracer DESTINATION bin)
