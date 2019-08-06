#include "SceneContext_t.h"

#include <iostream>
#include <string>
#include <sstream> 
#include <tuple>
#include <algorithm>
#include <chrono>

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
#include "MaterialMix_t.h"
#include "FresnelMix_t.h"
#include "FresnelMixIn_t.h"
#include "RandomMix_t.h"
#include "RandomMixIn_t.h"
#include "MaterialMap_t.h"

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
#include "TriangleMesh_t.h"
#include "TriangleMeshMotionblur_t.h"
#include "Mesh_t.h"
#include "MeshMotionblur_t.h"

#include "Camera_t.h"
#include "Cam_t.h"
#include "CamAperture_t.h"
#include "CamMotionblur_t.h"
#include "CamMotionblurAperture_t.h"
#include "IsoCam_t.h"
#include "IsoCamAperture_t.h"
#include "IsoCamMotionblur_t.h"
#include "IsoCamMotionblurAperture_t.h"
#include "RecCam_t.h"
#include "RecCamAperture_t.h"
#include "RecCamMotionblur_t.h"
#include "RecCamMotionblurAperture_t.h"
#include "Cam3D_t.h"
#include "Cam3DAperture_t.h"
#include "Cam3DMotionblur_t.h"
#include "Cam3DMotionblurAperture_t.h"

#include "SkyboxFlat_t.h"
#include "SkyboxFlatSun_t.h"
#include "SkyboxTexture_t.h"
#include "SkyboxTextureSun_t.h"
#include "SkyboxTextureTransformation_t.h"
#include "SkyboxTextureTransformationSun_t.h"

