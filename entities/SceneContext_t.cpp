#include "SceneContext_t.h"

#include <iostream>
#include <string>
#include <sstream> 
#include "Colours.h"
#include "NextFilename.h"
#include "OpenGLRenderer_t.h"

#include "Diffuse_t.h"
#include "DiffuseFull_t.h"
#include "DiffuseTex_t.h"
#include "Reflective_t.h"
#include "ReflectiveFuzz_t.h"
#include "ReflectiveFuzzTex_t.h"
#include "ReflectiveRefractive_t.h"
#include "ReflectiveRefractiveFuzz_t.h"
#include "Refractive_t.h"
#include "RefractiveFuzz_t.h"
#include "Transparent_t.h"
#include "PortalTop_t.h"
#include "Portal_t.h"
#include "NormalMaterial_t.h"
#include "FresnelMix_t.h"
#include "FresnelMixIn_t.h"
#include "RandomMix_t.h"
#include "RandomMixIn_t.h"

#include "NonAbsorber_t.h"
#include "Absorber_t.h"
#include "Scatterer_t.h"
#include "ScattererExp_t.h"
#include "PortalScattererTop_t.h"
#include "PortalScatterer_t.h"

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
    n_skyboxes_(0), n_imgbuffers_(0), n_cameras_(0), index_transform_matrices_(0), index_textures_(0), 
    index_scatterers_(0), index_materials_(0), index_mesh_geometries_(0), index_objects_(0), 
    index_directional_lights_(0), index_skyboxes_(0), index_imgbuffers_(0), index_cameras_(0),
    transform_matrices_(nullptr), textures_(nullptr), scatterers_(nullptr), materials_(nullptr), 
    mesh_geometries_(nullptr), objects_(nullptr), directional_lights_(nullptr), skyboxes_(nullptr), 
    imgbuffers_(nullptr), cameras_(nullptr) 
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

    std::list<unsigned int>** scatterers_medium_list = nullptr;
    unsigned int** materials_mix_list = nullptr;
    std::list<unsigned int>** materials_medium_list = nullptr;

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
        scatterers_medium_list = new std::list<unsigned int>*[n_scatterers_];
        for (unsigned int i = 0; i < n_scatterers_; i++){
            scatterers_medium_list[i] = nullptr;
        }
    }
    if (n_materials_){
        materials_ = new Material_t*[n_materials_];
        materials_mix_list = new unsigned int*[n_materials_];
        materials_medium_list = new std::list<unsigned int>*[n_materials_];
        for (unsigned int i = 0; i < n_materials_; i++){
            materials_mix_list[i] = nullptr;
            materials_medium_list[i] = nullptr;
        }
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
            transform_matrices_[index_transform_matrices_] = create_transform_matrix(xml_transform_matrix);
            ++index_transform_matrices_;
        }
    }

    if (xml_textures != nullptr){
        for (tinyxml2::XMLElement* xml_texture = xml_textures->FirstChildElement("texture"); xml_texture; xml_texture = xml_texture->NextSiblingElement("texture")){
            textures_[index_textures_] = create_texture(xml_texture);
            ++index_textures_;
        }
    }

    if (xml_scatterers != nullptr){
        for (tinyxml2::XMLElement* xml_scatterer = xml_scatterers->FirstChildElement("scatterer"); xml_scatterer; xml_scatterer = xml_scatterer->NextSiblingElement("scatterer")){
            scatterers_[index_scatterers_] = create_scatterer(xml_scatterer, scatterers_medium_list[index_scatterers_], xml_transform_matrices, xml_materials);
            ++index_scatterers_;
        }
    }

    if (xml_materials != nullptr){
        for (tinyxml2::XMLElement* xml_material = xml_materials->FirstChildElement("material"); xml_material; xml_material = xml_material->NextSiblingElement("material")){
            materials_[index_materials_] = create_material(xml_material, materials_medium_list[index_materials_], materials_mix_list[index_materials_], xml_textures, xml_transform_matrices, xml_materials, xml_scatterers);
            ++index_materials_;
        }
    }


    // Fixes 
    // Material mixes fix

    if (materials_mix_list != nullptr){
        for (unsigned int i = 0; i < n_materials_; i++){
            if (materials_mix_list[i] != nullptr){
                delete [] materials_mix_list[i];
            }
        }
        delete [] materials_mix_list;
        materials_mix_list = nullptr;
    }

    // Materials mediumn list fix

    if (materials_medium_list != nullptr){
        for (unsigned int i = 0; i < n_materials_; i++){
            if (materials_medium_list[i] != nullptr){
                delete materials_medium_list[i];
            }
        }
        delete [] materials_medium_list;
        materials_medium_list = nullptr;
    }

    // Scatterers medium list fix
    for (unsigned int i = 0; i < n_scatterers_; i++){
        if (scatterers_medium_list[i] != nullptr) {
            for (auto it = scatterers_medium_list[i]->begin(); it != scatterers_medium_list[i]->end(); ++it){
 
                PortalScattererTop_t* portal_scatterer = dynamic_cast<PortalScattererTop_t*>(scatterers_[i]);
                Medium_t* medium = dynamic_cast<Medium_t*>(materials_[*it]); // CHECK I don't like those either
                portal_scatterer->medium_list_.push_back(medium);
            }
        }
    }

    if (scatterers_medium_list != nullptr){
        for (unsigned int i = 0; i < n_scatterers_; i++){
            if (scatterers_medium_list[i] != nullptr){
                delete scatterers_medium_list[i];
            }
        }
        delete [] scatterers_medium_list;
        scatterers_medium_list = nullptr;
    }

    // Material aggregates fix

}    

