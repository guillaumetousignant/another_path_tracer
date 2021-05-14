.. another_path_tracer documentation master file, created by
   sphinx-quickstart on Tue Jan 19 12:08:19 2021.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to another_path_tracer's documentation!
===============================================

.. toctree::
   :maxdepth: 2
   :caption: Contents:



Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`

Docs
====

.. Entities

.. doxygenclass:: APTracer::Entities::AccelerationStructure_t
   :members:

.. doxygenclass:: APTracer::Entities::Camera_t
   :members:

.. doxygenclass:: APTracer::Entities::DirectionalLight_t
   :members:

.. doxygenclass:: APTracer::Entities::ImgBuffer_t
   :members:

.. doxygenclass:: APTracer::Entities::ImgBufferOpenGL_t
   :members:

.. doxygenclass:: APTracer::Entities::Material_t
   :members:

.. doxygenclass:: APTracer::Entities::MaterialMap_t
   :members:

.. doxygenclass:: APTracer::Entities::Medium_t
   :members:

.. doxygenclass:: APTracer::Entities::MeshGeometry_t
   :members:

.. doxygenclass:: APTracer::Entities::OpenGLRenderer_t
   :members:
   
.. doxygenclass:: APTracer::Entities::Ray_t
   :members:

.. doxygenclass:: APTracer::Entities::Scene_t
   :members:

.. doxygenclass:: APTracer::Entities::SceneContext_t
   :members:

.. doxygenclass:: APTracer::Entities::Shape_t
   :members:

.. doxygenclass:: APTracer::Entities::Skybox_t
   :members:

.. doxygenclass:: APTracer::Entities::Texture_t
   :members:

.. doxygenclass:: APTracer::Entities::TransformMatrix_t
   :members:

.. doxygenclass:: APTracer::Entities::Vec3f
   :members:

.. doxygenfunction:: APTracer::Entities::build_rng_vec

.. doxygenfunction:: APTracer::Entities::rng

.. doxygenvariable:: APTracer::Entities::rng_vec

.. Acceleration

.. doxygenclass:: APTracer::Acceleration::AccelerationGrid_t
   :members:

.. doxygenclass:: APTracer::Acceleration::AccelerationGridArray_t
   :members:

.. doxygenclass:: APTracer::Acceleration::AccelerationGridVector_t
   :members:

.. doxygenclass:: APTracer::Acceleration::AccelerationMultiGrid_t
   :members:

.. doxygenclass:: APTracer::Acceleration::AccelerationMultiGridArray_t
   :members:

.. doxygenclass:: APTracer::Acceleration::AccelerationMultiGridVector_t
   :members:

.. doxygenclass:: APTracer::Acceleration::GridCell_t
   :members:

.. doxygenclass:: APTracer::Acceleration::GridCellArray_t
   :members:

.. doxygenclass:: APTracer::Acceleration::GridCellVector_t
   :members:
   
.. Cameras

.. doxygenclass:: APTracer::Cameras::Cam_t
   :members:

.. doxygenclass:: APTracer::Cameras::Cam3D_t
   :members:

.. doxygenclass:: APTracer::Cameras::Cam3DAperture_t
   :members:

.. doxygenclass:: APTracer::Cameras::Cam3DMotionblur_t
   :members:

.. doxygenclass:: APTracer::Cameras::Cam3DMotionblurAperture_t
   :members:

.. doxygenclass:: APTracer::Cameras::CamAperture_t
   :members:

.. doxygenclass:: APTracer::Cameras::CamMotionblur_t
   :members:

.. doxygenclass:: APTracer::Cameras::CamMotionblurAperture_t
   :members:

.. doxygenclass:: APTracer::Cameras::IsoCam_t
   :members:

.. doxygenclass:: APTracer::Cameras::IsoCamAperture_t
   :members:

.. doxygenclass:: APTracer::Cameras::IsoCamMotionblur_t
   :members:

.. doxygenclass:: APTracer::Cameras::IsoCamMotionblurAperture_t
   :members:

.. doxygenclass:: APTracer::Cameras::RecCam_t
   :members:

.. doxygenclass:: APTracer::Cameras::RecCamAperture_t
   :members:

.. doxygenclass:: APTracer::Cameras::RecCamMotionblur_t
   :members:

.. doxygenclass:: APTracer::Cameras::RecCamMotionblurAperture_t
   :members:

.. Materials

.. doxygenclass:: APTracer::Materials::Absorber_t
   :members:

.. doxygenclass:: APTracer::Materials::BounceMaterial_t
   :members:

.. doxygenclass:: APTracer::Materials::Diffuse_t
   :members:

.. doxygenclass:: APTracer::Materials::DiffuseFull_t
   :members:

.. doxygenclass:: APTracer::Materials::DiffuseNormal_t
   :members:

.. doxygenclass:: APTracer::Materials::DiffuseTex_t
   :members:

.. doxygenclass:: APTracer::Materials::DiffuseTexNormal_t
   :members:

.. doxygenclass:: APTracer::Materials::DistanceMaterial_t
   :members:

.. doxygenclass:: APTracer::Materials::FresnelMix_t
   :members:

.. doxygenclass:: APTracer::Materials::FresnelMixIn_t
   :members:

.. doxygenclass:: APTracer::Materials::FresnelMixNormal_t
   :members:

.. doxygenclass:: APTracer::Materials::NonAbsorber_t
   :members:

.. doxygenclass:: APTracer::Materials::NormalDiffuseMaterial_t
   :members:

.. doxygenclass:: APTracer::Materials::NormalMaterial_t
   :members:

.. doxygenclass:: APTracer::Materials::Portal_t
   :members:

.. doxygenclass:: APTracer::Materials::PortalScatterer_t
   :members:

.. doxygenclass:: APTracer::Materials::RandomMix_t
   :members:

.. doxygenclass:: APTracer::Materials::RandomMixIn_t
   :members:

.. doxygenclass:: APTracer::Materials::Reflective_t
   :members:

.. doxygenclass:: APTracer::Materials::ReflectiveFuzz_t
   :members:

.. doxygenclass:: APTracer::Materials::ReflectiveFuzzNormal_t
   :members:

.. doxygenclass:: APTracer::Materials::ReflectiveFuzzTex_t
   :members:

.. doxygenclass:: APTracer::Materials::ReflectiveFuzzTexNormal_t
   :members:

.. doxygenclass:: APTracer::Materials::ReflectiveNormal_t
   :members:

.. doxygenclass:: APTracer::Materials::ReflectiveRefractive_t
   :members:

.. doxygenclass:: APTracer::Materials::ReflectiveRefractiveFuzz_t
   :members:

.. doxygenclass:: APTracer::Materials::ReflectiveRefractiveNormal_t
   :members:

.. doxygenclass:: APTracer::Materials::Refractive_t
   :members:

.. doxygenclass:: APTracer::Materials::RefractiveFuzz_t
   :members:

.. doxygenclass:: APTracer::Materials::Scatterer_t
   :members:

.. doxygenclass:: APTracer::Materials::ScattererExp_t
   :members:

.. doxygenclass:: APTracer::Materials::ScattererExpFull_t
   :members:

.. doxygenclass:: APTracer::Materials::ScattererFull_t
   :members:

.. doxygenclass:: APTracer::Materials::Toon_t
   :members:

.. doxygenclass:: APTracer::Materials::Transparent_t
   :members:

.. Shapes

.. doxygenclass:: APTracer::Shapes::Box_t
   :members:

.. doxygenclass:: APTracer::Shapes::Mesh_t
   :members:

.. doxygenclass:: APTracer::Shapes::MeshMotionblur_t
   :members:

.. doxygenclass:: APTracer::Shapes::MeshTop_t
   :members:

.. doxygenclass:: APTracer::Shapes::Sphere_t
   :members:

.. doxygenclass:: APTracer::Shapes::SphereMotionblur_t
   :members:

.. doxygenclass:: APTracer::Shapes::Triangle_t
   :members:

.. doxygenclass:: APTracer::Shapes::TriangleMesh_t
   :members:

.. doxygenclass:: APTracer::Shapes::TriangleMeshMotionblur_t
   :members:

.. doxygenclass:: APTracer::Shapes::TriangleMotionblur_t
   :members:

.. Skyboxes

.. doxygenclass:: APTracer::Skyboxes::SkyboxFlat_t
   :members:

.. doxygenclass:: APTracer::Skyboxes::SkyboxFlatSun_t
   :members:

.. doxygenclass:: APTracer::Skyboxes::SkyboxTexture_t
   :members:

.. doxygenclass:: APTracer::Skyboxes::SkyboxTextureSun_t
   :members:

.. doxygenclass:: APTracer::Skyboxes::SkyboxTextureTransformation_t
   :members:

.. doxygenclass:: APTracer::Skyboxes::SkyboxTextureTransformationSun_t
   :members:

.. Functions

.. doxygenfunction:: APTracer::next_filename

.. doxygenfunction:: APTracer::slerp

.. doxygenvariable:: APTracer::Colours::colours