SceneContext_t::SceneContext_t() :
    use_gl_(false), scene_name_(""), opengl_renderer_(nullptr), opengl_imgbuffer_(nullptr), 
    opengl_camera_(nullptr), scene_(nullptr), camera_rendermode_(nullptr), camera_n_iter_(nullptr), camera_write_interval_(nullptr), n_transform_matrices_(0), 
    n_textures_(0), n_scatterers_(0), n_materials_(0), n_mesh_geometries_(0), n_objects_(0), 
    n_directional_lights_(0), n_skyboxes_(0), n_imgbuffers_(0), n_cameras_(0), 
    index_transform_matrices_(0), index_textures_(0), index_scatterers_(0), index_materials_(0), 
    index_mesh_geometries_(0), index_objects_(0), index_directional_lights_(0), index_skyboxes_(0), 
    index_imgbuffers_(0), index_cameras_(0), transform_matrices_(nullptr), textures_(nullptr), 
    scatterers_(nullptr), materials_(nullptr), mesh_geometries_(nullptr), objects_(nullptr), 
    directional_lights_(nullptr), skyboxes_(nullptr), imgbuffers_(nullptr), cameras_(nullptr), 
    material_aggregates_(nullptr), meshes_(nullptr) 
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
    const char* temp_name = xml_top->Attribute("name");
    if (temp_name == nullptr) {
        std::cout << "Scene name not found. XML scene should have a 'name' attribute. Using 'null'." << std::endl;
        scene_name_ = "null";
    }
    else {
        scene_name_ = temp_name;
    }
    new_filename = next_filename(folder + scene_name_ + ".png");

    std::cout << "Scene name: '" << scene_name_ << "'." << std::endl; // REMOVE
    std::cout << "Next filename: '" << new_filename << "'." << std::endl; // REMOVE

    // Fields
    tinyxml2::XMLElement* xml_transform_matrices = xml_top->FirstChildElement("transform_matrices");
    tinyxml2::XMLElement* xml_textures = xml_top->FirstChildElement("textures");
    tinyxml2::XMLElement* xml_scatterers = xml_top->FirstChildElement("scatterers");
    tinyxml2::XMLElement* xml_materials = xml_top->FirstChildElement("materials");
    tinyxml2::XMLElement* xml_mesh_geometries = xml_top->FirstChildElement("mesh_geometries");
    tinyxml2::XMLElement* xml_objects = xml_top->FirstChildElement("objects");
    tinyxml2::XMLElement* xml_directional_lights = xml_top->FirstChildElement("directional_lights");
    tinyxml2::XMLElement* xml_skyboxes = xml_top->FirstChildElement("skyboxes");
    tinyxml2::XMLElement* xml_imgbuffers = xml_top->FirstChildElement("imgbuffers");
    tinyxml2::XMLElement* xml_cameras = xml_top->FirstChildElement("cameras");

    std::list<unsigned int>** scatterers_medium_list = nullptr;
    unsigned int** materials_mix_list = nullptr;
    std::list<unsigned int>** materials_medium_list = nullptr;
    std::tuple<std::list<unsigned int>*, std::list<std::string>*>** materials_aggregate_list = nullptr;

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
            ++n_materials_;
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

    std::cout << std::endl;
    std::cout << "Transform matrix count: " << n_transform_matrices_ << std::endl;
    std::cout << "Texture count: " << n_textures_ << std::endl;
    std::cout << "Scatterer count: " << n_scatterers_ << std::endl;
    std::cout << "Material count: " << n_materials_ << std::endl;
    std::cout << "Mesh count: " << n_mesh_geometries_ << std::endl;
    std::cout << "Shape count: " << n_objects_ << std::endl;
    std::cout << "Directional light count: " << n_directional_lights_ << std::endl;
    std::cout << "Skybox count: " << n_skyboxes_ << std::endl;
    std::cout << "Img buffers count: " << n_imgbuffers_ << std::endl;
    std::cout << "Camera count: " << n_cameras_ << std::endl << std::endl;

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
        materials_aggregate_list = new std::tuple<std::list<unsigned int>*, std::list<std::string>*>*[n_materials_];
        material_aggregates_ = new MaterialMap_t*[n_materials_];
        for (unsigned int i = 0; i < n_materials_; i++){
            materials_mix_list[i] = nullptr;
            materials_medium_list[i] = nullptr;
            materials_aggregate_list[i] = nullptr;
            material_aggregates_[i] = nullptr;
        }
    }
    if (n_mesh_geometries_){
        mesh_geometries_ = new MeshGeometry_t*[n_mesh_geometries_];
    }
    if (n_objects_){
        objects_ = new Shape_t*[n_objects_];
        meshes_ = new MeshTop_t*[n_objects_];
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

    std::cout << "Buffers allocated." << std::endl << std::endl;

    // Filling buffers
    // Transform matrices (1)
    if (xml_transform_matrices != nullptr){
        for (tinyxml2::XMLElement* xml_transform_matrix = xml_transform_matrices->FirstChildElement("transform_matrix"); xml_transform_matrix; xml_transform_matrix = xml_transform_matrix->NextSiblingElement("transform_matrix")){
            transform_matrices_[index_transform_matrices_] = create_transform_matrix(xml_transform_matrix);
            ++index_transform_matrices_;
        }
        std::cout << "Transform matrices created." << std::endl;
    }

    // Textures (2)
    if (xml_textures != nullptr){
        for (tinyxml2::XMLElement* xml_texture = xml_textures->FirstChildElement("texture"); xml_texture; xml_texture = xml_texture->NextSiblingElement("texture")){
            textures_[index_textures_] = create_texture(xml_texture);
            ++index_textures_;
        }
        std::cout << "Textures created." << std::endl;
    }

    // Scatterers (3)
    if (xml_scatterers != nullptr){
        for (tinyxml2::XMLElement* xml_scatterer = xml_scatterers->FirstChildElement("scatterer"); xml_scatterer; xml_scatterer = xml_scatterer->NextSiblingElement("scatterer")){
            scatterers_[index_scatterers_] = create_scatterer(xml_scatterer, scatterers_medium_list[index_scatterers_], xml_transform_matrices, xml_materials);
            ++index_scatterers_;
        }
        std::cout << "Scatterers created." << std::endl;
    }

    // Materials (4)
    if (xml_materials != nullptr){
        for (tinyxml2::XMLElement* xml_material = xml_materials->FirstChildElement("material"); xml_material; xml_material = xml_material->NextSiblingElement("material")){
            materials_[index_materials_] = create_material(xml_material, materials_medium_list[index_materials_], materials_mix_list[index_materials_], materials_aggregate_list[index_materials_], xml_textures, xml_transform_matrices, xml_materials, xml_scatterers);
            ++index_materials_;
        }
        std::cout << "Materials created." << std::endl;
    }

    // Fixes 
    // Material mixes fix
    for (unsigned int i = 0; i < n_materials_; i++){
        if (materials_mix_list[i] != nullptr){
            MaterialMix_t* material_mix = dynamic_cast<MaterialMix_t*>(materials_[i]); // dynamic caaaast :(
            if (material_mix == nullptr){
                std::cout << "Error: material #" << i << " was marked as a material mix but is not convertible to one. Exiting." << std::endl;
                exit(491);
            }
            material_mix->material_refracted_ = materials_[materials_mix_list[i][0]];
            material_mix->material_reflected_ = materials_[materials_mix_list[i][1]];
        }
    }

    if (materials_mix_list != nullptr){
        for (unsigned int i = 0; i < n_materials_; i++){
            if (materials_mix_list[i] != nullptr){
                delete [] materials_mix_list[i];
            }
        }
        delete [] materials_mix_list;
        materials_mix_list = nullptr;
    }

    // Materials medium list fix
    for (unsigned int i = 0; i < n_materials_; i++){
        if (materials_medium_list[i] != nullptr) {
            PortalTop_t* portal = dynamic_cast<PortalTop_t*>(materials_[i]);
            if (portal == nullptr){
                std::cout << "Error: material #" << i << " was marked as a portal but is not convertible to one. Exiting." << std::endl;
                exit(492);
            }
            for (auto it = materials_medium_list[i]->begin(); it != materials_medium_list[i]->end(); ++it){
                Medium_t* medium = dynamic_cast<Medium_t*>(materials_[*it]); // CHECK I don't like those either
                if (medium == nullptr){
                    std::cout << "Error: material #" << i << " had material #" << *it << " in its medium list, but it is not convertible to one. Exiting." << std::endl;
                    exit(493);
                }
                portal->medium_list_.push_back(medium);
            }
        }
    }

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
            PortalScattererTop_t* portal_scatterer = dynamic_cast<PortalScattererTop_t*>(scatterers_[i]);
            if (portal_scatterer == nullptr){
                std::cout << "Error: scatterer #" << i << " was marked as a portal but is not convertible to one. Exiting." << std::endl;
                exit(392);
            }
            for (auto it = scatterers_medium_list[i]->begin(); it != scatterers_medium_list[i]->end(); ++it){
                Medium_t* medium = dynamic_cast<Medium_t*>(materials_[*it]); // CHECK I don't like those either
                if (medium == nullptr){
                    std::cout << "Error: scatterer #" << i << " had material #" << *it << " in its medium list, but it is not convertible to one. Exiting." << std::endl;
                    exit(393);
                }
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
    for (unsigned int i = 0; i < n_materials_; i++){
        if (materials_aggregate_list[i] != nullptr) {
            unsigned int n = std::get<0>(*materials_aggregate_list[i])->size();
            std::string* names = new std::string[n];
            Material_t** materials = new Material_t*[n];

            unsigned int index = 0;
            for (auto it = std::get<0>(*materials_aggregate_list[i])->begin(); it != std::get<0>(*materials_aggregate_list[i])->end(); ++it){
                materials[index] = materials_[*it];
                ++index;
            }
            index = 0;
            for (auto it = std::get<1>(*materials_aggregate_list[i])->begin(); it != std::get<1>(*materials_aggregate_list[i])->end(); ++it){
                names[index] = *it;
                ++index;
            }
            
            material_aggregates_[i] = new MaterialMap_t(names, materials, n);
            materials_[i] = material_aggregates_[i]->getFirst();

            delete [] names;
            delete [] materials;
        }
    }

    if (materials_aggregate_list != nullptr){
        for (unsigned int i = 0; i < n_materials_; i++){
            if (materials_aggregate_list[i] != nullptr){
                delete std::get<0>(*materials_aggregate_list[i]);
                delete std::get<1>(*materials_aggregate_list[i]);
                delete materials_aggregate_list[i]; // CHECK is this right? should delete both members?
            }
        }
        delete [] materials_aggregate_list;
        materials_aggregate_list = nullptr;
    }
    std::cout << "\tFixes done." << std::endl;

    // Filling buffers again
    // Mesh geometries (5)
    if (xml_mesh_geometries != nullptr){
        for (tinyxml2::XMLElement* xml_mesh_geometry = xml_mesh_geometries->FirstChildElement("mesh_geometry"); xml_mesh_geometry; xml_mesh_geometry = xml_mesh_geometry->NextSiblingElement("mesh_geometry")){
            mesh_geometries_[index_mesh_geometries_] = create_mesh_geometry(xml_mesh_geometry);
            ++index_mesh_geometries_;
        }
        std::cout << "Mesh geometries created." << std::endl;
    }

    // Objects (6)
    if (xml_objects != nullptr){
        for (tinyxml2::XMLElement* xml_object = xml_objects->FirstChildElement("object"); xml_object; xml_object = xml_object->NextSiblingElement("object")){
            objects_[index_objects_] = create_object(xml_object, meshes_[index_objects_], xml_transform_matrices, xml_materials, xml_mesh_geometries);
            ++index_objects_;
        }
        std::cout << "Objects created." << std::endl;
    }

    // Directional lights (7)
    if (xml_directional_lights != nullptr){
        for (tinyxml2::XMLElement* xml_directional_light = xml_directional_lights->FirstChildElement("directional_light"); xml_directional_light; xml_directional_light = xml_directional_light->NextSiblingElement("directional_light")){
            directional_lights_[index_directional_lights_] = create_directional_light(xml_directional_light, xml_transform_matrices);
            ++index_directional_lights_;
        }
        std::cout << "Directional lights created." << std::endl;
    }

    // Skyboxes (8)
    if (xml_skyboxes != nullptr){
        for (tinyxml2::XMLElement* xml_skybox = xml_skyboxes->FirstChildElement("skybox"); xml_skybox; xml_skybox = xml_skybox->NextSiblingElement("skybox")){
            skyboxes_[index_skyboxes_] = create_skybox(xml_skybox, xml_textures, xml_transform_matrices, xml_directional_lights);
            ++index_skyboxes_;
        }
        std::cout << "Skyboxes created." << std::endl;
    }

    // Image buffers (9)
    if (xml_imgbuffers != nullptr){
        for (tinyxml2::XMLElement* xml_imgbuffer = xml_imgbuffers->FirstChildElement("imgbuffer"); xml_imgbuffer; xml_imgbuffer = xml_imgbuffer->NextSiblingElement("imgbuffer")){
            imgbuffers_[index_imgbuffers_] = create_imgbuffer(xml_imgbuffer);
            ++index_imgbuffers_;
        }
        std::cout << "Image buffers created." << std::endl;
    }

    // Cameras (10)
    if (xml_cameras != nullptr){
        for (tinyxml2::XMLElement* xml_camera = xml_cameras->FirstChildElement("camera"); xml_camera; xml_camera = xml_camera->NextSiblingElement("camera")){
            cameras_[index_cameras_] = create_camera(xml_camera, new_filename, xml_transform_matrices, xml_materials, xml_imgbuffers, xml_skyboxes);
            ++index_cameras_;
        }
        std::cout << "Cameras created." << std::endl;
    }
    std::cout << std::endl;

    // Updating pre
    // Transform matrices
    if (xml_transform_matrices != nullptr){
        unsigned int index = 0;
        for (tinyxml2::XMLElement* xml_transform_matrix = xml_transform_matrices->FirstChildElement("transform_matrix"); xml_transform_matrix; xml_transform_matrix = xml_transform_matrix->NextSiblingElement("transform_matrix")){
            tinyxml2::XMLElement* transformations_pre = xml_transform_matrix->FirstChildElement("transformations_pre");
            if (transformations_pre != nullptr){
                for (tinyxml2::XMLElement* transformation_pre = transformations_pre->FirstChildElement("transformation_pre"); transformation_pre; transformation_pre = transformation_pre->NextSiblingElement("transformation_pre")){
                    apply_transformation(transform_matrices_[index], transformation_pre);
                }
            }
            ++index;
        }
        std::cout << "Transform matrices transformed." << std::endl;
    }

    // Materials
    if (xml_materials != nullptr){
        unsigned int index = 0;
        for (tinyxml2::XMLElement* xml_material = xml_materials->FirstChildElement("material"); xml_material; xml_material = xml_material->NextSiblingElement("material")){
            tinyxml2::XMLElement* transformations_pre = xml_material->FirstChildElement("transformations_pre");
            if (transformations_pre != nullptr){
                PortalTop_t* portal = dynamic_cast<PortalTop_t*>(materials_[index]);
                if (portal == nullptr){
                    std::cout << "Error, material #" << index << " has transformations, but it is not convertible to a portal. Ignoring." << std::endl;
                }
                else{
                    for (tinyxml2::XMLElement* transformation_pre = transformations_pre->FirstChildElement("transformation_pre"); transformation_pre; transformation_pre = transformation_pre->NextSiblingElement("transformation_pre")){
                        apply_transformation(portal->transformation_, transformation_pre);
                    }
                }                
            }
            ++index;
        }
        std::cout << "Materials transformed." << std::endl;
    }

    // Directional lights
    if (xml_directional_lights != nullptr){
        unsigned int index = 0;
        for (tinyxml2::XMLElement* xml_directional_light = xml_directional_lights->FirstChildElement("directional_light"); xml_directional_light; xml_directional_light = xml_directional_light->NextSiblingElement("directional_light")){
            tinyxml2::XMLElement* transformations_pre = xml_directional_light->FirstChildElement("transformations_pre");
            if (transformations_pre != nullptr){
                for (tinyxml2::XMLElement* transformation_pre = transformations_pre->FirstChildElement("transformation_pre"); transformation_pre; transformation_pre = transformation_pre->NextSiblingElement("transformation_pre")){
                    apply_transformation(directional_lights_[index]->transformation_, transformation_pre);
                }
            }
            ++index;
        }
        std::cout << "Directional lights transformed." << std::endl;
    }

    // Objects
    if (xml_objects != nullptr){
        unsigned int index = 0;
        for (tinyxml2::XMLElement* xml_object = xml_objects->FirstChildElement("object"); xml_object; xml_object = xml_object->NextSiblingElement("object")){
            tinyxml2::XMLElement* transformations_pre = xml_object->FirstChildElement("transformations_pre");
            if (transformations_pre != nullptr){
                TransformMatrix_t* transform_matrix = (objects_[index] != nullptr) ? objects_[index]->transformation_ : meshes_[index]->transformation_;
                for (tinyxml2::XMLElement* transformation_pre = transformations_pre->FirstChildElement("transformation_pre"); transformation_pre; transformation_pre = transformation_pre->NextSiblingElement("transformation_pre")){
                    apply_transformation(transform_matrix, transformation_pre);
                }
            }
            ++index;
        }
        std::cout << "Shapes transformed." << std::endl;
    }

    // Cameras
    if (xml_cameras != nullptr){
        unsigned int index = 0;
        for (tinyxml2::XMLElement* xml_camera = xml_cameras->FirstChildElement("camera"); xml_camera; xml_camera = xml_camera->NextSiblingElement("camera")){
            tinyxml2::XMLElement* transformations_pre = xml_camera->FirstChildElement("transformations_pre");
            if (transformations_pre != nullptr){
                for (tinyxml2::XMLElement* transformation_pre = transformations_pre->FirstChildElement("transformation_pre"); transformation_pre; transformation_pre = transformation_pre->NextSiblingElement("transformation_pre")){
                    apply_transformation(cameras_[index]->transformation_, transformation_pre);
                }
            }
            ++index;
        }
        std::cout << "Cameras transformed." << std::endl;
    }
    std::cout << std::endl;

    // Scene building
    scene_ = new Scene_t();
    const char* object_list = xml_top->Attribute("object_list");
    std::cout << "Scene created." << std::endl;
    
    if (object_list != nullptr){
        Shape_t** shapes = nullptr;
        MeshTop_t** meshes = nullptr;
        unsigned int n_shapes = 0;
        unsigned int n_meshes = 0;

        get_objects(object_list, shapes, n_shapes, meshes, n_meshes, xml_objects);

        scene_->add(shapes, n_shapes);
        scene_->add(meshes, n_meshes);

        delete [] shapes;
        delete [] meshes;
        std::cout << "Primitives added." << std::endl;
    }

    // Update
    std::cout << std::endl << "Updating scene..." << std::endl;
    auto t_start = std::chrono::high_resolution_clock::now();
    scene_->update();
    for (unsigned int i = 0; i < n_cameras_; i++){
        cameras_[i]->update();
    }
    auto t_end = std::chrono::high_resolution_clock::now();
    std::cout << "Scene updated in " 
        << std::chrono::duration<double, std::milli>(t_end-t_start).count()/1000.0 
        << "s." << std::endl << std::endl;

    // Updating post
    // Transform matrices
    if (xml_transform_matrices != nullptr){
        unsigned int index = 0;
        for (tinyxml2::XMLElement* xml_transform_matrix = xml_transform_matrices->FirstChildElement("transform_matrix"); xml_transform_matrix; xml_transform_matrix = xml_transform_matrix->NextSiblingElement("transform_matrix")){
            tinyxml2::XMLElement* transformations_post = xml_transform_matrix->FirstChildElement("transformations_post");
            if (transformations_post != nullptr){
                for (tinyxml2::XMLElement* transformation_post = transformations_post->FirstChildElement("transformation_post"); transformation_post; transformation_post = transformation_post->NextSiblingElement("transformation_post")){
                    apply_transformation(transform_matrices_[index], transformation_post);
                }
            }
            ++index;
        }
        std::cout << "Transform matrices transformed post." << std::endl;
    }

    // Materials
    if (xml_materials != nullptr){
        unsigned int index = 0;
        for (tinyxml2::XMLElement* xml_material = xml_materials->FirstChildElement("material"); xml_material; xml_material = xml_material->NextSiblingElement("material")){
            tinyxml2::XMLElement* transformations_post = xml_material->FirstChildElement("transformations_post");
            if (transformations_post != nullptr){
                PortalTop_t* portal = dynamic_cast<PortalTop_t*>(materials_[index]);
                if (portal == nullptr){
                    std::cout << "Error, material #" << index << " has transformations, but it is not convertible to a portal. Ignoring." << std::endl;
                }
                else{
                    for (tinyxml2::XMLElement* transformation_post = transformations_post->FirstChildElement("transformation_post"); transformation_post; transformation_post = transformation_post->NextSiblingElement("transformation_post")){
                        apply_transformation(portal->transformation_, transformation_post);
                    }
                }                
            }
            ++index;
        }
        std::cout << "Materials transformed post." << std::endl;
    }

    // Directional lights
    if (xml_directional_lights != nullptr){
        unsigned int index = 0;
        for (tinyxml2::XMLElement* xml_directional_light = xml_directional_lights->FirstChildElement("directional_light"); xml_directional_light; xml_directional_light = xml_directional_light->NextSiblingElement("directional_light")){
            tinyxml2::XMLElement* transformations_post = xml_directional_light->FirstChildElement("transformations_post");
            if (transformations_post != nullptr){
                for (tinyxml2::XMLElement* transformation_post = transformations_post->FirstChildElement("transformation_post"); transformation_post; transformation_post = transformation_post->NextSiblingElement("transformation_post")){
                    apply_transformation(directional_lights_[index]->transformation_, transformation_post);
                }
            }
            ++index;
        }
        std::cout << "Directional lights transformed post." << std::endl;
    }

    // Objects
    if (xml_objects != nullptr){
        unsigned int index = 0;
        for (tinyxml2::XMLElement* xml_object = xml_objects->FirstChildElement("object"); xml_object; xml_object = xml_object->NextSiblingElement("object")){
            tinyxml2::XMLElement* transformations_post = xml_object->FirstChildElement("transformations_post");
            if (transformations_post != nullptr){
                TransformMatrix_t* transform_matrix = (objects_[index] != nullptr) ? objects_[index]->transformation_ : meshes_[index]->transformation_;
                for (tinyxml2::XMLElement* transformation_post = transformations_post->FirstChildElement("transformation_post"); transformation_post; transformation_post = transformation_post->NextSiblingElement("transformation_post")){
                    apply_transformation(transform_matrix, transformation_post);
                }
            }
            ++index;
        }
        std::cout << "Shapes transformed post." << std::endl;
    }

    // Cameras
    if (xml_cameras != nullptr){
        unsigned int index = 0;
        for (tinyxml2::XMLElement* xml_camera = xml_cameras->FirstChildElement("camera"); xml_camera; xml_camera = xml_camera->NextSiblingElement("camera")){
            tinyxml2::XMLElement* transformations_post = xml_camera->FirstChildElement("transformations_post");
            if (transformations_post != nullptr){
                for (tinyxml2::XMLElement* transformation_post = transformations_post->FirstChildElement("transformation_post"); transformation_post; transformation_post = transformation_post->NextSiblingElement("transformation_post")){
                    apply_transformation(cameras_[index]->transformation_, transformation_post);
                }
            }
            ++index;
        }
        std::cout << "Cameras transformed post." << std::endl;
    }
    std::cout << std::endl;

    // Update
    std::cout << "Updating scene..." << std::endl;
    t_start = std::chrono::high_resolution_clock::now();
    scene_->update();
    t_end = std::chrono::high_resolution_clock::now();
    std::cout << "Scene updated in " 
        << std::chrono::duration<double, std::milli>(t_end-t_start).count()/1000.0 
        << "s." << std::endl << std::endl;

    // Acceleration structure build
    std::cout << "Building acceleration structure..." << std::endl;
    t_start = std::chrono::high_resolution_clock::now();
    scene_->build_acc();
    t_end = std::chrono::high_resolution_clock::now();
    std::cout << "Acceleration structure built in " 
        << std::chrono::duration<double, std::milli>(t_end-t_start).count()/1000.0 
        << "s." << std::endl << std::endl;

    // Autofocus
    if (xml_cameras != nullptr){
        unsigned int index = 0;
        for (tinyxml2::XMLElement* xml_camera = xml_cameras->FirstChildElement("camera"); xml_camera; xml_camera = xml_camera->NextSiblingElement("camera")){
            const char* focal_length_char = xml_camera->Attribute("focal_length");
            if (focal_length_char != nullptr){
                std::string focal_length = focal_length_char;
                std::transform(focal_length.begin(), focal_length.end(), focal_length.begin(), ::tolower);
                if (focal_length == "nan"){
                    double position[2];
                    const char* focus_position_char = xml_camera->Attribute("focus_position");
                    std::string focus_position;
                    if (focus_position_char == nullptr) {
                        std::cout << "Error: Cameras with nan as 'focal_length' attribute should have a 'focus_position' attribute. Using 0.5 0.5." << std::endl;
                        focus_position = "0.5 0.5";
                    }
                    else {
                        focus_position = focus_position_char;
                    }
                    get_xy(focus_position, position);
                    cameras_[index]->autoFocus(scene_, position);
                    std::cout << "Camera #" << index << " autofocus." << std::endl;
                }
            }
            ++index;
        }
    }
    for (unsigned int i = 0; i < n_cameras_; i++){
        cameras_[i]->update();
    }

    // Running modes
    if (xml_cameras != nullptr){
        if (use_gl_) {
            opengl_camera_ = cameras_[0]; // CHECK dunno how to fix this
            opengl_renderer_ = new OpenGLRenderer_t(scene_, opengl_camera_, opengl_imgbuffer_);
            opengl_renderer_->initialise();
            std::cout << "OpenGL initialised." << std::endl;
        }

        camera_rendermode_ = new std::string[n_cameras_];
        camera_n_iter_ = new unsigned int[n_cameras_];
        camera_write_interval_ = new unsigned int[n_cameras_];

        for (unsigned int i = 0; i < n_cameras_; i++){
            camera_rendermode_[i] = "";
            camera_n_iter_[i] = 0;
            camera_write_interval_[i] = 0;
        }

        unsigned int index = 0;
        for (tinyxml2::XMLElement* xml_camera = xml_cameras->FirstChildElement("camera"); xml_camera; xml_camera = xml_camera->NextSiblingElement("camera")){
            std::string render_mode;
            const char* render_mode_char = xml_camera->Attribute("rendermode");
            if (render_mode_char == nullptr){
                std::cout << "Error: XML cameras should have a 'rendermode' attribute. Using 'single'." << std::endl;
                render_mode = "single";
            }
            else{
                render_mode = render_mode_char;
            }

            std::transform(render_mode.begin(), render_mode.end(), render_mode.begin(), ::tolower);
            
            if (render_mode == "accumulation") {
                camera_rendermode_[index] = "accumulation";
                std::string n_iter;
                const char* n_iter_char = xml_camera->Attribute("n_iter");
                if (n_iter_char == nullptr){
                    std::cout << "Error: XML cameras in accumulation mode should have a 'n_iter' attribute. Using 1." << std::endl;
                    n_iter = "1";
                }
                else {
                    n_iter = n_iter_char;
                }

                std::transform(n_iter.begin(), n_iter.end(), n_iter.begin(), ::tolower);
                if (n_iter == "inf"){
                    camera_n_iter_[index] = 1000000000;
                }
                else {
                    camera_n_iter_[index] = std::stoi(n_iter);
                }              
            }
            else if (render_mode == "accumulation_write") {
                std::string n_iter, write_interval;
                const char* n_iter_char = xml_camera->Attribute("n_iter");
                const char* write_interval_char = xml_camera->Attribute("write_interval");
                if (n_iter_char == nullptr){
                    std::cout << "Error: XML cameras in accumulation mode should have a 'n_iter' attribute. Using 1." << std::endl;
                    n_iter = "1";
                }
                else {
                    n_iter = n_iter_char;
                }
                if (write_interval_char == nullptr){
                    std::cout << "Error: XML cameras in accumulation_write mode should have a 'write_interval' attribute. Using 1." << std::endl;
                    write_interval = "1";
                }
                else {
                    write_interval = write_interval_char;
                }

                camera_rendermode_[index] = "accumulation_write";

                std::transform(n_iter.begin(), n_iter.end(), n_iter.begin(), ::tolower);
                if (n_iter == "inf"){
                    camera_n_iter_[index] = 1000000000;
                }
                else {
                    camera_n_iter_[index] = std::stoi(n_iter);
                }

                std::transform(write_interval.begin(), write_interval.end(), write_interval.begin(), ::tolower);
                if (write_interval == "inf"){
                    camera_write_interval_[index] = 1000000000;
                }
                else {
                    camera_write_interval_[index] = std::stoi(write_interval);
                }
            }
            else if (render_mode == "single") {
                camera_rendermode_[index] = "single";
            }
            else if (render_mode == "motion") {
                std::cout << "Error, motion render mode not implemented yet. Single frame render fallback." << std::endl;
                camera_rendermode_[index] = "single";
            }
            else {
                std::cout << "Error, render mode '" << render_mode << "', used by camera #" << index << ", is unknown. Only 'accumulation', 'accumulation_write', 'single', and 'motion' exist for now. Ignoring." << std::endl;
                camera_rendermode_[index] = "";
            }

            ++index;
        }
        std::cout << "Camera render modes set." << std::endl;
    }
    std::cout << std::endl << "ReadXML done." << std::endl << std::endl;
}    

void SceneContext_t::render(){
    std::cout << "In rendering function." << std::endl;

    // Running
    if (use_gl_) {
        std::string render_mode = camera_rendermode_[0];
        std::transform(render_mode.begin(), render_mode.end(), render_mode.begin(), ::tolower);

        std::cout << "Camera #" << 0 << " rendering scene '" << scene_name_ << "' in '" <<  render_mode << "' mode." << std::endl;

        if (render_mode == "accumulation") {
            opengl_renderer_->render();
        }
        else if (render_mode == "accumulation_write") {
            opengl_renderer_->render_write(camera_write_interval_[0]);
        }
        else if (render_mode == "single") {
            opengl_renderer_->render_write();
        }
        else if (render_mode == "motion") {
            std::cout << "Error, motion render mode not implemented yet. Accumulation render fallback." << std::endl;
            opengl_renderer_->render();
        }
        else if (render_mode == "") {

        }
        else {
            std::cout << "Error, render mode '" << render_mode << "', used by camera #" << 0 << ", is unknown. Only 'accumulation', 'accumulation_write', 'single', and 'motion' exist for now. Ignoring." << std::endl;
        }
    }
    else {
        for (unsigned int i = 0; i < n_cameras_; i++){
            std::string render_mode = camera_rendermode_[i];
            std::transform(render_mode.begin(), render_mode.end(), render_mode.begin(), ::tolower);

            std::cout << "Camera #" << i << " rendering scene '" << scene_name_ << "' in '" <<  render_mode << "' mode." << std::endl;

            if (render_mode == "accumulation") {
                cameras_[i]->accumulate(scene_, camera_n_iter_[i]);
                cameras_[i]->write();
            }
            else if (render_mode == "accumulation_write") {
                cameras_[i]->accumulateWrite(scene_, camera_n_iter_[i], camera_write_interval_[i]);
                cameras_[i]->write();
            }
            else if (render_mode == "single") {
                cameras_[i]->raytrace(scene_);
                cameras_[i]->write();
            }
            else if (render_mode == "motion") {
                std::cout << "Error, motion render mode not implemented yet. Single frame render fallback." << std::endl;
                cameras_[i]->raytrace(scene_);
                cameras_[i]->write();
            }
            else if (render_mode == "") {

            }
            else {
                std::cout << "Error, render mode '" << render_mode << "', used by camera #" << i << ", is unknown. Only 'accumulation', 'accumulation_write', 'single', and 'motion' exist for now. Ignoring." << std::endl;
            }
        }
    }
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
    opengl_imgbuffer_ = nullptr;
    opengl_camera_ = nullptr;

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
    if (material_aggregates_ != nullptr){
        for (unsigned int i = 0; i < n_materials_; i++){
            if (material_aggregates_[i] != nullptr){
                delete material_aggregates_[i];
            }
        }
        delete [] material_aggregates_;
        material_aggregates_ = nullptr;
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
            if (meshes_[i] != nullptr){
                delete meshes_[i];
            }
        }
        delete [] objects_;
        delete [] meshes_;
        objects_ = nullptr;
        meshes_ = nullptr;
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

    if (scene_ != nullptr){
        delete scene_;
        scene_ = nullptr;
    }

    if (camera_rendermode_ != nullptr){
        delete [] camera_rendermode_;
        camera_rendermode_ = nullptr;
    }

    if (camera_n_iter_ != nullptr){
        delete [] camera_n_iter_;
        camera_n_iter_ = nullptr;
    }

    if (camera_write_interval_ != nullptr){
        delete [] camera_write_interval_;
        camera_write_interval_ = nullptr;
    }

    use_gl_ = false;
    scene_name_ = "";
}

TransformMatrix_t* SceneContext_t::create_transform_matrix(const tinyxml2::XMLElement* xml_transform_matrix) const {
    std::string string_transform_matrix;
    const char* transform_matrix_char = xml_transform_matrix->Attribute("value");
    if (transform_matrix_char == nullptr){
        std::cout << "Error: XML transform matrices should have a 'value' attribute. Using identity." << std::endl;
        string_transform_matrix = "nan";
    }
    else {
        string_transform_matrix = transform_matrix_char;
    }

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
    std::string type;
    const char* type_char = xml_texture->Attribute("type");
    if (type_char == nullptr) {
        std::cout << "Error: XML textures should have a 'type' attribute. Using 'texture'." << std::endl;
        type = "texture";
    }
    else {
        type = type_char;
    }
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);

    if (type == "texture"){
        const char* attributes[] = {"filename"};
        require_attributes(xml_texture, attributes, 1);
        std::string filename = xml_texture->Attribute("filename");
        for (unsigned int i = 0; i < filename.size(); i++){
            #ifdef _WIN32
                if (filename[i] == '/') {
                    filename[i] = '\\';
                }
            #else
                if (filename[i] == '\\') {
                    filename[i] = '/';
                }
            #endif
        }  
        
        return new Texture_t(filename);
    }
    else{
        std::cout << "Error, texture type '" << type << "' not implemented. Only 'texture' exists for now. Exiting." << std::endl; 
        exit(20);
    }
}