void SceneContext_t::reset(){
    index_transform_matrices_ = 0;
    index_textures_ = 0;
    index_scatterers_ = 0;
    index_materials_ = 0;
    index_mesh_geometries_ = 0;
    index_objects_ = 0;
    index_directional_lights_ = 0;
    index_skyboxes_ = 0;
    index_imgbuffers_ = 0;
    index_cameras_ = 0;

    // Deleting buffers
    if (transform_matrices_ != nullptr){
        for (unsigned int i = 0; i < n_transform_matrices_; i++){
            if (transform_matrices_[i] != nullptr){
                delete transform_matrices_[i];
            }
        }
        delete [] transform_matrices_;
        transform_matrices_ = nullptr;
    }
    n_transform_matrices_ = 0;

    if (textures_ != nullptr){
        for (unsigned int i = 0; i < n_textures_; i++){
            if (textures_[i] != nullptr){
                delete textures_[i];
            }
        }
        delete [] textures_;
        textures_ = nullptr;
    }
    n_textures_ = 0;
    
    if (scatterers_ != nullptr){
        for (unsigned int i = 0; i < n_scatterers_; i++){
            if (scatterers_[i] != nullptr){
                delete scatterers_[i];
            }
        }
        delete [] scatterers_;
        scatterers_ = nullptr;
    }
    n_scatterers_ = 0;

    if (materials_ != nullptr){
        for (unsigned int i = 0; i < n_materials_; i++){
            if (materials_[i] != nullptr){
                delete materials_[i];
            }
        }
        delete [] materials_;
        materials_ = nullptr;
    }
    n_materials_ = 0;

    if (mesh_geometries_ != nullptr){
        for (unsigned int i = 0; i < n_mesh_geometries_; i++){
            if (mesh_geometries_[i] != nullptr){
                delete mesh_geometries_[i];
            }
        }
        delete [] mesh_geometries_;
        mesh_geometries_ = nullptr;
    }
    n_mesh_geometries_ = 0;

    if (objects_ != nullptr){
        for (unsigned int i = 0; i < n_objects_; i++){
            if (objects_[i] != nullptr){
                delete objects_[i];
            }
        }
        delete [] objects_;
        objects_ = nullptr;
    }
    n_objects_ = 0;

    if (directional_lights_ != nullptr){
        for (unsigned int i = 0; i < n_directional_lights_; i++){
            if (directional_lights_[i] != nullptr){
                delete directional_lights_[i];
            }
        }
        delete [] directional_lights_;
        directional_lights_ = nullptr;
    }
    n_directional_lights_ = 0;

    if (skyboxes_ != nullptr){
        for (unsigned int i = 0; i < n_skyboxes_; i++){
            if (skyboxes_[i] != nullptr){
                delete skyboxes_[i];
            }
        }
        delete [] skyboxes_;
        skyboxes_ = nullptr;
    }
    n_skyboxes_ = 0;

    if (imgbuffers_ != nullptr){
        for (unsigned int i = 0; i < n_imgbuffers_; i++){
            if (imgbuffers_[i] != nullptr){
                delete imgbuffers_[i];
            }
        }
        delete [] imgbuffers_;
        imgbuffers_ = nullptr;
    }
    n_imgbuffers_ = 0;

    if (cameras_ != nullptr){
        for (unsigned int i = 0; i < n_cameras_; i++){
            if (cameras_[i] != nullptr){
                delete cameras_[i];
            }
        }
        delete [] cameras_;
        cameras_ = nullptr;
    }
    n_cameras_ = 0;

    if (opengl_renderer_ != nullptr){
        delete opengl_renderer_;
        opengl_renderer_= nullptr;
    }

    use_gl_ = false;
    scene_name_ = "";
}

