#include "SceneContext_t.h"

#include <iostream>
#include "Colours.h"
#include "NextFilename.h"
#include "OpenGLRenderer_t.h"

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
    use_gl_(false), scene_name_(""), opengl_renderer_(nullptr), n_transform_matrices_(0), n_textures_(0), 
    n_scatterers_(0), n_materials_(0), n_mesh_geometries_(0), n_objects_(0), n_directional_lights_(0), 
    n_skyboxes_(0), n_imgbuffers_(0), n_cameras_(0), transform_matrices_(nullptr), textures_(nullptr), 
    scatterers_(nullptr), materials_(nullptr), mesh_geometries_(nullptr), objects_(nullptr), 
    directional_lights_(nullptr), skyboxes_(nullptr), imgbuffers_(nullptr), cameras_(nullptr) 
    {}

SceneContext_t::~SceneContext_t() {
    reset();
}

void SceneContext_t::readXML(const std::string &filename){
    reset();

    std::string new_filename, folder;

    #ifdef _WIN32
        folder = "images\\";
    #else
        folder = "images/";
    #endif

    tinyxml2::XMLDocument xml_scene;
	xml_scene.LoadFile(filename.c_str());
    if (xml_scene.Error()){
        std::cout << "There was an error while opening or parsing file '" << filename << "'. Exiting." << std::endl;
        return;
    }

    tinyxml2::XMLElement* xml_top = xml_scene.FirstChildElement();
    scene_name_ = xml_top->Attribute("name");
    new_filename = next_filename(folder + scene_name_ + ".png");

    std::cout << "Scene name: '" << scene_name_ << "'." << std::endl; // REMOVE
    std::cout << "Filename: '" << new_filename << "'." << std::endl; // REMOVE

    // Data
    unsigned int index_transform_matrices = 0;
    unsigned int index_textures = 0;
    unsigned int index_scatterers = 0;
    unsigned int index_materials = 0;
    unsigned int index_mesh_geometries = 0;
    unsigned int index_objects = 0;
    unsigned int index_directional_lights = 0;
    unsigned int index_skyboxes = 0;
    unsigned int index_imgbuffers = 0;
    unsigned int index_cameras = 0;

    // Fields
    tinyxml2::XMLElement* xml_transform_matrices = xml_top->FirstChildElement("transform_matrices_");
    tinyxml2::XMLElement* xml_textures = xml_top->FirstChildElement("textures_");
    tinyxml2::XMLElement* xml_scatterers = xml_top->FirstChildElement("scatterers_");
    tinyxml2::XMLElement* xml_materials = xml_top->FirstChildElement("materials_");
    tinyxml2::XMLElement* xml_mesh_geometries = xml_top->FirstChildElement("mesh_geometries_");
    tinyxml2::XMLElement* xml_objects = xml_top->FirstChildElement("objects_");
    tinyxml2::XMLElement* xml_directional_lights = xml_top->FirstChildElement("directional_lights_");
    tinyxml2::XMLElement* xml_skyboxes = xml_top->FirstChildElement("skyboxes_");
    tinyxml2::XMLElement* xml_imgbuffers = xml_top->FirstChildElement("imgbuffers_");
    tinyxml2::XMLElement* xml_cameras = xml_top->FirstChildElement("cameras_");

    // Counts
    if (xml_transform_matrices != nullptr){
        for (tinyxml2::XMLElement* xml_transform_matrix = xml_transform_matrices->FirstChildElement("transform_matrix"); xml_transform_matrix; xml_transform_matrix = xml_transform_matrix->NextSiblingElement("transform_matrix")){
            ++n_transform_matrices_;
        }
    }

    if (xml_textures != nullptr){
        for (tinyxml2::XMLElement* xml_texture = xml_textures->FirstChildElement("texture"); xml_texture; xml_texture = xml_texture->NextSiblingElement("texture")){
            ++n_textures_;
        }
    }

    if (xml_scatterers != nullptr){
        for (tinyxml2::XMLElement* xml_scatterer = xml_scatterers->FirstChildElement("scatterer"); xml_scatterer; xml_scatterer = xml_scatterer->NextSiblingElement("scatterer")){
            ++n_scatterers_;
            const char* char_transform_matrix = xml_scatterer->Attribute("transform_matrix");
            if (char_transform_matrix != nullptr){
                std::string string_transform_matrix = char_transform_matrix;
                std::transform(string_transform_matrix.begin(), string_transform_matrix.end(), string_transform_matrix.begin(), ::tolower);
                if (string_transform_matrix == "nan"){
                    ++n_transform_matrices_;
                }
            }
        }
    }

    if (xml_materials != nullptr){
        for (tinyxml2::XMLElement* xml_material = xml_materials->FirstChildElement("material"); xml_material; xml_material = xml_material->NextSiblingElement("material")){
            ++ n_materials_;
            const char* char_transform_matrix = xml_material->Attribute("transform_matrix");
            if (char_transform_matrix != nullptr){
                std::string string_transform_matrix = char_transform_matrix;
                std::transform(string_transform_matrix.begin(), string_transform_matrix.end(), string_transform_matrix.begin(), ::tolower);
                if (string_transform_matrix == "nan"){
                    ++n_transform_matrices_;
                }
            }
        }
    }

    if (xml_mesh_geometries != nullptr){
        for (tinyxml2::XMLElement* xml_mesh_geometry = xml_mesh_geometries->FirstChildElement("mesh_geometry"); xml_mesh_geometry; xml_mesh_geometry = xml_mesh_geometry->NextSiblingElement("mesh_geometry")){
            ++n_mesh_geometries_;
        }
    }

    if (xml_objects != nullptr){
        for (tinyxml2::XMLElement* xml_object = xml_objects->FirstChildElement("object"); xml_object; xml_object = xml_object->NextSiblingElement("object")){
            ++n_objects_;
            const char* char_transform_matrix = xml_object->Attribute("transform_matrix");
            if (char_transform_matrix != nullptr){
                std::string string_transform_matrix = char_transform_matrix;
                std::transform(string_transform_matrix.begin(), string_transform_matrix.end(), string_transform_matrix.begin(), ::tolower);
                if (string_transform_matrix == "nan"){
                    ++n_transform_matrices_;
                }
            }
        }
    }

    if (xml_directional_lights != nullptr){
        for (tinyxml2::XMLElement* xml_directional_light = xml_directional_lights->FirstChildElement("directional_light"); xml_directional_light; xml_directional_light = xml_directional_light->NextSiblingElement("directional_light")){
            ++n_directional_lights_;
            const char* char_transform_matrix = xml_directional_light->Attribute("transform_matrix");
            if (char_transform_matrix != nullptr){
                std::string string_transform_matrix = char_transform_matrix;
                std::transform(string_transform_matrix.begin(), string_transform_matrix.end(), string_transform_matrix.begin(), ::tolower);
                if (string_transform_matrix == "nan"){
                    ++n_transform_matrices_;
                }
            }
        }
    }

    if (xml_skyboxes != nullptr){
        for (tinyxml2::XMLElement* xml_skybox = xml_skyboxes->FirstChildElement("skybox"); xml_skybox; xml_skybox = xml_skybox->NextSiblingElement("skybox")){
            ++n_skyboxes_;
            const char* char_transform_matrix = xml_skybox->Attribute("transform_matrix");
            if (char_transform_matrix != nullptr){
                std::string string_transform_matrix = char_transform_matrix;
                std::transform(string_transform_matrix.begin(), string_transform_matrix.end(), string_transform_matrix.begin(), ::tolower);
                if (string_transform_matrix == "nan"){
                    ++n_transform_matrices_;
                }
            }
        }
    }

    if (xml_imgbuffers != nullptr){
        for (tinyxml2::XMLElement* xml_imgbuffer = xml_imgbuffers->FirstChildElement("imgbuffer"); xml_imgbuffer; xml_imgbuffer = xml_imgbuffer->NextSiblingElement("imgbuffer")){
            ++n_imgbuffers_;
        }
    }

    if (xml_cameras != nullptr){
        for (tinyxml2::XMLElement* xml_camera = xml_cameras->FirstChildElement("camera"); xml_camera; xml_camera = xml_camera->NextSiblingElement("camera")){
            ++n_cameras_;
            const char* char_transform_matrix = xml_camera->Attribute("transform_matrix");
            if (char_transform_matrix != nullptr){
                std::string string_transform_matrix = char_transform_matrix;
                std::transform(string_transform_matrix.begin(), string_transform_matrix.end(), string_transform_matrix.begin(), ::tolower);
                if (string_transform_matrix == "nan"){
                    ++n_transform_matrices_;
                }
            }
        }
    }

    std::cout << "Transform matrix count: " << n_transform_matrices_ << std::endl; // REMOVE
    std::cout << "Texture count: " << n_textures_ << std::endl; // REMOVE
    std::cout << "Scatterer count: " << n_scatterers_ << std::endl; // REMOVE
    std::cout << "Material count: " << n_materials_ << std::endl; // REMOVE
    std::cout << "Mesh count: " << n_mesh_geometries_ << std::endl; // REMOVE
    std::cout << "Object count: " << n_objects_ << std::endl; // REMOVE
    std::cout << "Directional light count: " << n_directional_lights_ << std::endl; // REMOVE
    std::cout << "Skybox count: " << n_skyboxes_ << std::endl; // REMOVE
    std::cout << "Img buffers count: " << n_imgbuffers_ << std::endl; // REMOVE
    std::cout << "Camera count: " << n_cameras_ << std::endl << std::endl; // REMOVE

    // Buffer creation
    if (n_transform_matrices_){
        transform_matrices_ = new TransformMatrix_t*[n_transform_matrices_];
    }
    if (n_textures_){
        textures_ = new Texture_t*[n_textures_];
    }
    if (n_scatterers_){
        scatterers_ = new ScatteringFunction_t*[n_scatterers_];
    }
    if (n_materials_){
        materials_ = new Material_t*[n_materials_];
    }
    if (n_mesh_geometries_){
        mesh_geometries_ = new MeshGeometry_t*[n_mesh_geometries_];
    }
    if (n_objects_){
        objects_ = new Shape_t*[n_objects_];
    }
    if (n_directional_lights_){
        directional_lights_ = new DirectionalLight_t*[n_directional_lights_];
    }
    if (n_skyboxes_){
        skyboxes_ = new Skybox_t*[n_skyboxes_];
    }
    if (n_imgbuffers_){
        imgbuffers_ = new ImgBuffer_t*[n_imgbuffers_];
    }
    if (n_cameras_){
        cameras_ = new Camera_t*[n_cameras_];
    }

    // Filling buffers
    if (xml_transform_matrices != nullptr){
        for (tinyxml2::XMLElement* xml_transform_matrix = xml_transform_matrices->FirstChildElement("transform_matrix"); xml_transform_matrix; xml_transform_matrix = xml_transform_matrix->NextSiblingElement("transform_matrix")){
            transform_matrices_[index_transform_matrices] = create_transform_matrix(xml_transform_matrix);
            ++index_transform_matrices;
        }
    }

    if (xml_textures != nullptr){
        for (tinyxml2::XMLElement* xml_texture = xml_textures->FirstChildElement("texture"); xml_textures; xml_textures = xml_textures->NextSiblingElement("texture")){
            textures_[index_textures] = create_texture(xml_texture);
            ++index_textures;
        }
    }

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

Texture_t* SceneContext_t::create_texture(const tinyxml2::XMLElement* xml_texture) const {

}

void SceneContext_t::render(){
    
}