ScatteringFunction_t* SceneContext_t::create_scatterer(const tinyxml2::XMLElement* xml_scatterer, std::list<unsigned int>* &scatterers_medium_list, const tinyxml2::XMLElement* xml_transform_matrices, const tinyxml2::XMLElement* xml_materials) {
    std::string type;
    const char* type_char = xml_scatterer->Attribute("type");
    if (type_char == nullptr) {
        std::cout << "Error: XML scatterers should have a 'type' attribute. Using 'nonabsorber'." << std::endl;
        type = "nonabsorber";
    }
    else {
        type = type_char;
    }
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);

    if (type == "absorber"){
        const char* attributes[] = {"emission", "colour", "emission_distance", "absorption_distance"};
        require_attributes(xml_scatterer, attributes, 4);
        return new Absorber_t(get_colour(xml_scatterer->Attribute("emission")), get_colour(xml_scatterer->Attribute("colour")), 
                                std::stod(xml_scatterer->Attribute("emission_distance")), std::stod(xml_scatterer->Attribute("absorption_distance")));
    }
    else if (type == "nonabsorber"){
        return new NonAbsorber_t();
    }
    else if (type == "portal_scatterer"){
        // CHECK add medium_list stuff
        const char* attributes[] = {"medium_list", "transform_matrix", "scattering_distance"};
        require_attributes(xml_scatterer, attributes, 3);
        scatterers_medium_list = get_medium_index_list(xml_scatterer->Attribute("medium_list"), xml_materials);
        return new PortalScatterer_t(get_transform_matrix(xml_scatterer->Attribute("transform_matrix"), xml_transform_matrices), std::stod(xml_scatterer->Attribute("scattering_distance")), std::list<Medium_t*>());
    }
    else if (type == "scatterer_exp"){
        const char* attributes[] = {"emission", "colour", "emission_distance", "absorption_distance", "scattering_distance", "order", "scattering_angle"};
        require_attributes(xml_scatterer, attributes, 7);
        return new ScattererExp_t(get_colour(xml_scatterer->Attribute("emission")), get_colour(xml_scatterer->Attribute("colour")),
                                std::stod(xml_scatterer->Attribute("emission_distance")), std::stod(xml_scatterer->Attribute("absorption_distance")),
                                std::stod(xml_scatterer->Attribute("scattering_distance")), std::stod(xml_scatterer->Attribute("order")), 
                                std::stod(xml_scatterer->Attribute("scattering_angle")));
    }
    else if (type == "scatterer"){
        const char* attributes[] = {"emission", "colour", "emission_distance", "absorption_distance", "scattering_distance"};
        require_attributes(xml_scatterer, attributes, 5);
        return new Scatterer_t(get_colour(xml_scatterer->Attribute("emission")), get_colour(xml_scatterer->Attribute("colour")),
                                std::stod(xml_scatterer->Attribute("emission_distance")), std::stod(xml_scatterer->Attribute("absorption_distance")),
                                std::stod(xml_scatterer->Attribute("scattering_distance")));
    }
    else{
        std::cout << "Error, scatterer type '" << type << "' not implemented. Only 'absorber', 'nonabsorber', 'portal_scatterer', 'scatterer_exp', and 'scatterer' exists for now. Ignoring." << std::endl; 
        return new NonAbsorber_t();
    }
}