TransformMatrix_t* SceneContext_t::create_transform_matrix(const tinyxml2::XMLElement* xml_transform_matrix) const {
    std::string string_transform_matrix = xml_transform_matrix->Attribute("value");
    std::transform(string_transform_matrix.begin(), string_transform_matrix.end(), string_transform_matrix.begin(), ::tolower);

    if (string_transform_matrix == "nan"){
        return new TransformMatrix_t();
    }
    else{
        double values[16];
        unsigned int count = 0;
        std::stringstream ss(string_transform_matrix);
        
        for(std::string s; ss >> s; ){
            if (count < 16){
                values[count] = std::stod(s);
            }
            ++count;
        }
        if (count != 16) {
            std::cout << "Error, transform matrix value should be 16 values seperated by spaces, or nan. Current number of values is " << count << ". Ignoring." << std::endl;
            return new TransformMatrix_t();
        }
        else{
            return new TransformMatrix_t(values[0], values[1], values[2], values[3],
                                        values[4], values[5], values[6], values[7],
                                        values[8], values[9], values[10], values[11],
                                        values[12], values[13], values[14], values[15]);
        }
    }
}

Texture_t* SceneContext_t::create_texture(const tinyxml2::XMLElement* xml_texture) const {
    std::string type = xml_texture->Attribute("type");
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);

    if (type == "texture"){
        return new Texture_t(xml_texture->Attribute("filename"));
    }
    else{
        std::cout << "Error, texture type '" << type << "' not implemented. Only 'texture' exists for now. Exiting." << std::endl; 
        exit(20);
    }
}

ScatteringFunction_t* SceneContext_t::create_scatterer(const tinyxml2::XMLElement* xml_scatterer, std::list<unsigned int>* &scatterers_medium_list, const tinyxml2::XMLElement* xml_transform_matrices, const tinyxml2::XMLElement* xml_materials) {
    std::string type = xml_scatterer->Attribute("type");
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);

    if (type == "absorber"){
        return new Absorber_t(get_colour(xml_scatterer->Attribute("emission")), get_colour(xml_scatterer->Attribute("colour")), 
                                std::stod(xml_scatterer->Attribute("emission_distance")), std::stod(xml_scatterer->Attribute("absorption_distance")));
    }
    else if (type == "nonabsorber"){
        return new NonAbsorber_t();
    }
    else if (type == "portal_scatterer"){
        // CHECK add medium_list stuff
        scatterers_medium_list = get_medium_list(xml_scatterer->Attribute("medium_list"), xml_materials);
        return new PortalScatterer_t(get_transform_matrix(xml_scatterer->Attribute("transform_matrix"), xml_transform_matrices), std::stod(xml_scatterer->Attribute("scattering_distance")), std::list<Medium_t*>());
    }
    else if (type == "scatterer_exp"){
        return new ScattererExp_t(get_colour(xml_scatterer->Attribute("emission")), get_colour(xml_scatterer->Attribute("colour")),
                                std::stod(xml_scatterer->Attribute("emission_distance")), std::stod(xml_scatterer->Attribute("absorption_distance")),
                                std::stod(xml_scatterer->Attribute("scattering_distance")), std::stod(xml_scatterer->Attribute("order")), 
                                std::stod(xml_scatterer->Attribute("scattering_angle")));
    }
    else if (type == "scatterer"){
        return new Scatterer_t(get_colour(xml_scatterer->Attribute("emission")), get_colour(xml_scatterer->Attribute("colour")),
                                std::stod(xml_scatterer->Attribute("emission_distance")), std::stod(xml_scatterer->Attribute("absorption_distance")),
                                std::stod(xml_scatterer->Attribute("scattering_distance")));
    }
    else{
        std::cout << "Error, scatterer type '" << type << "' not implemented. Only 'absorber', 'nonabsorber', 'portal_scatterer', 'scatterer_exp', and 'scatterer' exists for now. Ignoring." << std::endl; 
        return new NonAbsorber_t();
    }
}

