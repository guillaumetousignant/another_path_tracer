#include "SceneContext_t.h"

#include "Diffuse_t.h"
#include "DiffuseTex_t.h"
#include "Refractive_t.h"
#include "Reflective_t.h"
#include "ReflectiveRefractiveFuzz_t.h"
#include "ReflectiveRefractive_t.h"

#include "NonAbsorber_t.h"
#include "Absorber_t.h"
#include "Transparent_t.h"

#include "TransformMatrix_t.h"
#include "ImgBuffer_t.h"
#include "ImgBufferOpenGL_t.h"
#include "Scene_t.h"
#include "MeshGeometry_t.h"
#include "DirectionalLight_t.h"
#include "Texture_t.h"

#include "Sphere_t.h"
#include "SphereMotionblur_t.h"
#include "Triangle_t.h"
#include "TriangleMotionblur_t.h"
#include "Mesh_t.h"
#include "MeshMotionblur_t.h"

#include "Camera_t.h"
#include "Cam_t.h"
#include "CamAperture_t.h"
#include "CamMotionblur_t.h"
#include "CamMotionblurAperture_t.h"
#include "RecCam_t.h"
#include "RecCamAperture_t.h"
#include "RecCamMotionblur_t.h"
#include "RecCamMotionblurAperture_t.h"
#include "IsoCam_t.h"
#include "IsoCamAperture_t.h"
#include "IsoCamMotionblur_t.h"
#include "IsoCamMotionblurAperture_t.h"
#include "Cam3D_t.h"
#include "Cam3DAperture_t.h"
#include "Cam3DMotionblur_t.h"
#include "Cam3DMotionblurAperture_t.h"

#include "SkyboxFlat_t.h"
#include "SkyboxFlatSun_t.h"

SceneContext_t::SceneContext_t() :
    use_gl_(false), scene_name_(""), n_transform_matrices_(0), n_textures_(0), n_scatterers_(0),
    n_materials_(0), n_mesh_geometries_(0), n_objects_(0), n_directional_lights_(0), n_skyboxes_(0), 
    n_imgbuffers_(0), n_cameras_(0), transform_matrices_(nullptr), textures_(nullptr), scatterers_(nullptr), 
    materials_(nullptr), mesh_geometries_(nullptr), objects_(nullptr), directional_lights_(nullptr),
    skyboxes_(nullptr), imgbuffers_(nullptr), cameras_(nullptr) {}

SceneContext_t::~SceneContext_t() {
    reset();
}

void SceneContext_t::readXML(const std::string &filename){
    reset();
}    

void SceneContext_t::reset(){
    // Deleting buffers
    if (transform_matrices_ != nullptr){
        for (unsigned int i = 0; i < n_transform_matrices_; i++){
            delete transform_matrices_[i];
        }
        delete [] transform_matrices_;
    }
    n_transform_matrices_ = 0;

    if (textures_ != nullptr){
        for (unsigned int i = 0; i < n_textures_; i++){
            delete textures_[i];
        }
        delete [] textures_;
    }
    n_textures_ = 0;
    
    if (scatterers_ != nullptr){
        for (unsigned int i = 0; i < n_scatterers_; i++){
            delete scatterers_[i];
        }
        delete [] scatterers_;
    }
    n_scatterers_ = 0;

    if (materials_ != nullptr){
        for (unsigned int i = 0; i < n_materials_; i++){
            delete materials_[i];
        }
        delete [] materials_;
    }
    n_materials_ = 0;

    if (mesh_geometries_ != nullptr){
        for (unsigned int i = 0; i < n_mesh_geometries_; i++){
            delete mesh_geometries_[i];
        }
        delete [] mesh_geometries_;
    }
    n_mesh_geometries_ = 0;

    if (objects_ != nullptr){
        for (unsigned int i = 0; i < n_objects_; i++){
            delete objects_[i];
        }
        delete [] objects_;
    }
    n_objects_ = 0;

    if (directional_lights_ != nullptr){
        for (unsigned int i = 0; i < n_directional_lights_; i++){
            delete directional_lights_[i];
        }
        delete [] directional_lights_;
    }
    n_directional_lights_ = 0;

    if (skyboxes_ != nullptr){
        for (unsigned int i = 0; i < n_skyboxes_; i++){
            delete skyboxes_[i];
        }
        delete [] skyboxes_;
    }
    n_skyboxes_ = 0;

    if (imgbuffers_ != nullptr){
        for (unsigned int i = 0; i < n_imgbuffers_; i++){
            delete imgbuffers_[i];
        }
        delete [] imgbuffers_;
    }
    n_imgbuffers_ = 0;

    if (cameras_ != nullptr){
        for (unsigned int i = 0; i < n_cameras_; i++){
            delete cameras_[i];
        }
        delete [] cameras_;
    }
    n_cameras_ = 0;
}

TransformMatrix_t* SceneContext_t::create_transform_matrix(const tinyxml2::XMLElement* xml_transform_matrix) const {

}