Material_t* SceneContext_t::create_material(const tinyxml2::XMLElement* xml_material, std::list<unsigned int>* &materials_medium_list, unsigned int* &materials_mix_list, std::tuple<std::list<unsigned int>*, std::list<std::string>*>* &materials_aggregate_list, const tinyxml2::XMLElement* xml_textures, const tinyxml2::XMLElement* xml_transform_matrices, const tinyxml2::XMLElement* xml_materials, const tinyxml2::XMLElement* xml_scatterers) {
    std::string type;
    const char* type_char = xml_material->Attribute("type");
    if (type_char == nullptr) {
        std::cout << "Error: XML materials should have a 'type' attribute. Using 'normal_material'." << std::endl;
        type = "normal_material";
    }
    else {
        type = type_char;
    }
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);

    if (type == "diffuse"){
        const char* attributes[] = {"emission", "colour", "roughness"};
        require_attributes(xml_material, attributes, 3);
        return new Diffuse_t(get_colour(xml_material->Attribute("emission")), get_colour(xml_material->Attribute("colour")), 
                                std::stod(xml_material->Attribute("roughness")));
    }
    else if (type == "diffuse_full"){
        const char* attributes[] = {"emission_map", "texture", "roughness"};
        require_attributes(xml_material, attributes, 3);
        return new DiffuseFull_t(get_texture(xml_material->Attribute("emission_map"), xml_textures), get_texture(xml_material->Attribute("texture"), xml_textures), 
                                std::stod(xml_material->Attribute("roughness")));
    }
    else if (type == "diffuse_tex"){
        const char* attributes[] = {"emission", "texture", "roughness"};
        require_attributes(xml_material, attributes, 3);
        return new DiffuseTex_t(get_colour(xml_material->Attribute("emission")), get_texture(xml_material->Attribute("texture"), xml_textures), 
                                std::stod(xml_material->Attribute("roughness")));
    }
    else if (type == "fresnelmix"){
        const char* attributes[] = {"material_refracted", "material_reflected", "ind"};
        require_attributes(xml_material, attributes, 3);
        materials_mix_list = get_material_mix(xml_material->Attribute("material_refracted"), xml_material->Attribute("material_reflected"), xml_materials);
        return new FresnelMix_t(nullptr, nullptr, std::stod(xml_material->Attribute("ind")));
    }
    else if (type == "fresnelmix_in"){
        const char* attributes[] = {"material_refracted", "material_reflected", "ind"};
        require_attributes(xml_material, attributes, 3);
        materials_mix_list = get_material_mix(xml_material->Attribute("material_refracted"), xml_material->Attribute("material_reflected"), xml_materials);
        return new FresnelMixIn_t(nullptr, nullptr, std::stod(xml_material->Attribute("ind")));
    }
    else if (type == "normal_material"){
        return new NormalMaterial_t();
    }
    else if (type == "portal"){
        const char* attributes[] = {"medium_list", "transform_matrix"};
        require_attributes(xml_material, attributes, 2);
        materials_medium_list = get_medium_index_list(xml_material->Attribute("medium_list"), xml_materials);
        return new Portal_t(get_transform_matrix(xml_material->Attribute("transform_matrix"), xml_transform_matrices), std::list<Medium_t*>());
    }
    else if (type == "portal_refractive"){
        std::cout << "Error, refractive portal not implemented yet. Ignoring." << std::endl; 
        return new Diffuse_t(Vec3f(0.0, 0.0, 0.0), Vec3f(0.5, 0.5, 0.5), 1.0);
    }
    else if (type == "randommix"){
        const char* attributes[] = {"material_refracted", "material_reflected", "ratio"};
        require_attributes(xml_material, attributes, 3);
        materials_mix_list = get_material_mix(xml_material->Attribute("material_refracted"), xml_material->Attribute("material_reflected"), xml_materials);
        return new RandomMix_t(nullptr, nullptr, std::stod(xml_material->Attribute("ratio")));
    }
    else if (type == "randommix_in"){
        const char* attributes[] = {"material_refracted", "material_reflected", "ratio"};
        require_attributes(xml_material, attributes, 3);
        materials_mix_list = get_material_mix(xml_material->Attribute("material_refracted"), xml_material->Attribute("material_reflected"), xml_materials);
        return new RandomMixIn_t(nullptr, nullptr, std::stod(xml_material->Attribute("ratio")));
    }
    else if (type == "reflective"){
        const char* attributes[] = {"emission", "colour"};
        require_attributes(xml_material, attributes, 2);
        return new Reflective_t(get_colour(xml_material->Attribute("emission")), get_colour(xml_material->Attribute("colour")));
    }
    else if (type == "reflective_fuzz"){
        const char* attributes[] = {"emission", "colour", "order", "diffusivity"};
        require_attributes(xml_material, attributes, 4);
        return new ReflectiveFuzz_t(get_colour(xml_material->Attribute("emission")), get_colour(xml_material->Attribute("colour")), std::stod(xml_material->Attribute("order")), std::stod(xml_material->Attribute("diffusivity")));
    }
    else if (type == "reflective_fuzz_tex"){
        const char* attributes[] = {"emission", "texture", "order", "diffusivity"};
        require_attributes(xml_material, attributes, 4);
        return new ReflectiveFuzzTex_t(get_colour(xml_material->Attribute("emission")), get_texture(xml_material->Attribute("texture"), xml_textures), std::stod(xml_material->Attribute("order")), std::stod(xml_material->Attribute("diffusivity")));
    }
    else if (type == "reflective_refractive"){
        const char* attributes[] = {"emission", "colour", "ind", "priority", "scattering_fn"};
        require_attributes(xml_material, attributes, 5);
        return new ReflectiveRefractive_t(get_colour(xml_material->Attribute("emission")), get_colour(xml_material->Attribute("colour")), std::stod(xml_material->Attribute("ind")), std::stoi(xml_material->Attribute("priority")), get_scatterer(xml_material->Attribute("scattering_fn"), xml_scatterers));
    }
    else if (type == "reflective_refractive_fuzz"){
        const char* attributes[] = {"emission", "colour", "ind", "priority", "scattering_fn", "order", "diffusivity"};
        require_attributes(xml_material, attributes, 7);
        return new ReflectiveRefractiveFuzz_t(get_colour(xml_material->Attribute("emission")), get_colour(xml_material->Attribute("colour")), std::stod(xml_material->Attribute("ind")), std::stoi(xml_material->Attribute("priority")), std::stod(xml_material->Attribute("order")), std::stod(xml_material->Attribute("diffusivity")), get_scatterer(xml_material->Attribute("scattering_fn"), xml_scatterers));
    }
    else if (type == "refractive"){
        const char* attributes[] = {"emission", "colour", "ind", "priority", "scattering_fn"};
        require_attributes(xml_material, attributes, 5);
        return new Refractive_t(get_colour(xml_material->Attribute("emission")), get_colour(xml_material->Attribute("colour")), std::stod(xml_material->Attribute("ind")), std::stoi(xml_material->Attribute("priority")), get_scatterer(xml_material->Attribute("scattering_fn"), xml_scatterers));
    }
    else if (type == "refractive_fuzz"){
        const char* attributes[] = {"emission", "colour", "ind", "priority", "scattering_fn", "order", "diffusivity"};
        require_attributes(xml_material, attributes, 7);
        return new RefractiveFuzz_t(get_colour(xml_material->Attribute("emission")), get_colour(xml_material->Attribute("colour")), std::stod(xml_material->Attribute("ind")), std::stoi(xml_material->Attribute("priority")), std::stod(xml_material->Attribute("order")), std::stod(xml_material->Attribute("diffusivity")), get_scatterer(xml_material->Attribute("scattering_fn"), xml_scatterers));
    }
    else if (type == "transparent"){
        const char* attributes[] = {"priority", "scattering_fn"};
        require_attributes(xml_material, attributes, 2);
        return new Transparent_t(std::stoi(xml_material->Attribute("priority")), get_scatterer(xml_material->Attribute("scattering_fn"), xml_scatterers));
    }
    else if (type == "toon"){
        std::cout << "Error, toon shader not implemented yet. Ignoring." << std::endl; 
        return new Diffuse_t(Vec3f(0.0, 0.0, 0.0), Vec3f(0.5, 0.5, 0.5), 1.0);
    }
    else if (type == "aggregate"){
        const char* attributes[] = {"materials_list", "materials_names"};
        require_attributes(xml_material, attributes, 2);
        materials_aggregate_list = new std::tuple<std::list<unsigned int>*, std::list<std::string>*>(get_medium_index_list(xml_material->Attribute("materials_list"), xml_materials), get_medium_names(xml_material->Attribute("materials_names")));
        return nullptr;
        // CHECK add aggregates
    }
    else{
        std::cout << "Error, material type '" << type << "' not implemented. Ignoring." << std::endl; 
        return new NormalMaterial_t();
    }
}