Material_t* SceneContext_t::create_material(const tinyxml2::XMLElement* xml_material, std::list<unsigned int>* &materials_medium_list, unsigned int* &materials_mix_list, const tinyxml2::XMLElement* xml_textures, const tinyxml2::XMLElement* xml_transform_matrices, const tinyxml2::XMLElement* xml_materials, const tinyxml2::XMLElement* xml_scatterers) {
    std::string type = xml_material->Attribute("type");
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);

    if (type == "diffuse"){
        return new Diffuse_t(get_colour(xml_material->Attribute("emission")), get_colour(xml_material->Attribute("colour")), 
                                std::stod(xml_material->Attribute("roughness")));
    }
    else if (type == "diffuse_full"){
        return new DiffuseFull_t(get_texture(xml_material->Attribute("emission_map"), xml_textures), get_texture(xml_material->Attribute("texture"), xml_textures), 
                                std::stod(xml_material->Attribute("roughness")));
    }
    else if (type == "diffuse_tex"){
        return new DiffuseTex_t(get_colour(xml_material->Attribute("emission")), get_texture(xml_material->Attribute("texture"), xml_textures), 
                                std::stod(xml_material->Attribute("roughness")));
    }
    else if (type == "fresnelmix"){
        materials_mix_list = get_material_mix(xml_material->Attribute("material_refracted"), xml_material->Attribute("material_reflected"), xml_materials);
        return new FresnelMix_t(nullptr, nullptr, std::stod(xml_material->Attribute("ind")));
    }
    else if (type == "fresnelmix_in"){
        materials_mix_list = get_material_mix(xml_material->Attribute("material_refracted"), xml_material->Attribute("material_reflected"), xml_materials);
        return new FresnelMixIn_t(nullptr, nullptr, std::stod(xml_material->Attribute("ind")));
    }
    else if (type == "normal_material"){
        return new NormalMaterial_t();
    }
    else if (type == "portal"){
        materials_medium_list = get_medium_list(xml_material->Attribute("medium_list"), xml_materials);
        return new Portal_t(get_transform_matrix(xml_material->Attribute("transform_matrix"), xml_transform_matrices), std::list<Medium_t*>());
    }
    else if (type == "portal_refractive"){
        std::cout << "Error, refractive portal not implemented yet. Ignoring." << std::endl; 
        return new Diffuse_t(Vec3f(0.0, 0.0, 0.0), Vec3f(0.5, 0.5, 0.5), 1.0);
    }
    else if (type == "randommix"){
        materials_mix_list = get_material_mix(xml_material->Attribute("material_refracted"), xml_material->Attribute("material_reflected"), xml_materials);
        return new RandomMix_t(nullptr, nullptr, std::stod(xml_material->Attribute("ratio")));
    }
    else if (type == "randommix_in"){
        materials_mix_list = get_material_mix(xml_material->Attribute("material_refracted"), xml_material->Attribute("material_reflected"), xml_materials);
        return new RandomMixIn_t(nullptr, nullptr, std::stod(xml_material->Attribute("ratio")));
    }
    else if (type == "reflective"){
        return new Reflective_t(get_colour(xml_material->Attribute("emission")), get_colour(xml_material->Attribute("colour")));
    }
    else if (type == "reflective_fuzz"){
        return new ReflectiveFuzz_t(get_colour(xml_material->Attribute("emission")), get_colour(xml_material->Attribute("colour")), std::stod(xml_material->Attribute("order")), std::stod(xml_material->Attribute("diffusivity")));
    }
    else if (type == "reflective_fuzz_tex"){
        return new ReflectiveFuzzTex_t(get_texture(xml_material->Attribute("texture"), xml_textures), get_colour(xml_material->Attribute("emission")), std::stod(xml_material->Attribute("order")), std::stod(xml_material->Attribute("diffusivity")));
    }
    else if (type == "reflective_refractive"){
        return new ReflectiveRefractive_t(get_colour(xml_material->Attribute("emission")), get_colour(xml_material->Attribute("colour")), std::stod(xml_material->Attribute("ind")), std::stoi(xml_material->Attribute("priority")), get_scatterer(xml_material->Attribute("scattering_fn"), xml_scatterers));
    }
    else if (type == "reflective_refractive_fuzz"){
        return new ReflectiveRefractiveFuzz_t(get_colour(xml_material->Attribute("emission")), get_colour(xml_material->Attribute("colour")), std::stod(xml_material->Attribute("ind")), std::stoi(xml_material->Attribute("priority")), std::stod(xml_material->Attribute("order")), std::stod(xml_material->Attribute("diffusivity")), get_scatterer(xml_material->Attribute("scattering_fn"), xml_scatterers));
    }
    else if (type == "refractive"){
        return new Refractive_t(get_colour(xml_material->Attribute("emission")), get_colour(xml_material->Attribute("colour")), std::stod(xml_material->Attribute("ind")), std::stoi(xml_material->Attribute("priority")), get_scatterer(xml_material->Attribute("scattering_fn"), xml_scatterers));
    }
    else if (type == "refractive_fuzz"){
        return new RefractiveFuzz_t(get_colour(xml_material->Attribute("emission")), get_colour(xml_material->Attribute("colour")), std::stod(xml_material->Attribute("ind")), std::stoi(xml_material->Attribute("priority")), std::stod(xml_material->Attribute("order")), std::stod(xml_material->Attribute("diffusivity")), get_scatterer(xml_material->Attribute("scattering_fn"), xml_scatterers));
    }
    else if (type == "transparent"){
        return new Transparent_t(std::stoi(xml_material->Attribute("priority")), get_scatterer(xml_material->Attribute("scattering_fn"), xml_scatterers));
    }
    else if (type == "toon"){
        std::cout << "Error, toon shader not implemented yet. Ignoring." << std::endl; 
        return new Diffuse_t(Vec3f(0.0, 0.0, 0.0), Vec3f(0.5, 0.5, 0.5), 1.0);
    }
    else if (type == "aggregate"){
        
    }
    else{
        std::cout << "Error, material type '" << type << "' not implemented. Ignoring." << std::endl; 
        return new Diffuse_t(Vec3f(0.0, 0.0, 0.0), Vec3f(0.5, 0.5, 0.5), 1.0);
    }
}

