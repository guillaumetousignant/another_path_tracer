cmake_minimum_required(VERSION 3.13)

project(another_path_tracer VERSION 1.1.0 LANGUAGES C CXX)

add_library(libanother_path_tracer
    src/acceleration/AccelerationGrid_t.cpp
    src/acceleration/AccelerationGridArray_t
    src/acceleration/AccelerationGridVector_t
    src/acceleration/AccelerationMultigrid_t
    src/acceleration/AccelerationMultigridArray_t
    src/acceleration/AccelerationMultigridVector_t
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
    src/cameras/Cam3DMotionblurAperture_t.cpp
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
)

target_include_directories(libanother_path_tracer PUBLIC include)

add_executable(another_path_tracer src/main.cpp)

target_link_libraries(another_path_tracer PRIVATE libanother_path_tracer)