MeshGeometry_t* SceneContext_t::create_mesh_geometry(const tinyxml2::XMLElement* xml_mesh_geometry) const {
    std::string type;
    const char* type_char = xml_mesh_geometry->Attribute("type");
    if (type_char == nullptr) {
        std::cout << "Error: XML mesh geometries should have a 'type' attribute. Using 'mesh_geometry'." << std::endl;
        type = "mesh_geometry";
    }
    else {
        type = type_char;
    }
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);

    if (type == "mesh_geometry"){
        const char* attributes[] = {"filename"};
        require_attributes(xml_mesh_geometry, attributes, 1);
        std::string filename = xml_mesh_geometry->Attribute("filename");
        for (unsigned int i = 0; i < filename.size(); i++){
            #ifdef _WIN32
                if (filename[i] == '/') {
                    filename[i] = '\\';
                }
            #else
                if (filename[i] == '\\') {
                    filename[i] = '/';
                }
            #endif
        } 

        return new MeshGeometry_t(filename);
    }
    else{
        std::cout << "Error, mesh geometry type '" << type << "' not implemented. Only 'mesh_geometry' exists for now. Exiting." << std::endl; 
        exit(50);
    }
}

Shape_t* SceneContext_t::create_object(const tinyxml2::XMLElement* xml_object, MeshTop_t* &mesh, const tinyxml2::XMLElement* xml_transform_matrices, const tinyxml2::XMLElement* xml_materials, const tinyxml2::XMLElement* xml_mesh_geometries) {
    std::string type;
    const char* type_char = xml_object->Attribute("type");
    if (type_char == nullptr) {
        std::cout << "Error: XML objects should have a 'type' attribute. Using 'sphere'." << std::endl;
        type = "sphere";
    }
    else {
        type = type_char;
    }
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);

    if (type == "mesh"){
        const char* attributes[] = {"material", "transform_matrix", "mesh_geometry"};
        require_attributes(xml_object, attributes, 3);
        unsigned int material_index = get_material_index(xml_object->Attribute("material"), xml_materials);
        if (material_aggregates_[material_index] != nullptr){
            mesh = new Mesh_t(material_aggregates_[material_index], get_transform_matrix(xml_object->Attribute("transform_matrix"), xml_transform_matrices), get_mesh_geometry(xml_object->Attribute("mesh_geometry"), xml_mesh_geometries));
            return nullptr;
        }
        else {
            mesh = new Mesh_t(materials_[material_index], get_transform_matrix(xml_object->Attribute("transform_matrix"), xml_transform_matrices), get_mesh_geometry(xml_object->Attribute("mesh_geometry"), xml_mesh_geometries));
            return nullptr;
        }
    }
    else if (type == "mesh_motionblur"){
        const char* attributes[] = {"material", "transform_matrix", "mesh_geometry"};
        require_attributes(xml_object, attributes, 3);
        unsigned int material_index = get_material_index(xml_object->Attribute("material"), xml_materials);
        if (material_aggregates_[material_index] != nullptr){
            mesh = new MeshMotionblur_t(material_aggregates_[material_index], get_transform_matrix(xml_object->Attribute("transform_matrix"), xml_transform_matrices), get_mesh_geometry(xml_object->Attribute("mesh_geometry"), xml_mesh_geometries));
            return nullptr;
        }
        else {
            mesh = new MeshMotionblur_t(materials_[material_index], get_transform_matrix(xml_object->Attribute("transform_matrix"), xml_transform_matrices), get_mesh_geometry(xml_object->Attribute("mesh_geometry"), xml_mesh_geometries));
            return nullptr;
        }
    }
    else if (type == "sphere"){
        const char* attributes[] = {"material", "transform_matrix"};
        require_attributes(xml_object, attributes, 2);
        return new Sphere_t(get_material(xml_object->Attribute("material"), xml_materials), get_transform_matrix(xml_object->Attribute("transform_matrix"), xml_transform_matrices));
    }
    else if (type == "sphere_motionblur"){
        const char* attributes[] = {"material", "transform_matrix"};
        require_attributes(xml_object, attributes, 2);
        return new SphereMotionblur_t(get_material(xml_object->Attribute("material"), xml_materials), get_transform_matrix(xml_object->Attribute("transform_matrix"), xml_transform_matrices));
    }
    else if (type == "triangle"){
        const char* attributes[] = {"material", "transform_matrix", "points", "normals", "texture_coordinates"};
        require_attributes(xml_object, attributes, 5);
        Vec3f* points = get_points(xml_object->Attribute("points"));
        Vec3f* normals = get_points(xml_object->Attribute("normals"));
        double** texture_coordinates = get_texture_coordinates(xml_object->Attribute("texture_coordinates"));

        Shape_t* triangle = new Triangle_t(get_material(xml_object->Attribute("material"), xml_materials), get_transform_matrix(xml_object->Attribute("transform_matrix"), xml_transform_matrices), points, normals, texture_coordinates);

        if (points != nullptr){
            delete [] points;
        }
        if (normals != nullptr){
            delete [] normals;
        }
        if (texture_coordinates != nullptr){
            delete [] texture_coordinates[0];
            delete [] texture_coordinates[1];
            delete [] texture_coordinates[2];
            delete [] texture_coordinates;
        }

        return triangle;
    }
    else if (type == "triangle_motionblur"){
        const char* attributes[] = {"material", "transform_matrix", "points", "normals", "texture_coordinates"};
        require_attributes(xml_object, attributes, 5);
        Vec3f* points = get_points(xml_object->Attribute("points"));
        Vec3f* normals = get_points(xml_object->Attribute("normals"));
        double** texture_coordinates = get_texture_coordinates(xml_object->Attribute("texture_coordinates"));

        Shape_t* triangle = new TriangleMotionblur_t(get_material(xml_object->Attribute("material"), xml_materials), get_transform_matrix(xml_object->Attribute("transform_matrix"), xml_transform_matrices), points, normals, texture_coordinates);

        if (points != nullptr){
            delete [] points;
        }
        if (normals != nullptr){
            delete [] normals;
        }
        if (texture_coordinates != nullptr){
            delete [] texture_coordinates[0];
            delete [] texture_coordinates[1];
            delete [] texture_coordinates[2];
            delete [] texture_coordinates;
        }

        return triangle;
    }
    else if (type == "triangle_mesh"){
        const char* attributes[] = {"material", "transform_matrix", "mesh_geometry", "index"};
        require_attributes(xml_object, attributes, 4);
        return new TriangleMesh_t(get_material(xml_object->Attribute("material"), xml_materials), get_transform_matrix(xml_object->Attribute("transform_matrix"), xml_transform_matrices), get_mesh_geometry(xml_object->Attribute("mesh_geometry"), xml_mesh_geometries), std::stoi(xml_object->Attribute("index")));
    }
    else if (type == "triangle_mesh_motionblur"){
        const char* attributes[] = {"material", "transform_matrix", "mesh_geometry", "index"};
        require_attributes(xml_object, attributes, 4);
        return new TriangleMeshMotionblur_t(get_material(xml_object->Attribute("material"), xml_materials), get_transform_matrix(xml_object->Attribute("transform_matrix"), xml_transform_matrices), get_mesh_geometry(xml_object->Attribute("mesh_geometry"), xml_mesh_geometries), std::stoi(xml_object->Attribute("index")));
    }
    else{
        std::cout << "Error, object type '" << type << "' not implemented. Exiting." << std::endl; 
        exit(60);
    }
}

DirectionalLight_t* SceneContext_t::create_directional_light(const tinyxml2::XMLElement* xml_directional_light, const tinyxml2::XMLElement* xml_transform_matrices){
    std::string type;
    const char* type_char = xml_directional_light->Attribute("type");
    if (type_char == nullptr) {
        std::cout << "Error: XML directional lights should have a 'type' attribute. Using 'directional_light'." << std::endl;
        type = "directional_light";
    }
    else {
        type = type_char;
    }
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);

    if (type == "directional_light"){
        const char* attributes[] = {"colour", "transform_matrix"};
        require_attributes(xml_directional_light, attributes, 2);
        return new DirectionalLight_t(get_colour(xml_directional_light->Attribute("colour")), get_transform_matrix(xml_directional_light->Attribute("transform_matrix"), xml_transform_matrices));
    }
    else{
        std::cout << "Error, directional light type '" << type << "' not implemented. Only 'directional_light' exists for now. Exiting." << std::endl; 
        exit(70);
    }
}

Skybox_t* SceneContext_t::create_skybox(const tinyxml2::XMLElement* xml_skybox, const tinyxml2::XMLElement* xml_textures, const tinyxml2::XMLElement* xml_transform_matrices, const tinyxml2::XMLElement* xml_directional_lights) {
    std::string type;
    const char* type_char = xml_skybox->Attribute("type");
    if (type_char == nullptr) {
        std::cout << "Error: XML skyboxes should have a 'type' attribute. Using 'skybox_flat'." << std::endl;
        type = "skybox_flat";
    }
    else {
        type = type_char;
    }
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);

    if (type == "skybox_flat"){
        const char* attributes[] = {"colour"};
        require_attributes(xml_skybox, attributes, 1);
        return new SkyboxFlat_t(get_colour(xml_skybox->Attribute("colour")));
    }
    else if (type == "skybox_flat_sun"){
        const char* attributes[] = {"colour", "lights"};
        require_attributes(xml_skybox, attributes, 2);
        DirectionalLight_t** lights = nullptr;
        unsigned int n = 0;

        get_lights(xml_skybox->Attribute("lights"), lights, n, xml_directional_lights);

        Skybox_t* skybox = new SkyboxFlatSun_t(get_colour(xml_skybox->Attribute("colour")), lights, n);
        
        delete [] lights;        
        return skybox;
    }
    else if (type == "skybox_texture"){
        const char* attributes[] = {"texture"};
        require_attributes(xml_skybox, attributes, 1);
        return new SkyboxTexture_t(get_texture(xml_skybox->Attribute("texture"), xml_textures));
    }
    else if (type == "skybox_texture_sun"){
        const char* attributes[] = {"texture", "light_position", "light_colour", "light_radius"};
        require_attributes(xml_skybox, attributes, 4);
        double sun_pos[2];
        get_xy(xml_skybox->Attribute("light_position"), sun_pos);
        return new SkyboxTextureSun_t(get_texture(xml_skybox->Attribute("texture"), xml_textures), sun_pos, get_colour(xml_skybox->Attribute("light_colour")), std::stod(xml_skybox->Attribute("light_radius")));
    }
    else if (type == "skybox_texture_transformation"){
        const char* attributes[] = {"texture", "transform_matrix"};
        require_attributes(xml_skybox, attributes, 2);
        return new SkyboxTextureTransformation_t(get_texture(xml_skybox->Attribute("texture"), xml_textures), get_transform_matrix(xml_skybox->Attribute("transform_matrix"), xml_transform_matrices));
    }
    else if (type == "skybox_texture_transformation_sun"){
        const char* attributes[] = {"texture", "light_position", "light_colour", "light_radius", "transform_matrix"};
        require_attributes(xml_skybox, attributes, 5);
        double sun_pos[2];
        get_xy(xml_skybox->Attribute("light_position"), sun_pos);
        return new SkyboxTextureTransformationSun_t(get_texture(xml_skybox->Attribute("texture"), xml_textures), get_transform_matrix(xml_skybox->Attribute("transform_matrix"), xml_transform_matrices), sun_pos, get_colour(xml_skybox->Attribute("light_colour")), std::stod(xml_skybox->Attribute("light_radius")));
    }
    else{
        std::cout << "Error, skybox type '" << type << "' not implemented. Ignoring." << std::endl; 
        return new SkyboxFlat_t(Vec3f(0.5, 0.5, 0.5));
    }
}