void SceneContext_t::render(){

}

Vec3f SceneContext_t::get_colour(std::string colour) const {
    std::transform(colour.begin(), colour.end(), colour.begin(), ::tolower);
    std::map<std::string, Vec3f>::iterator it;
    it = my_colours::colours.find(colour);
    if (it != my_colours::colours.end()) {
		return it->second;
	} 
    else {
        double values[3];
        unsigned int count = 0;
		std::stringstream ss(colour);

        for(std::string s; ss >> s; ){
            if (count < 3){
                values[count] = std::stod(s);
            }
            ++count;
        }

        if (count != 3) {
            std::cout << "Error, colour should be 3 values seperated by spaces, or a string. Current number of values is " << count << ", colour is '" << colour << "'. Ignoring." << std::endl;
            return Vec3f(0.5, 0.5, 0.5);
        }
        else{
            return Vec3f(values[0], values[1], values[2]);
        }
	}
}

TransformMatrix_t* SceneContext_t::get_transform_matrix(std::string transform_matrix, const tinyxml2::XMLElement* xml_transform_matrices){
    std::transform(transform_matrix.begin(), transform_matrix.end(), transform_matrix.begin(), ::tolower);
    if (transform_matrix == "nan"){
        transform_matrices_[index_transform_matrices_] = new TransformMatrix_t();
        ++index_transform_matrices_;
        return transform_matrices_[index_transform_matrices_ - 1];
    }
    else if (is_number(transform_matrix)) {
        return transform_matrices_[std::stoi(transform_matrix) - 1];
    }
    else {
        if (xml_transform_matrices != nullptr){
            unsigned int index = 0;
            for (const tinyxml2::XMLElement* xml_transform_matrix = xml_transform_matrices->FirstChildElement("transform_matrix"); xml_transform_matrix; xml_transform_matrix = xml_transform_matrix->NextSiblingElement("transform_matrix")){
                std::string name_transform_matrix = xml_transform_matrix->Attribute("name");
                std::transform(name_transform_matrix.begin(), name_transform_matrix.end(), name_transform_matrix.begin(), ::tolower);
                if (name_transform_matrix == transform_matrix){
                    return transform_matrices_[index];
                }
                ++index;
            }
        }
    }
    std::cout << "Error, transformation matrix '" << transform_matrix << "' not found. Ignoring. This causes a memory leak." << std::endl;
    return new TransformMatrix_t();
}