ImgBuffer_t* SceneContext_t::create_imgbuffer(const tinyxml2::XMLElement* xml_imgbuffer) {
    std::string type;
    const char* type_char = xml_imgbuffer->Attribute("type");
    if (type_char == nullptr) {
        std::cout << "Error: XML imgbuffers should have a 'type' attribute. Using 'imgbuffer'." << std::endl;
        type = "imgbuffer";
    }
    else {
        type = type_char;
    }
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);

    if (type == "imgbuffer"){
        const char* attributes[] = {"resx", "resy"};
        require_attributes(xml_imgbuffer, attributes, 2);
        return new ImgBuffer_t(std::stod(xml_imgbuffer->Attribute("resx")), std::stod(xml_imgbuffer->Attribute("resy")));
    }
    else if (type == "imgbuffer_opengl"){
        const char* attributes[] = {"resx", "resy"};
        require_attributes(xml_imgbuffer, attributes, 2);
        use_gl_ = true; 
        opengl_imgbuffer_ = new ImgBufferOpenGL_t(std::stod(xml_imgbuffer->Attribute("resx")), std::stod(xml_imgbuffer->Attribute("resy")));
        return opengl_imgbuffer_;
    }
    else{
        std::cout << "Error, imgbuffer type '" << type << "' not implemented. Only 'imgbuffer', and 'imgbuffer_opengl' exist for now. Ignoring." << std::endl; 
        return new ImgBuffer_t(300, 200);
    }
}