std::list<unsigned int>* SceneContext_t::get_medium_list(std::string string_medium_list, const tinyxml2::XMLElement* xml_materials) const {
    std::list<unsigned int>* medium_list = new std::list<unsigned int>();
    std::string delimiter = ", ";
    size_t pos = 0;
    std::string token;

    while ((pos = string_medium_list.find(delimiter)) != std::string::npos) {
        token = string_medium_list.substr(0, pos);

        if (is_number(token)) {
            medium_list->push_back(std::stoi(token) - 1);
        }
        else {
            if (xml_materials != nullptr){
                unsigned int index = 0;
                for (const tinyxml2::XMLElement* xml_material = xml_materials->FirstChildElement("material"); xml_material; xml_material = xml_material->NextSiblingElement("material")){
                    std::string name_material = xml_material->Attribute("name");
                    std::transform(name_material.begin(), name_material.end(), name_material.begin(), ::tolower);
                    if (name_material == token){
                        medium_list->push_back(index);
                        break;
                    }
                    ++index;
                }
            }
        }
        // CHECK this should check for errors.

        string_medium_list.erase(0, pos + delimiter.length());
    }
    if (is_number(string_medium_list)) {
        medium_list->push_back(std::stoi(string_medium_list) - 1);
    }
    else {
        if (xml_materials != nullptr){
            unsigned int index = 0;
            for (const tinyxml2::XMLElement* xml_material = xml_materials->FirstChildElement("material"); xml_material; xml_material = xml_material->NextSiblingElement("material")){
                std::string name_material = xml_material->Attribute("name");
                std::transform(name_material.begin(), name_material.end(), name_material.begin(), ::tolower);
                if (name_material == string_medium_list){
                    medium_list->push_back(index);
                    break;
                }
                ++index;
            }
        }
    }
    return medium_list;
}

Texture_t* SceneContext_t::get_texture(std::string texture, const tinyxml2::XMLElement* xml_textures) const {
    if (is_number(texture)) {
        return textures_[std::stoi(texture) - 1];
    }
    else {
        if (xml_textures != nullptr){
            std::transform(texture.begin(), texture.end(), texture.begin(), ::tolower);
            unsigned int index = 0;
            for (const tinyxml2::XMLElement* xml_texture = xml_textures->FirstChildElement("texture"); xml_texture; xml_texture = xml_texture->NextSiblingElement("texture")){
                std::string name_texture = xml_texture->Attribute("name");
                std::transform(name_texture.begin(), name_texture.end(), name_texture.begin(), ::tolower);
                if (name_texture == texture){
                    return textures_[index];
                }
                ++index;
            }
        }
    }
    std::cout << "Error, texture '" << texture << "' not found. Exiting." << std::endl;
    exit(21); 
}