Camera_t* SceneContext_t::create_camera(const tinyxml2::XMLElement* xml_camera, const std::string &next_filename, const tinyxml2::XMLElement* xml_transform_matrices, const tinyxml2::XMLElement* xml_materials, const tinyxml2::XMLElement* xml_imgbuffers, const tinyxml2::XMLElement* xml_skyboxes) {
    std::string type;
    const char* type_char = xml_camera->Attribute("type");
    if (type_char == nullptr) {
        std::cout << "Error: XML cameras should have a 'type' attribute. Using 'cam'." << std::endl;
        type = "cam";
    }
    else {
        type = type_char;
    }
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);
    std::string filename;
    const char* filename_char = xml_camera->Attribute("filename");
    if (filename_char == nullptr){
        std::cout << "Error: XML cameras should have a 'filename' attribute. Using 'nan'." << std::endl;
        filename = "nan";
    }
    else {
        filename = filename_char;
        for (unsigned int i = 0; i < filename.size(); i++){
            #ifdef _WIN32
                if (filename[i] == '/') {
                    filename[i] = '\\';
                }
            #else
                if (filename[i] == '\\') {
                    filename[i] = '/';
                }
            #endif
        }
    }
    std::string filename_check = filename;
    std::transform(filename_check.begin(), filename_check.end(), filename_check.begin(), ::tolower);

    if (filename_check == "nan"){
        filename = next_filename;
    }

    if (type == "cam"){
        double fov[2];
        unsigned int subpix[2];
        get_xy(xml_camera->Attribute("fov"), fov);
        get_xy(xml_camera->Attribute("subpix"), subpix);

        return new Cam_t(get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, get_colour(xml_camera->Attribute("up")), 
                            fov, subpix, get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_materials), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), std::stoi(xml_camera->Attribute("max_bounces")), std::stod(xml_camera->Attribute("gammaind")));
    }
    else if (type == "cam_aperture"){
        double fov[2];
        unsigned int subpix[2];
        get_xy(xml_camera->Attribute("fov"), fov);
        get_xy(xml_camera->Attribute("subpix"), subpix);

        return new CamAperture_t(get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, get_colour(xml_camera->Attribute("up")), 
                            fov, subpix, get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_materials), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), std::stoi(xml_camera->Attribute("max_bounces")), std::stod(xml_camera->Attribute("focal_length")),
                            std::stod(xml_camera->Attribute("aperture")), std::stod(xml_camera->Attribute("gammaind")));
    }
    else if (type == "cam_motionblur"){
        double fov[2];
        unsigned int subpix[2];
        double time[2];
        get_xy(xml_camera->Attribute("fov"), fov);
        get_xy(xml_camera->Attribute("subpix"), subpix);
        get_xy(xml_camera->Attribute("time"), time);
        
        return new CamMotionblur_t(get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, get_colour(xml_camera->Attribute("up")), 
                            fov, subpix, get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_materials), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), std::stoi(xml_camera->Attribute("max_bounces")), time, std::stod(xml_camera->Attribute("gammaind")));
    }
    else if (type == "cam_motionblur_aperture"){
        double fov[2];
        unsigned int subpix[2];
        double time[2];
        get_xy(xml_camera->Attribute("fov"), fov);
        get_xy(xml_camera->Attribute("subpix"), subpix);
        get_xy(xml_camera->Attribute("time"), time);
        
        return new CamMotionblurAperture_t(get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, get_colour(xml_camera->Attribute("up")), 
                            fov, subpix, get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_materials), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), std::stoi(xml_camera->Attribute("max_bounces")), std::stod(xml_camera->Attribute("focal_length")),
                            std::stod(xml_camera->Attribute("aperture")), time, std::stod(xml_camera->Attribute("gammaind")));
    }
    else if (type == "isocam"){
        double fov[2];
        unsigned int subpix[2];
        get_xy(xml_camera->Attribute("fov"), fov);
        get_xy(xml_camera->Attribute("subpix"), subpix);

        return new IsoCam_t(get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, get_colour(xml_camera->Attribute("up")), 
                            fov, subpix, get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_materials), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), std::stoi(xml_camera->Attribute("max_bounces")), std::stod(xml_camera->Attribute("gammaind")));
    }
    else if (type == "isocam_aperture"){
        double fov[2];
        unsigned int subpix[2];
        get_xy(xml_camera->Attribute("fov"), fov);
        get_xy(xml_camera->Attribute("subpix"), subpix);

        return new IsoCamAperture_t(get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, get_colour(xml_camera->Attribute("up")), 
                            fov, subpix, get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_materials), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), std::stoi(xml_camera->Attribute("max_bounces")), std::stod(xml_camera->Attribute("focal_length")),
                            std::stod(xml_camera->Attribute("aperture")), std::stod(xml_camera->Attribute("gammaind")));
    }
    else if (type == "isocam_motionblur"){
        double fov[2];
        unsigned int subpix[2];
        double time[2];
        get_xy(xml_camera->Attribute("fov"), fov);
        get_xy(xml_camera->Attribute("subpix"), subpix);
        get_xy(xml_camera->Attribute("time"), time);
        
        return new IsoCamMotionblur_t(get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, get_colour(xml_camera->Attribute("up")), 
                            fov, subpix, get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_materials), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), std::stoi(xml_camera->Attribute("max_bounces")), time, std::stod(xml_camera->Attribute("gammaind")));
    }
    else if (type == "isocam_motionblur_aperture"){
        double fov[2];
        unsigned int subpix[2];
        double time[2];
        get_xy(xml_camera->Attribute("fov"), fov);
        get_xy(xml_camera->Attribute("subpix"), subpix);
        get_xy(xml_camera->Attribute("time"), time);
        
        return new IsoCamMotionblurAperture_t(get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, get_colour(xml_camera->Attribute("up")), 
                            fov, subpix, get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_materials), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), std::stoi(xml_camera->Attribute("max_bounces")), std::stod(xml_camera->Attribute("focal_length")),
                            std::stod(xml_camera->Attribute("aperture")), time, std::stod(xml_camera->Attribute("gammaind")));
    }
    else if (type == "reccam"){
        double fov[2];
        unsigned int subpix[2];
        get_xy(xml_camera->Attribute("fov"), fov);
        get_xy(xml_camera->Attribute("subpix"), subpix);

        return new RecCam_t(get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, get_colour(xml_camera->Attribute("up")), 
                            fov, subpix, get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_materials), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), std::stoi(xml_camera->Attribute("max_bounces")), std::stod(xml_camera->Attribute("gammaind")));
    }
    else if (type == "reccam_aperture"){
        double fov[2];
        unsigned int subpix[2];
        get_xy(xml_camera->Attribute("fov"), fov);
        get_xy(xml_camera->Attribute("subpix"), subpix);

        return new RecCamAperture_t(get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, get_colour(xml_camera->Attribute("up")), 
                            fov, subpix, get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_materials), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), std::stoi(xml_camera->Attribute("max_bounces")), std::stod(xml_camera->Attribute("focal_length")),
                            std::stod(xml_camera->Attribute("aperture")), std::stod(xml_camera->Attribute("gammaind")));
    }
    else if (type == "reccam_motionblur"){
        double fov[2];
        unsigned int subpix[2];
        double time[2];
        get_xy(xml_camera->Attribute("fov"), fov);
        get_xy(xml_camera->Attribute("subpix"), subpix);
        get_xy(xml_camera->Attribute("time"), time);
        
        return new RecCamMotionblur_t(get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, get_colour(xml_camera->Attribute("up")), 
                            fov, subpix, get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_materials), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), std::stoi(xml_camera->Attribute("max_bounces")), time, std::stod(xml_camera->Attribute("gammaind")));
    }
    else if (type == "reccam_motionblur_aperture"){
        double fov[2];
        unsigned int subpix[2];
        double time[2];
        get_xy(xml_camera->Attribute("fov"), fov);
        get_xy(xml_camera->Attribute("subpix"), subpix);
        get_xy(xml_camera->Attribute("time"), time);
        
        return new RecCamMotionblurAperture_t(get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, get_colour(xml_camera->Attribute("up")), 
                            fov, subpix, get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_materials), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), std::stoi(xml_camera->Attribute("max_bounces")), std::stod(xml_camera->Attribute("focal_length")),
                            std::stod(xml_camera->Attribute("aperture")), time, std::stod(xml_camera->Attribute("gammaind")));
    }
    else if (type == "cam_3d"){
        double fov[2];
        unsigned int subpix[2];
        get_xy(xml_camera->Attribute("fov"), fov);
        get_xy(xml_camera->Attribute("subpix"), subpix);

        return new Cam3D_t(get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, get_colour(xml_camera->Attribute("up")), fov, subpix, 
                            get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_imgbuffer(xml_camera->Attribute("imgbuffer_L"), xml_imgbuffers), 
                            get_imgbuffer(xml_camera->Attribute("imgbuffer_R"), xml_imgbuffers), std::stod(xml_camera->Attribute("eye_dist")), 
                            get_medium_list(xml_camera->Attribute("medium_list"), xml_materials), get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), 
                            std::stoi(xml_camera->Attribute("max_bounces")), std::stod(xml_camera->Attribute("focal_length")), std::stod(xml_camera->Attribute("gammaind")));
    }
    else if (type == "cam_3d_aperture"){
        double fov[2];
        unsigned int subpix[2];
        get_xy(xml_camera->Attribute("fov"), fov);
        get_xy(xml_camera->Attribute("subpix"), subpix);

        return new Cam3DAperture_t(get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, get_colour(xml_camera->Attribute("up")), fov, subpix, 
                            get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_imgbuffer(xml_camera->Attribute("imgbuffer_L"), xml_imgbuffers), 
                            get_imgbuffer(xml_camera->Attribute("imgbuffer_R"), xml_imgbuffers), std::stod(xml_camera->Attribute("eye_dist")), 
                            get_medium_list(xml_camera->Attribute("medium_list"), xml_materials), get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), 
                            std::stoi(xml_camera->Attribute("max_bounces")), std::stod(xml_camera->Attribute("focal_length")), std::stod(xml_camera->Attribute("aperture")), 
                            std::stod(xml_camera->Attribute("gammaind")));
    }
    else if (type == "cam_3d_motionblur"){
        double fov[2];
        unsigned int subpix[2];
        double time[2];
        get_xy(xml_camera->Attribute("fov"), fov);
        get_xy(xml_camera->Attribute("subpix"), subpix);
        get_xy(xml_camera->Attribute("time"), time);

        return new Cam3DMotionblur_t(get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, get_colour(xml_camera->Attribute("up")), fov, subpix, 
                            get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_imgbuffer(xml_camera->Attribute("imgbuffer_L"), xml_imgbuffers), 
                            get_imgbuffer(xml_camera->Attribute("imgbuffer_R"), xml_imgbuffers), std::stod(xml_camera->Attribute("eye_dist")), 
                            get_medium_list(xml_camera->Attribute("medium_list"), xml_materials), get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), 
                            std::stoi(xml_camera->Attribute("max_bounces")), std::stod(xml_camera->Attribute("focal_length")), time, std::stod(xml_camera->Attribute("gammaind")));
    }
    else if (type == "cam_3d_motionblur_aperture"){
        double fov[2];
        unsigned int subpix[2];
        double time[2];
        get_xy(xml_camera->Attribute("fov"), fov);
        get_xy(xml_camera->Attribute("subpix"), subpix);
        get_xy(xml_camera->Attribute("time"), time);

        return new Cam3DMotionblurAperture_t(get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, get_colour(xml_camera->Attribute("up")), fov, subpix, 
                            get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_imgbuffer(xml_camera->Attribute("imgbuffer_L"), xml_imgbuffers), 
                            get_imgbuffer(xml_camera->Attribute("imgbuffer_R"), xml_imgbuffers), std::stod(xml_camera->Attribute("eye_dist")), 
                            get_medium_list(xml_camera->Attribute("medium_list"), xml_materials), get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), 
                            std::stoi(xml_camera->Attribute("max_bounces")), std::stod(xml_camera->Attribute("focal_length")), std::stod(xml_camera->Attribute("aperture")), 
                            time, std::stod(xml_camera->Attribute("gammaind")));
    }
    else{
        std::cout << "Error, camera type '" << type << "' not implemented. Exiting." << std::endl; 
        exit(100);
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
                std::string name_transform_matrix;
                const char* transform_matrix_char = xml_transform_matrix->Attribute("name");
                if (transform_matrix_char == nullptr){
                    name_transform_matrix = "";
                }
                else{
                    name_transform_matrix = transform_matrix_char;
                }
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

std::list<unsigned int>* SceneContext_t::get_medium_index_list(std::string string_medium_list, const tinyxml2::XMLElement* xml_materials) const {
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
                    std::string name_material;
                    const char* material_char = xml_material->Attribute("name");
                    if (material_char == nullptr){
                        name_material = "";
                    }
                    else{
                        name_material = material_char;
                    }
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
                std::string name_material;
                const char* material_char = xml_material->Attribute("name");
                if (material_char == nullptr){
                    name_material = "";
                }
                else{
                    name_material = material_char;
                }
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

std::list<Medium_t*> SceneContext_t::get_medium_list(std::string string_medium_list, const tinyxml2::XMLElement* xml_materials) const {
    std::list<Medium_t*> medium_list = std::list<Medium_t*>(); // CHECK full of dynamic casts with no checks... should maybe check for errors
    std::string delimiter = ", ";
    size_t pos = 0;
    std::string token;

    while ((pos = string_medium_list.find(delimiter)) != std::string::npos) {
        token = string_medium_list.substr(0, pos);

        if (is_number(token)) {
            Medium_t* medium = dynamic_cast<Medium_t*>(materials_[std::stoi(token) - 1]);
            if (medium == nullptr){
                std::cout << "Error: material #" << token << " is in a medium list, but it is not convertible to one. Exiting." << std::endl;
                exit(494);
            }
            medium_list.push_back(medium);
        }
        else {
            if (xml_materials != nullptr){
                unsigned int index = 0;
                for (const tinyxml2::XMLElement* xml_material = xml_materials->FirstChildElement("material"); xml_material; xml_material = xml_material->NextSiblingElement("material")){
                    std::string name_material;
                    const char* material_char = xml_material->Attribute("name");
                    if (material_char == nullptr){
                        name_material = "";
                    }
                    else{
                        name_material = material_char;
                    }
                    std::transform(name_material.begin(), name_material.end(), name_material.begin(), ::tolower);
                    if (name_material == token){
                        Medium_t* medium = dynamic_cast<Medium_t*>(materials_[index]);
                        if (medium == nullptr){
                            std::cout << "Error: material '" << token << "' is in a medium list, but it is not convertible to one. Exiting." << std::endl;
                            exit(495);
                        }
                        medium_list.push_back(medium);
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
        Medium_t* medium = dynamic_cast<Medium_t*>(materials_[std::stoi(string_medium_list) - 1]);
        if (medium == nullptr){
            std::cout << "Error: material #" << token << " is in a medium list, but it is not convertible to one. Exiting." << std::endl;
            exit(494);
        }
        medium_list.push_back(medium);
    }
    else {
        if (xml_materials != nullptr){
            unsigned int index = 0;
            for (const tinyxml2::XMLElement* xml_material = xml_materials->FirstChildElement("material"); xml_material; xml_material = xml_material->NextSiblingElement("material")){
                std::string name_material;
                const char* material_char = xml_material->Attribute("name");
                if (material_char == nullptr){
                    name_material = "";
                }
                else{
                    name_material = material_char;
                }
                std::transform(name_material.begin(), name_material.end(), name_material.begin(), ::tolower);
                if (name_material == string_medium_list){
                    Medium_t* medium = dynamic_cast<Medium_t*>(materials_[index]);
                    if (medium == nullptr){
                        std::cout << "Error: material '" << token << "' is in a medium list, but it is not convertible to one. Exiting." << std::endl;
                        exit(495);
                    }
                    medium_list.push_back(medium);
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
                std::string name_texture;
                const char* name_char = xml_texture->Attribute("name");
                if (name_char == nullptr){
                    name_texture = "";
                }
                else{
                    name_texture = name_char;
                }
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
                std::string name_material;
                const char* name_char = xml_material->Attribute("name");
                if (name_char == nullptr){
                    name_material = "";
                }
                else{
                    name_material = name_char;
                }
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
                std::string name_material;
                const char* name_char = xml_material->Attribute("name");
                if (name_char == nullptr){
                    name_material = "";
                }
                else{
                    name_material = name_char;
                }
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
                std::string name_scatterer;
                const char* name_char = xml_scatterer->Attribute("name");
                if (name_char == nullptr){
                    name_scatterer = "";
                }
                else{
                    name_scatterer = name_char;
                }
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

MeshGeometry_t* SceneContext_t::get_mesh_geometry(std::string mesh_geometry, const tinyxml2::XMLElement* xml_mesh_geometries) const {
    if (is_number(mesh_geometry)) {
        return mesh_geometries_[std::stoi(mesh_geometry) - 1];
    }
    else {
        if (xml_mesh_geometries != nullptr){
            std::transform(mesh_geometry.begin(), mesh_geometry.end(), mesh_geometry.begin(), ::tolower);
            unsigned int index = 0;
            for (const tinyxml2::XMLElement* xml_mesh_geometry = xml_mesh_geometries->FirstChildElement("mesh_geometry"); xml_mesh_geometry; xml_mesh_geometry = xml_mesh_geometry->NextSiblingElement("mesh_geometry")){
                std::string name_mesh_geometry;
                const char* name_char = xml_mesh_geometry->Attribute("name");
                if (name_char == nullptr){
                    name_mesh_geometry = "";
                }
                else{
                    name_mesh_geometry = name_char;
                }
                std::transform(name_mesh_geometry.begin(), name_mesh_geometry.end(), name_mesh_geometry.begin(), ::tolower);
                if (name_mesh_geometry == mesh_geometry){
                    return mesh_geometries_[index];
                }
                ++index;
            }
        }
    }
    std::cout << "Error, mesh geometry '" << mesh_geometry << "' not found. Exiting." << std::endl;
    exit(51);
}

unsigned int SceneContext_t::get_material_index(std::string material, const tinyxml2::XMLElement* xml_materials) const {
    if (is_number(material)) {
        return std::stoi(material) - 1;
    }
    else {
        if (xml_materials != nullptr){
            std::transform(material.begin(), material.end(), material.begin(), ::tolower);
            unsigned int index = 0;
            for (const tinyxml2::XMLElement* xml_material = xml_materials->FirstChildElement("material"); xml_material; xml_material = xml_material->NextSiblingElement("material")){
                std::string name_material;
                const char* name_char = xml_material->Attribute("name");
                if (name_char == nullptr){
                    name_material = "";
                }
                else{
                    name_material = name_char;
                }
                std::transform(name_material.begin(), name_material.end(), name_material.begin(), ::tolower);
                if (name_material == material){
                    return index;
                }
                ++index;
            }
        }
    }
    std::cout << "Error, material '" << material << "' not found. Exiting." << std::endl;
    exit(41);
}

Material_t* SceneContext_t::get_material(std::string material, const tinyxml2::XMLElement* xml_materials) const {
    if (is_number(material)) {
        return materials_[std::stoi(material) - 1];
    }
    else {
        if (xml_materials != nullptr){
            std::transform(material.begin(), material.end(), material.begin(), ::tolower);
            unsigned int index = 0;
            for (const tinyxml2::XMLElement* xml_material = xml_materials->FirstChildElement("material"); xml_material; xml_material = xml_material->NextSiblingElement("material")){
                std::string name_material;
                const char* name_char = xml_material->Attribute("name");
                if (name_char == nullptr){
                    name_material = "";
                }
                else{
                    name_material = name_char;
                }
                std::transform(name_material.begin(), name_material.end(), name_material.begin(), ::tolower);
                if (name_material == material){
                    return materials_[index];
                }
                ++index;
            }
        }
    }
    std::cout << "Error, material '" << material << "' not found. Exiting." << std::endl;
    exit(41);
}

void SceneContext_t::get_lights(std::string lights_string, DirectionalLight_t** &lights, unsigned int &n, const tinyxml2::XMLElement* xml_directional_lights) const {
    std::list<DirectionalLight_t*> lights_list = std::list<DirectionalLight_t*>();
    std::string delimiter = ", ";
    size_t pos = 0;
    std::string token;

    while ((pos = lights_string.find(delimiter)) != std::string::npos) {
        token = lights_string.substr(0, pos);

        if (is_number(token)) {
            lights_list.push_back(directional_lights_[std::stoi(token) - 1]);
        }
        else {
            if (xml_directional_lights != nullptr){
                unsigned int index = 0;
                for (const tinyxml2::XMLElement* xml_directional_light = xml_directional_lights->FirstChildElement("directional_light"); xml_directional_light; xml_directional_light = xml_directional_light->NextSiblingElement("directional_light")){
                    std::string name_light;
                    const char* name_char = xml_directional_light->Attribute("name");
                    if (name_char == nullptr){
                        name_light = "";
                    }
                    else{
                        name_light = name_char;
                    }
                    std::transform(name_light.begin(), name_light.end(), name_light.begin(), ::tolower);
                    if (name_light == token){
                        lights_list.push_back(directional_lights_[index]);
                        break;
                    }
                    ++index;
                }
            }
        }
        // CHECK this should check for errors.

        lights_string.erase(0, pos + delimiter.length());
    }
    if (is_number(lights_string)) {
        lights_list.push_back(directional_lights_[std::stoi(lights_string) - 1]);
    }
    else {
        if (xml_directional_lights != nullptr){
            unsigned int index = 0;
            for (const tinyxml2::XMLElement* xml_directional_light = xml_directional_lights->FirstChildElement("directional_light"); xml_directional_light; xml_directional_light = xml_directional_light->NextSiblingElement("directional_light")){
                std::string name_light;
                const char* name_char = xml_directional_light->Attribute("name");
                if (name_char == nullptr){
                    name_light = "";
                }
                else{
                    name_light = name_char;
                }
                std::transform(name_light.begin(), name_light.end(), name_light.begin(), ::tolower);
                if (name_light == lights_string){
                    lights_list.push_back(directional_lights_[index]);
                    break;
                }
                ++index;
            }
        }
    }

    if (lights_list.size() > 0){
        n = lights_list.size();
        lights = new DirectionalLight_t*[n];
        unsigned int index = 0;

        for (auto it = lights_list.begin(); it != lights_list.end(); ++it){
            lights[index] = *it;
            ++index;
        }
    }
}

ImgBuffer_t* SceneContext_t::get_imgbuffer(std::string imgbuffer, const tinyxml2::XMLElement* xml_imgbuffers) const {
    if (is_number(imgbuffer)) {
        return imgbuffers_[std::stoi(imgbuffer) - 1];
    }
    else {
        if (xml_imgbuffers != nullptr){
            std::transform(imgbuffer.begin(), imgbuffer.end(), imgbuffer.begin(), ::tolower);
            unsigned int index = 0;
            for (const tinyxml2::XMLElement* xml_imgbuffer = xml_imgbuffers->FirstChildElement("imgbuffer"); xml_imgbuffer; xml_imgbuffer = xml_imgbuffer->NextSiblingElement("imgbuffer")){
                std::string name_imgbuffer;
                const char* name_char = xml_imgbuffer->Attribute("name");
                if (name_char == nullptr){
                    name_imgbuffer = "";
                }
                else{
                    name_imgbuffer = name_char;
                }
                std::transform(name_imgbuffer.begin(), name_imgbuffer.end(), name_imgbuffer.begin(), ::tolower);
                if (name_imgbuffer == imgbuffer){
                    return imgbuffers_[index];
                }
                ++index;
            }
        }
    }
    std::cout << "Error, image buffer '" << imgbuffer << "' not found. Exiting." << std::endl;
    exit(91); 
}

Skybox_t* SceneContext_t::get_skybox(std::string skybox, const tinyxml2::XMLElement* xml_skyboxes) const {
    if (is_number(skybox)) {
        return skyboxes_[std::stoi(skybox) - 1];
    }
    else {
        if (xml_skyboxes != nullptr){
            std::transform(skybox.begin(), skybox.end(), skybox.begin(), ::tolower);
            unsigned int index = 0;
            for (const tinyxml2::XMLElement* xml_skybox = xml_skyboxes->FirstChildElement("skybox"); xml_skybox; xml_skybox = xml_skybox->NextSiblingElement("skybox")){
                std::string name_skybox;
                const char* name_char = xml_skybox->Attribute("name");
                if (name_char == nullptr){
                    name_skybox = "";
                }
                else{
                    name_skybox = name_char;
                }
                std::transform(name_skybox.begin(), name_skybox.end(), name_skybox.begin(), ::tolower);
                if (name_skybox == skybox){
                    return skyboxes_[index];
                }
                ++index;
            }
        }
    }
    std::cout << "Error, skybox '" << skybox << "' not found. Exiting." << std::endl;
    exit(81); 
}

void SceneContext_t::get_objects(std::string objects_string, Shape_t** &shapes, unsigned int &n_shapes, MeshTop_t** &meshes, unsigned int &n_meshes, const tinyxml2::XMLElement* xml_objects) const {
    std::list<unsigned int> objects_list = std::list<unsigned int>();
    std::string delimiter = ", ";
    size_t pos = 0;
    std::string token;
    n_shapes = 0;
    n_meshes = 0;

    while ((pos = objects_string.find(delimiter)) != std::string::npos) {
        token = objects_string.substr(0, pos);

        if (is_number(token)) {
            objects_list.push_back(std::stoi(token) - 1);
        }
        else {
            if (xml_objects != nullptr){
                unsigned int index = 0;
                for (const tinyxml2::XMLElement* xml_object = xml_objects->FirstChildElement("object"); xml_object; xml_object = xml_object->NextSiblingElement("object")){
                    std::string name_object;
                    const char* name_char = xml_object->Attribute("name");
                    if (name_char == nullptr){
                        name_object = "";
                    }
                    else{
                        name_object = name_char;
                    }
                    std::transform(name_object.begin(), name_object.end(), name_object.begin(), ::tolower);
                    if (name_object == token){
                        objects_list.push_back(index);
                        break;
                    }
                    ++index;
                }
            }
        }
        // CHECK this should check for errors.

        objects_string.erase(0, pos + delimiter.length());
    }
    if (is_number(objects_string)) {
        objects_list.push_back(std::stoi(objects_string) - 1);
    }
    else {
        if (xml_objects != nullptr){
            unsigned int index = 0;
            for (const tinyxml2::XMLElement* xml_object = xml_objects->FirstChildElement("object"); xml_object; xml_object = xml_object->NextSiblingElement("object")){
                std::string name_object;
                const char* name_char = xml_object->Attribute("name");
                if (name_char == nullptr){
                    name_object = "";
                }
                else{
                    name_object = name_char;
                }
                std::transform(name_object.begin(), name_object.end(), name_object.begin(), ::tolower);
                if (name_object == objects_string){
                    objects_list.push_back(index);
                    break;
                }
                ++index;
            }
        }
    }

    if (objects_list.size() > 0){
        for (auto it = objects_list.begin(); it != objects_list.end(); ++it){
            if (objects_[*it] != nullptr){
                ++n_shapes;
            }
            else {
                ++n_meshes;
            }
        }

        shapes = new Shape_t*[n_shapes];
        meshes = new MeshTop_t*[n_meshes];
        unsigned int index_shapes = 0;
        unsigned int index_meshes = 0;

        for (auto it = objects_list.begin(); it != objects_list.end(); ++it){
            if (objects_[*it] != nullptr){
                shapes[index_shapes] = objects_[*it];
                ++index_shapes;
            }
            else {
                meshes[index_meshes] = meshes_[*it];
                ++index_meshes;
            }
        }
    }
}

Vec3f get_colour(std::string colour) {
    std::transform(colour.begin(), colour.end(), colour.begin(), ::tolower);
    
    auto it = my_colours::colours.find(colour);
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

        if (count == 1) {
            return Vec3f(values[0], values[0], values[0]);
        }
        else if (count != 3) {
            std::cout << "Error, colour should be 1 or 3 values seperated by spaces, or a string. Current number of values is " << count << ", colour is '" << colour << "'. Ignoring." << std::endl;
            return Vec3f(0.5, 0.5, 0.5);
        }
        else{
            return Vec3f(values[0], values[1], values[2]);
        }
	}
}

Vec3f* get_points(std::string points_string) {
    Vec3f* points = nullptr;
    std::transform(points_string.begin(), points_string.end(), points_string.begin(), ::tolower);

    if (points_string != "nan"){
        points = new Vec3f[3];
        double values[9];
        unsigned int count = 0;
        std::stringstream ss(points_string);
        
        for(std::string s; ss >> s; ){
            if (count < 9){
                values[count] = std::stod(s);
            }
            ++count;
        }
        if (count != 9) {
            std::cout << "Error, triangle points should be 9 values seperated by spaces, or nan. Current number of values is " << count << ". Exiting." << std::endl;
            exit(67);
        }
        else{
            points[0][0] = values[0];
            points[0][1] = values[1];
            points[0][2] = values[2];
            points[1][0] = values[3];
            points[1][1] = values[4];
            points[1][2] = values[5];
            points[2][0] = values[6];
            points[2][1] = values[7];
            points[2][2] = values[8];
        }
    } 

    return points;
}

double** get_texture_coordinates(std::string texture_coordinates_string) {
    double** texture_coordinates = nullptr;

    std::transform(texture_coordinates_string.begin(), texture_coordinates_string.end(), texture_coordinates_string.begin(), ::tolower);

    if (texture_coordinates_string != "nan"){
        texture_coordinates = new double*[3];
        texture_coordinates[0] = new double[2];
        texture_coordinates[1] = new double[2];
        texture_coordinates[2] = new double[2];
        double values[6];
        unsigned int count = 0;
        std::stringstream ss(texture_coordinates_string);
        
        for(std::string s; ss >> s; ){
            if (count < 6){
                values[count] = std::stod(s);
            }
            ++count;
        }
        if (count != 6) {
            std::cout << "Error, triangle texture coordinates should be 6 values seperated by spaces, or nan. Current number of values is " << count << ". Exiting." << std::endl;
            exit(68);
        }
        else{
            texture_coordinates[0][0] = values[0];
            texture_coordinates[0][1] = values[1];
            texture_coordinates[1][0] = values[2];
            texture_coordinates[1][1] = values[3];
            texture_coordinates[2][0] = values[4];
            texture_coordinates[2][1] = values[5];
        }
    }

    return texture_coordinates;
}

std::list<std::string>* get_medium_names(std::string string_medium_names) {
    std::list<std::string>* medium_names = new std::list<std::string>();
    std::string delimiter = ", ";
    size_t pos = 0;

    while ((pos = string_medium_names.find(delimiter)) != std::string::npos) {
        medium_names->push_back(string_medium_names.substr(0, pos));
        string_medium_names.erase(0, pos + delimiter.length());
    }
    medium_names->push_back(string_medium_names);

    return medium_names;
}

void get_xy(const std::string &string_value, double (&value)[2]) {
    unsigned int count = 0;
    std::stringstream ss(string_value);

    for(std::string s; ss >> s; ){
        if (count < 2){
            value[count] = std::stod(s);
        }
        ++count;
    }

    if (count != 2) {
        std::cout << "Error, xy should be 2 values seperated by spaces. Current number of values is " << count << ", string is '" << string_value << "'. Ignoring." << std::endl;
    }
}

void get_xy(const std::string &string_value, unsigned int (&value)[2]) {
    unsigned int count = 0;
    std::stringstream ss(string_value);

    for(std::string s; ss >> s; ){
        if (count < 2){
            value[count] = std::stoi(s);
        }
        ++count;
    }

    if (count != 2) {
        std::cout << "Error, xy should be 2 values seperated by spaces. Current number of values is " << count << ", string is '" << string_value << "'. Ignoring." << std::endl;
    }
}

bool is_number(const std::string& s) {
    return !s.empty() && std::find_if(s.begin(), 
        s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

void apply_transformation(TransformMatrix_t* transform_matrix, const tinyxml2::XMLElement* transform) {
    std::string type;
    const char* type_char = transform->Attribute("type");
    if (type_char == nullptr) {
        std::cout << "Error: XML transforms should have a 'type' attribute. Ignoring." << std::endl;
        return;
    }
    else {
        type = type_char;
    }
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);

    if (type == "rotatexaxis"){
        transform_matrix->rotateXAxis(std::stod(transform->Attribute("value")));
    }
    else if (type == "rotateyaxis"){
        transform_matrix->rotateYAxis(std::stod(transform->Attribute("value")));
    }
    else if (type == "rotatezaxis"){
        transform_matrix->rotateZAxis(std::stod(transform->Attribute("value")));
    }
    else if (type == "rotatex"){
        transform_matrix->rotateX(std::stod(transform->Attribute("value")));
    }
    else if (type == "rotatey"){
        transform_matrix->rotateY(std::stod(transform->Attribute("value")));
    }
    else if (type == "rotatez"){
        transform_matrix->rotateZ(std::stod(transform->Attribute("value")));
    }
    else if (type == "rotateaxis"){
        transform_matrix->rotateAxis(get_colour(transform->Attribute("axis")), std::stod(transform->Attribute("value")));
    }
    else if (type == "rotate"){
        transform_matrix->rotate(get_colour(transform->Attribute("axis")), std::stod(transform->Attribute("value")));
    }
    else if (type == "translate"){
        transform_matrix->translate(get_colour(transform->Attribute("value")));
    }
    else if (type == "scaleaxis"){
        transform_matrix->scaleAxis(get_colour(transform->Attribute("value")));
    }
    else if (type == "uniformscaleaxis"){
        transform_matrix->scaleAxis(std::stod(transform->Attribute("value")));
    }
    else if (type == "scale"){
        transform_matrix->scale(get_colour(transform->Attribute("value")));
    }
    else if (type == "uniformscale"){
        transform_matrix->scale(std::stod(transform->Attribute("value")));
    }
    else if (type == "reflect"){
        transform_matrix->reflect(get_colour(transform->Attribute("value")));
    }
    else if (type == "shear"){
        transform_matrix->shear(get_colour(transform->Attribute("value")));
    }
    else if (type == "transpose"){
        transform_matrix->transpose();
    }
    else if (type == "invert"){
        transform_matrix->invert();
    }
    else if (type == "neg"){
        transform_matrix->neg();
    }
    else{
        std::cout << "Error, transform type '" << type << "' not implemented. Ignoring." << std::endl; 
    }
}

void require_attributes(const tinyxml2::XMLElement* element, const char** attributes, unsigned int n) {
    bool missing = false;

    for (unsigned int i = 0; i < n; ++i){
        const char* value = element->Attribute(attributes[i]);
        if (value == nullptr){
            if (!missing){
                missing = true;
                const char* name_char = element->Attribute("name");
                const char* type_type = element->Attribute("type");
                std::string name = (name_char == nullptr) ? "" : name_char;
                std::string type = (type_type == nullptr) ? "" : type_type;
                std::cout << "Error: " << element->Name() << " XML element with name '" << name << "' and type '" << type << "' has the following attributes missing:" << std::endl;
            }
        }
        std::cout << attributes[i] << std::endl;
    }

    if (missing) {
        exit(2); 
    }
}