unsigned int* SceneContext_t::get_material_mix(std::string material_refracted, std::string material_reflected, const tinyxml2::XMLElement* xml_materials) const {
    unsigned int* output_materials = new unsigned int[2];
    
    if (is_number(material_refracted)) {
        output_materials[0] = std::stoi(material_refracted) - 1;
    }
    else {
        if (xml_materials != nullptr){
            std::transform(material_refracted.begin(), material_refracted.end(), material_refracted.begin(), ::tolower);
            bool material_missing = true;
            unsigned int index = 0;
            for (const tinyxml2::XMLElement* xml_material = xml_materials->FirstChildElement("material"); xml_material; xml_material = xml_material->NextSiblingElement("material")){
                std::string name_material = xml_material->Attribute("name");
                std::transform(name_material.begin(), name_material.end(), name_material.begin(), ::tolower);
                if (material_refracted == name_material){
                    output_materials[0] = index;
                    material_missing = false;
                    break;
                }
                ++index;
            }
            if (material_missing){
                std::cout << "Error, material '" << material_refracted << "' not found. Exiting." << std::endl;
                exit(41); 
            }
        }
        else{
            std::cout << "Error, no materials, '" << material_refracted << "' not found. Exiting." << std::endl;
            exit(47);
        }        
    }

    if (is_number(material_reflected)) {
        output_materials[1] = std::stoi(material_reflected) - 1;
    }
    else {
        if (xml_materials != nullptr){
            std::transform(material_reflected.begin(), material_reflected.end(), material_reflected.begin(), ::tolower);
            bool material_missing = true;
            unsigned int index = 0;
            for (const tinyxml2::XMLElement* xml_material = xml_materials->FirstChildElement("material"); xml_material; xml_material = xml_material->NextSiblingElement("material")){
                std::string name_material = xml_material->Attribute("name");
                std::transform(name_material.begin(), name_material.end(), name_material.begin(), ::tolower);
                if (material_reflected == name_material){
                    output_materials[1] = index;
                    material_missing = false;
                    break;
                }
                ++index;
            }
            if (material_missing){
                std::cout << "Error, material '" << material_reflected << "' not found. Exiting." << std::endl;
                exit(41); 
            }
        }
        else{
            std::cout << "Error, no materials, '" << material_reflected << "' not found. Exiting." << std::endl;
            exit(47);
        }        
    }

    return output_materials;
}

ScatteringFunction_t* SceneContext_t::get_scatterer(std::string scatterer, const tinyxml2::XMLElement* xml_scatterers) const {
    if (is_number(scatterer)) {
        return scatterers_[std::stoi(scatterer) - 1];
    }
    else {
        if (xml_scatterers != nullptr){
            std::transform(scatterer.begin(), scatterer.end(), scatterer.begin(), ::tolower);
            unsigned int index = 0;
            for (const tinyxml2::XMLElement* xml_scatterer = xml_scatterers->FirstChildElement("scatterer"); xml_scatterer; xml_scatterer = xml_scatterer->NextSiblingElement("scatterer")){
                std::string name_scatterer = xml_scatterer->Attribute("name");
                std::transform(name_scatterer.begin(), name_scatterer.end(), name_scatterer.begin(), ::tolower);
                if (name_scatterer == scatterer){
                    return scatterers_[index];
                }
                ++index;
            }
        }
    }
    std::cout << "Error, scatterer '" << scatterer << "' not found. Ignoring. This Causes a memory leak." << std::endl;
    return new NonAbsorber_t();
}

bool SceneContext_t::is_number(const std::string& s) const {
    return !s.empty() && std::find_if(s.begin(), 
        s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}