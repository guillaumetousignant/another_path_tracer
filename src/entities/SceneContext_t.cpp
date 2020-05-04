#include "entities/SceneContext_t.h"

#include <iostream>
#include <string>
#include <sstream> 
#include <tuple>
#include <algorithm>
#include <chrono>

#include "functions/Colours.h"
#include "functions/NextFilename.h"
#include "entities/OpenGLRenderer_t.h"

#include "materials/Diffuse_t.h"
#include "materials/DiffuseNormal_t.h"
#include "materials/DiffuseFull_t.h"
#include "materials/DiffuseTex_t.h"
#include "materials/DiffuseTexNormal_t.h"
#include "materials/Reflective_t.h"
#include "materials/ReflectiveNormal_t.h"
#include "materials/ReflectiveFuzz_t.h"
#include "materials/ReflectiveFuzzNormal_t.h"
#include "materials/ReflectiveFuzzTex_t.h"
#include "materials/ReflectiveFuzzTexNormal_t.h"
#include "materials/ReflectiveRefractive_t.h"
#include "materials/ReflectiveRefractiveNormal_t.h"
#include "materials/ReflectiveRefractiveFuzz_t.h"
#include "materials/Refractive_t.h"
#include "materials/RefractiveFuzz_t.h"
#include "materials/Transparent_t.h"
#include "materials/PortalTop_t.h"
#include "materials/Portal_t.h"
#include "materials/NormalMaterial_t.h"
#include "materials/NormalDiffuseMaterial_t.h"
#include "entities/MaterialMix_t.h"
#include "materials/FresnelMix_t.h"
#include "materials/FresnelMixNormal_t.h"
#include "materials/FresnelMixIn_t.h"
#include "materials/RandomMix_t.h"
#include "materials/RandomMixIn_t.h"
#include "materials/BounceMaterial_t.h"
#include "materials/DistanceMaterial_t.h"
#include "materials/Toon_t.h"
#include "entities/MaterialMap_t.h"

#include "materials/NonAbsorber_t.h"
#include "materials/Absorber_t.h"
#include "materials/Scatterer_t.h"
#include "materials/ScattererExp_t.h"
#include "materials/ScattererFull_t.h"
#include "materials/ScattererExpFull_t.h"
#include "materials/PortalScattererTop_t.h"
#include "materials/PortalScatterer_t.h"

#include "entities/TransformMatrix_t.h"
#include "entities/ImgBuffer_t.h"
#include "entities/ImgBufferOpenGL_t.h"
#include "entities/Scene_t.h"
#include "entities/MeshGeometry_t.h"
#include "entities/DirectionalLight_t.h"
#include "entities/Texture_t.h"

#include "shapes/Sphere_t.h"
#include "shapes/SphereMotionblur_t.h"
#include "shapes/Triangle_t.h"
#include "shapes/TriangleMotionblur_t.h"
#include "shapes/TriangleMesh_t.h"
#include "shapes/TriangleMeshMotionblur_t.h"
#include "shapes/Mesh_t.h"
#include "shapes/MeshMotionblur_t.h"

#include "entities/Camera_t.h"
#include "cameras/Cam_t.h"
#include "cameras/CamAperture_t.h"
#include "cameras/CamMotionblur_t.h"
#include "cameras/CamMotionblurAperture_t.h"
#include "cameras/IsoCam_t.h"
#include "cameras/IsoCamAperture_t.h"
#include "cameras/IsoCamMotionblur_t.h"
#include "cameras/IsoCamMotionblurAperture_t.h"
#include "cameras/RecCam_t.h"
#include "cameras/RecCamAperture_t.h"
#include "cameras/RecCamMotionblur_t.h"
#include "cameras/RecCamMotionblurAperture_t.h"
#include "cameras/Cam3D_t.h"
#include "cameras/Cam3DAperture_t.h"
#include "cameras/Cam3DMotionblur_t.h"
#include "cameras/Cam3DMotionblurAperture_t.h"

#include "skyboxes/SkyboxFlat_t.h"
#include "skyboxes/SkyboxFlatSun_t.h"
#include "skyboxes/SkyboxTexture_t.h"
#include "skyboxes/SkyboxTextureSun_t.h"
#include "skyboxes/SkyboxTextureTransformation_t.h"
#include "skyboxes/SkyboxTextureTransformationSun_t.h"

#include "acceleration/AccelerationGrid_t.h"
#include "acceleration/AccelerationGridArray_t.h"
#include "acceleration/AccelerationGridVector_t.h"
#include "acceleration/AccelerationMultiGrid_t.h"
#include "acceleration/AccelerationMultiGridArray_t.h"
#include "acceleration/AccelerationMultiGridVector_t.h"

using namespace APTracer::Materials;
using namespace APTracer::Shapes;
using namespace APTracer::Cameras;
using namespace APTracer::Skyboxes;
using namespace APTracer::Acceleration;

APTracer::Entities::SceneContext_t::SceneContext_t() :
    use_gl_(false), scene_name_(""), opengl_renderer_(), opengl_imgbuffer_(nullptr), 
    opengl_camera_(nullptr), scene_(), camera_rendermode_(), camera_n_iter_(), camera_write_interval_(), 
    index_transform_matrices_(0), index_textures_(0), index_mediums_(0), index_materials_(0), 
    index_mesh_geometries_(0), index_objects_(0), index_directional_lights_(0), index_skyboxes_(0), 
    index_imgbuffers_(0), index_cameras_(0), transform_matrices_(), textures_(), 
    mediums_(), materials_(), mesh_geometries_(), objects_(), 
    directional_lights_(), skyboxes_(), imgbuffers_(), cameras_(), 
    material_aggregates_(), meshes_() 
    {}

APTracer::Entities::SceneContext_t::~SceneContext_t() {
    reset();
}

void APTracer::Entities::SceneContext_t::readXML(const std::string &filename){
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
    tinyxml2::XMLElement* xml_mediums = xml_top->FirstChildElement("mediums");
    tinyxml2::XMLElement* xml_materials = xml_top->FirstChildElement("materials");
    tinyxml2::XMLElement* xml_mesh_geometries = xml_top->FirstChildElement("mesh_geometries");
    tinyxml2::XMLElement* xml_objects = xml_top->FirstChildElement("objects");
    tinyxml2::XMLElement* xml_directional_lights = xml_top->FirstChildElement("directional_lights");
    tinyxml2::XMLElement* xml_skyboxes = xml_top->FirstChildElement("skyboxes");
    tinyxml2::XMLElement* xml_imgbuffers = xml_top->FirstChildElement("imgbuffers");
    tinyxml2::XMLElement* xml_cameras = xml_top->FirstChildElement("cameras");
    tinyxml2::XMLElement* xml_acceleration_structures = xml_top->FirstChildElement("acceleration_structures");

    unsigned int n_transform_matrices = 0;
    unsigned int n_textures = 0;
    unsigned int n_mediums = 0;
    unsigned int n_materials = 0;
    unsigned int n_mesh_geometries = 0;
    unsigned int n_objects = 0;
    unsigned int n_directional_lights = 0;
    unsigned int n_skyboxes = 0;
    unsigned int n_imgbuffers = 0;
    unsigned int n_cameras = 0;

    std::vector<std::unique_ptr<std::list<unsigned int>>> mediums_medium_list;
    std::vector<std::vector<unsigned int>> materials_mix_list;
    std::vector<std::unique_ptr<std::list<unsigned int>>> materials_medium_list;
    std::vector<
        std::unique_ptr<std::tuple<
            std::unique_ptr<std::list<unsigned int>>, 
            std::unique_ptr<std::list<std::string>>
        >>
    > materials_aggregate_list;            
    
    // Counts
    if (xml_transform_matrices != nullptr){
        for (tinyxml2::XMLElement* xml_transform_matrix = xml_transform_matrices->FirstChildElement("transform_matrix"); xml_transform_matrix; xml_transform_matrix = xml_transform_matrix->NextSiblingElement("transform_matrix")){
            ++n_transform_matrices;
        }
    }

    if (xml_textures != nullptr){
        for (tinyxml2::XMLElement* xml_texture = xml_textures->FirstChildElement("texture"); xml_texture; xml_texture = xml_texture->NextSiblingElement("texture")){
            ++n_textures;
        }
    }

    if (xml_mediums != nullptr){
        for (tinyxml2::XMLElement* xml_medium = xml_mediums->FirstChildElement("medium"); xml_medium; xml_medium = xml_medium->NextSiblingElement("medium")){
            ++n_mediums;
            const char* char_transform_matrix = xml_medium->Attribute("transform_matrix");
            if (char_transform_matrix != nullptr){
                std::string string_transform_matrix = char_transform_matrix;
                std::transform(string_transform_matrix.begin(), string_transform_matrix.end(), string_transform_matrix.begin(), ::tolower);
                if (string_transform_matrix == "nan"){
                    ++n_transform_matrices;
                }
            }
        }
    }

    if (xml_materials != nullptr){
        for (tinyxml2::XMLElement* xml_material = xml_materials->FirstChildElement("material"); xml_material; xml_material = xml_material->NextSiblingElement("material")){
            ++n_materials;
            const char* char_transform_matrix = xml_material->Attribute("transform_matrix");
            if (char_transform_matrix != nullptr){
                std::string string_transform_matrix = char_transform_matrix;
                std::transform(string_transform_matrix.begin(), string_transform_matrix.end(), string_transform_matrix.begin(), ::tolower);
                if (string_transform_matrix == "nan"){
                    ++n_transform_matrices;
                }
            }
        }
    }

    if (xml_mesh_geometries != nullptr){
        for (tinyxml2::XMLElement* xml_mesh_geometry = xml_mesh_geometries->FirstChildElement("mesh_geometry"); xml_mesh_geometry; xml_mesh_geometry = xml_mesh_geometry->NextSiblingElement("mesh_geometry")){
            ++n_mesh_geometries;
        }
    }

    if (xml_objects != nullptr){
        for (tinyxml2::XMLElement* xml_object = xml_objects->FirstChildElement("object"); xml_object; xml_object = xml_object->NextSiblingElement("object")){
            ++n_objects;
            const char* char_transform_matrix = xml_object->Attribute("transform_matrix");
            if (char_transform_matrix != nullptr){
                std::string string_transform_matrix = char_transform_matrix;
                std::transform(string_transform_matrix.begin(), string_transform_matrix.end(), string_transform_matrix.begin(), ::tolower);
                if (string_transform_matrix == "nan"){
                    ++n_transform_matrices;
                }
            }
        }
    }

    if (xml_directional_lights != nullptr){
        for (tinyxml2::XMLElement* xml_directional_light = xml_directional_lights->FirstChildElement("directional_light"); xml_directional_light; xml_directional_light = xml_directional_light->NextSiblingElement("directional_light")){
            ++n_directional_lights;
            const char* char_transform_matrix = xml_directional_light->Attribute("transform_matrix");
            if (char_transform_matrix != nullptr){
                std::string string_transform_matrix = char_transform_matrix;
                std::transform(string_transform_matrix.begin(), string_transform_matrix.end(), string_transform_matrix.begin(), ::tolower);
                if (string_transform_matrix == "nan"){
                    ++n_transform_matrices;
                }
            }
        }
    }

    if (xml_skyboxes != nullptr){
        for (tinyxml2::XMLElement* xml_skybox = xml_skyboxes->FirstChildElement("skybox"); xml_skybox; xml_skybox = xml_skybox->NextSiblingElement("skybox")){
            ++n_skyboxes;
            const char* char_transform_matrix = xml_skybox->Attribute("transform_matrix");
            if (char_transform_matrix != nullptr){
                std::string string_transform_matrix = char_transform_matrix;
                std::transform(string_transform_matrix.begin(), string_transform_matrix.end(), string_transform_matrix.begin(), ::tolower);
                if (string_transform_matrix == "nan"){
                    ++n_transform_matrices;
                }
            }
        }
    }

    if (xml_imgbuffers != nullptr){
        for (tinyxml2::XMLElement* xml_imgbuffer = xml_imgbuffers->FirstChildElement("imgbuffer"); xml_imgbuffer; xml_imgbuffer = xml_imgbuffer->NextSiblingElement("imgbuffer")){
            ++n_imgbuffers;
        }
    }

    if (xml_cameras != nullptr){
        for (tinyxml2::XMLElement* xml_camera = xml_cameras->FirstChildElement("camera"); xml_camera; xml_camera = xml_camera->NextSiblingElement("camera")){
            ++n_cameras;
            const char* char_transform_matrix = xml_camera->Attribute("transform_matrix");
            if (char_transform_matrix != nullptr){
                std::string string_transform_matrix = char_transform_matrix;
                std::transform(string_transform_matrix.begin(), string_transform_matrix.end(), string_transform_matrix.begin(), ::tolower);
                if (string_transform_matrix == "nan"){
                    ++n_transform_matrices;
                }
            }
        }
    }

    std::cout << std::endl;
    std::cout << "Transform matrix count: " << n_transform_matrices << std::endl;
    std::cout << "Texture count: " << n_textures << std::endl;
    std::cout << "Medium count: " << n_mediums << std::endl;
    std::cout << "Material count: " << n_materials << std::endl;
    std::cout << "Mesh count: " << n_mesh_geometries << std::endl;
    std::cout << "Shape count: " << n_objects << std::endl;
    std::cout << "Directional light count: " << n_directional_lights << std::endl;
    std::cout << "Skybox count: " << n_skyboxes << std::endl;
    std::cout << "Img buffers count: " << n_imgbuffers << std::endl;
    std::cout << "Camera count: " << n_cameras << std::endl << std::endl;

    // Buffer creation    
    transform_matrices_ = std::vector<std::unique_ptr<TransformMatrix_t>>(n_transform_matrices);
    textures_ = std::vector<std::unique_ptr<Texture_t>>(n_textures);
    mediums_ = std::vector<std::unique_ptr<Medium_t>>(n_mediums);
    materials_ = std::vector<std::unique_ptr<Material_t>>(n_materials);
    material_aggregates_ = std::vector<std::unique_ptr<MaterialMap_t>>(n_materials);
    mesh_geometries_ = std::vector<std::unique_ptr<MeshGeometry_t>>(n_mesh_geometries);
    objects_ = std::vector<std::unique_ptr<Shape_t>>(n_objects);
    meshes_ = std::vector<std::unique_ptr<MeshTop_t>>(n_objects);
    directional_lights_ = std::vector<std::unique_ptr<DirectionalLight_t>>(n_directional_lights);
    skyboxes_ = std::vector<std::unique_ptr<Skybox_t>>(n_skyboxes);
    imgbuffers_ = std::vector<std::unique_ptr<ImgBuffer_t>>(n_imgbuffers);
    cameras_ = std::vector<std::unique_ptr<Camera_t>>(n_cameras);
 
    mediums_medium_list = std::vector<std::unique_ptr<std::list<unsigned int>>>(n_mediums);     
    materials_mix_list = std::vector<std::vector<unsigned int>>(n_materials);
    materials_medium_list = std::vector<std::unique_ptr<std::list<unsigned int>>>(n_materials);
    materials_aggregate_list = std::vector<
                                    std::unique_ptr<std::tuple<
                                        std::unique_ptr<std::list<unsigned int>>, 
                                        std::unique_ptr<std::list<std::string>>
                                    >>
                                >(n_materials);

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

    // Mediums (3)
    if (xml_mediums != nullptr){
        for (tinyxml2::XMLElement* xml_medium = xml_mediums->FirstChildElement("medium"); xml_medium; xml_medium = xml_medium->NextSiblingElement("medium")){
            mediums_[index_mediums_] = create_medium(xml_medium, mediums_medium_list[index_mediums_], xml_transform_matrices, xml_mediums);
            ++index_mediums_;
        }
        std::cout << "Mediums created." << std::endl;
    }

    // Materials (4)
    if (xml_materials != nullptr){
        for (tinyxml2::XMLElement* xml_material = xml_materials->FirstChildElement("material"); xml_material; xml_material = xml_material->NextSiblingElement("material")){
            materials_[index_materials_] = create_material(xml_material, materials_medium_list[index_materials_], materials_mix_list[index_materials_], materials_aggregate_list[index_materials_], xml_textures, xml_transform_matrices, xml_materials, xml_mediums);
            ++index_materials_;
        }
        std::cout << "Materials created." << std::endl;
    }

    // Fixes 
    // Material mixes fix
    for (unsigned int i = 0; i < materials_.size(); i++){
        if (!materials_mix_list[i].empty()){
            MaterialMix_t* material_mix = dynamic_cast<MaterialMix_t*>(materials_[i].get()); // dynamic caaaast :(
            if (material_mix == nullptr){
                std::cerr << "Error: material #" << i << " was marked as a material mix but is not convertible to one. Exiting." << std::endl;
                exit(491);
            }
            material_mix->material_refracted_ = materials_[materials_mix_list[i][0]].get();
            material_mix->material_reflected_ = materials_[materials_mix_list[i][1]].get();
        }
    }

    // Materials medium list fix
    for (unsigned int i = 0; i < materials_.size(); i++){
        if (materials_medium_list[i]) {
            PortalTop_t* portal = dynamic_cast<PortalTop_t*>(materials_[i].get());
            if (portal == nullptr){
                std::cerr << "Error: material #" << i << " was marked as a portal but is not convertible to one. Exiting." << std::endl;
                exit(492);
            }
            for (auto it = materials_medium_list[i]->begin(); it != materials_medium_list[i]->end(); ++it){
                portal->medium_list_.push_back(mediums_[*it].get());
            }
        }
    }

    // Mediums medium list fix
    for (unsigned int i = 0; i < mediums_.size(); i++){
        if (mediums_medium_list[i]) {
            PortalScattererTop_t* portal_scatterer = dynamic_cast<PortalScattererTop_t*>(mediums_[i].get());
            if (portal_scatterer == nullptr){
                std::cerr << "Error: medium #" << i << " was marked as a portal but is not convertible to one. Exiting." << std::endl;
                exit(392);
            }
            for (auto it = mediums_medium_list[i]->begin(); it != mediums_medium_list[i]->end(); ++it){
                portal_scatterer->medium_list_.push_back(mediums_[*it].get());
            }
        }
    }

    // Material aggregates fix
    for (unsigned int i = 0; i < materials_.size(); i++){
        if (materials_aggregate_list[i]) {
            unsigned int n = std::get<0>(*materials_aggregate_list[i])->size();
            std::vector<std::string> names(n);
            std::vector<Material_t*> materials(n);

            unsigned int index = 0;
            for (auto it = std::get<0>(*materials_aggregate_list[i])->begin(); it != std::get<0>(*materials_aggregate_list[i])->end(); ++it){
                materials[index] = materials_[*it].get();
                ++index;
            }
            index = 0;
            for (auto it = std::get<1>(*materials_aggregate_list[i])->begin(); it != std::get<1>(*materials_aggregate_list[i])->end(); ++it){
                names[index] = *it;
                ++index;
            }
            
            material_aggregates_[i] = std::unique_ptr<MaterialMap_t>(new MaterialMap_t(names.data(), materials.data(), n));
            //materials_[i] = std::unique_ptr<Material_t>(material_aggregates_[i]->getFirst()); // CHECK this is not intended, will delete stuff from material list
            materials_[i] = std::unique_ptr<Material_t>();
        }
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
            cameras_[index_cameras_] = create_camera(xml_camera, new_filename, xml_transform_matrices, xml_mediums, xml_imgbuffers, xml_skyboxes);
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
                    apply_transformation(transform_matrices_[index].get(), transformation_pre);
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
                PortalTop_t* portal = dynamic_cast<PortalTop_t*>(materials_[index].get());
                if (portal == nullptr){
                    std::cerr << "Error, material #" << index << " has transformations, but it is not convertible to a portal. Ignoring." << std::endl;
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
                TransformMatrix_t* transform_matrix = (objects_[index]) ? objects_[index]->transformation_ : meshes_[index]->transformation_;
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
    scene_ = std::unique_ptr<Scene_t>(new Scene_t());
    const char* object_list = xml_top->Attribute("object_list");
    std::cout << "Scene created." << std::endl;
    
    if (objects_.size() > 0){
        std::vector<Shape_t*> shapes;
        std::vector<MeshTop_t*> meshes;

        if (object_list != nullptr){
            get_objects(object_list, shapes, meshes, xml_objects);
        }
        else {
            get_objects(shapes, meshes);
        }

        scene_->add(shapes.data(), shapes.size());
        scene_->add(meshes.data(), meshes.size());

        std::cout << "Primitives added." << std::endl;
    }

    // Update
    std::cout << std::endl << "Updating scene..." << std::endl;
    auto t_start = std::chrono::high_resolution_clock::now();
    scene_->update();
    for (auto &camera : cameras_){
        camera->update();
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
                    apply_transformation(transform_matrices_[index].get(), transformation_post);
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
                PortalTop_t* portal = dynamic_cast<PortalTop_t*>(materials_[index].get());
                if (portal == nullptr){
                    std::cerr << "Error, material #" << index << " has transformations, but it is not convertible to a portal. Ignoring." << std::endl;
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
                TransformMatrix_t* transform_matrix = (objects_[index]) ? objects_[index]->transformation_ : meshes_[index]->transformation_;
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
    tinyxml2::XMLElement* xml_acceleration_structure;
    if (xml_acceleration_structures != nullptr){
        xml_acceleration_structure = xml_acceleration_structures->FirstChildElement("acceleration_structure");
              
    }
    else {
        xml_acceleration_structure = nullptr;
    }

    t_start = std::chrono::high_resolution_clock::now();
    create_acceleration_structure(xml_acceleration_structure);
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
                        std::cerr << "Error: Cameras with nan as 'focal_length' attribute should have a 'focus_position' attribute. Using 0.5 0.5." << std::endl;
                        focus_position = "0.5 0.5";
                    }
                    else {
                        focus_position = focus_position_char;
                    }
                    get_xy(focus_position, position);
                    cameras_[index]->autoFocus(scene_.get(), position);
                    std::cout << "Camera #" << index << " autofocus." << std::endl;
                }
            }
            ++index;
        }
    }
    for (auto &camera : cameras_){
        camera->update();
    }

    // Running modes
    if (xml_cameras != nullptr){
        if (use_gl_) {
            opengl_camera_ = cameras_[0].get(); // CHECK dunno how to fix this
            opengl_renderer_ = std::unique_ptr<OpenGLRenderer_t>(new OpenGLRenderer_t(scene_.get(), opengl_camera_, opengl_imgbuffer_));
            opengl_renderer_->initialise();
            std::cout << "OpenGL initialised." << std::endl;
        }

        camera_rendermode_ = std::vector<std::string>(cameras_.size(), "");
        camera_n_iter_ = std::vector<unsigned int>(cameras_.size(), 0);
        camera_write_interval_ = std::vector<unsigned int>(cameras_.size(), 0);

        unsigned int index = 0;
        for (tinyxml2::XMLElement* xml_camera = xml_cameras->FirstChildElement("camera"); xml_camera; xml_camera = xml_camera->NextSiblingElement("camera")){
            std::string render_mode;
            const char* render_mode_char = xml_camera->Attribute("rendermode");
            if (render_mode_char == nullptr){
                std::cerr << "Error: XML cameras should have a 'rendermode' attribute. Using 'single'." << std::endl;
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
                    std::cerr << "Error: XML cameras in accumulation mode should have a 'n_iter' attribute. Using 1." << std::endl;
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
                    std::cerr << "Error: XML cameras in accumulation mode should have a 'n_iter' attribute. Using 1." << std::endl;
                    n_iter = "1";
                }
                else {
                    n_iter = n_iter_char;
                }
                if (write_interval_char == nullptr){
                    std::cerr << "Error: XML cameras in accumulation_write mode should have a 'write_interval' attribute. Using 1." << std::endl;
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
                std::cerr << "Error, motion render mode not implemented yet. Single frame render fallback." << std::endl;
                camera_rendermode_[index] = "single";
            }
            else {
                std::cerr << "Error, render mode '" << render_mode << "', used by camera #" << index << ", is unknown. Only 'accumulation', 'accumulation_write', 'single', and 'motion' exist for now. Ignoring." << std::endl;
                camera_rendermode_[index] = "";
            }

            ++index;
        }
        std::cout << "Camera render modes set." << std::endl;
    }
    std::cout << std::endl << "ReadXML done." << std::endl << std::endl;
}    

void APTracer::Entities::SceneContext_t::render(){
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
            std::cerr << "Error, motion render mode not implemented yet. Accumulation render fallback." << std::endl;
            opengl_renderer_->render();
        }
        else if (render_mode == "") {

        }
        else {
            std::cerr << "Error, render mode '" << render_mode << "', used by camera #" << 0 << ", is unknown. Only 'accumulation', 'accumulation_write', 'single', and 'motion' exist for now. Ignoring." << std::endl;
        }
    }
    else {
        for (unsigned int i = 0; i < cameras_.size(); i++){
            std::string render_mode = camera_rendermode_[i];
            std::transform(render_mode.begin(), render_mode.end(), render_mode.begin(), ::tolower);

            std::cout << "Camera #" << i << " rendering scene '" << scene_name_ << "' in '" <<  render_mode << "' mode." << std::endl;

            if (render_mode == "accumulation") {
                cameras_[i]->accumulate(scene_.get(), camera_n_iter_[i]);
                cameras_[i]->write();
            }
            else if (render_mode == "accumulation_write") {
                cameras_[i]->accumulateWrite(scene_.get(), camera_n_iter_[i], camera_write_interval_[i]);
                cameras_[i]->write();
            }
            else if (render_mode == "single") {
                cameras_[i]->raytrace(scene_.get());
                cameras_[i]->write();
            }
            else if (render_mode == "motion") {
                std::cerr << "Error, motion render mode not implemented yet. Single frame render fallback." << std::endl;
                cameras_[i]->raytrace(scene_.get());
                cameras_[i]->write();
            }
            else if (render_mode == "") {

            }
            else {
                std::cerr << "Error, render mode '" << render_mode << "', used by camera #" << i << ", is unknown. Only 'accumulation', 'accumulation_write', 'single', and 'motion' exist for now. Ignoring." << std::endl;
            }
        }
    }
}

void APTracer::Entities::SceneContext_t::reset(){
    index_transform_matrices_ = 0;
    index_textures_ = 0;
    index_mediums_ = 0;
    index_materials_ = 0;
    index_mesh_geometries_ = 0;
    index_objects_ = 0;
    index_directional_lights_ = 0;
    index_skyboxes_ = 0;
    index_imgbuffers_ = 0;
    index_cameras_ = 0;
    opengl_imgbuffer_ = nullptr;
    opengl_camera_ = nullptr;

    use_gl_ = false;
    scene_name_ = "";
}

std::unique_ptr<TransformMatrix_t> APTracer::Entities::SceneContext_t::create_transform_matrix(const tinyxml2::XMLElement* xml_transform_matrix) const {
    std::string string_transform_matrix;
    const char* transform_matrix_char = xml_transform_matrix->Attribute("value");
    if (transform_matrix_char == nullptr){
        std::cerr << "Error: XML transform matrices should have a 'value' attribute. Using identity." << std::endl;
        string_transform_matrix = "nan";
    }
    else {
        string_transform_matrix = transform_matrix_char;
    }

    std::transform(string_transform_matrix.begin(), string_transform_matrix.end(), string_transform_matrix.begin(), ::tolower);

    if (string_transform_matrix == "nan"){
        return std::unique_ptr<TransformMatrix_t>(new TransformMatrix_t());
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
            std::cerr << "Error, transform matrix value should be 16 values seperated by spaces, or nan. Current number of values is " << count << ". Ignoring." << std::endl;
            return std::unique_ptr<TransformMatrix_t>(new TransformMatrix_t());
        }
        else{
            return std::unique_ptr<TransformMatrix_t>(
                  new TransformMatrix_t(values[0], values[1], values[2], values[3],
                                        values[4], values[5], values[6], values[7],
                                        values[8], values[9], values[10], values[11],
                                        values[12], values[13], values[14], values[15]));
        }
    }
}

std::unique_ptr<Texture_t> APTracer::Entities::SceneContext_t::create_texture(const tinyxml2::XMLElement* xml_texture) const {
    std::string type;
    const char* type_char = xml_texture->Attribute("type");
    if (type_char == nullptr) {
        std::cerr << "Error: XML textures should have a 'type' attribute. Using 'texture'." << std::endl;
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
        
        return std::unique_ptr<Texture_t>(new Texture_t(filename));
    }
    else{
        std::cerr << "Error, texture type '" << type << "' not implemented. Only 'texture' exists for now. Exiting." << std::endl; 
        exit(20);
    }
}

std::unique_ptr<Medium_t> APTracer::Entities::SceneContext_t::create_medium(const tinyxml2::XMLElement* xml_medium, std::unique_ptr<std::list<unsigned int>> &mediums_medium_list, const tinyxml2::XMLElement* xml_transform_matrices, const tinyxml2::XMLElement* xml_mediums) {
    std::string type;
    const char* type_char = xml_medium->Attribute("type");
    if (type_char == nullptr) {
        std::cerr << "Error: XML mediums should have a 'type' attribute. Using 'nonabsorber'." << std::endl;
        type = "nonabsorber";
    }
    else {
        type = type_char;
    }
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);

    if (type == "absorber"){
        const char* attributes[] = {"emission", "colour", "emission_distance", "absorption_distance", "ind", "priority"};
        require_attributes(xml_medium, attributes, 6);
        return std::unique_ptr<Medium_t>(
                    new Absorber_t(APTracer::get_colour(xml_medium->Attribute("emission")), APTracer::get_colour(xml_medium->Attribute("colour")), 
                                xml_medium->DoubleAttribute("emission_distance"), xml_medium->DoubleAttribute("absorption_distance"),
                                xml_medium->DoubleAttribute("ind"), xml_medium->UnsignedAttribute("priority")));
    }
    else if (type == "nonabsorber"){
        const char* attributes[] = {"ind", "priority"};
        require_attributes(xml_medium, attributes, 2);
        return std::unique_ptr<Medium_t>(new NonAbsorber_t(xml_medium->DoubleAttribute("ind"), xml_medium->UnsignedAttribute("priority")));
    }
    else if (type == "portal_scatterer"){
        // CHECK add medium_list stuff
        const char* attributes[] = {"medium_list", "transform_matrix", "scattering_distance", "ind", "priority"};
        require_attributes(xml_medium, attributes, 5);
        mediums_medium_list = get_medium_index_list(xml_medium->Attribute("medium_list"), xml_mediums);
        return std::unique_ptr<Medium_t>(
                    new PortalScatterer_t(get_transform_matrix(xml_medium->Attribute("transform_matrix"), xml_transform_matrices), xml_medium->DoubleAttribute("scattering_distance"), std::list<Medium_t*>(),
                                xml_medium->DoubleAttribute("ind"), xml_medium->UnsignedAttribute("priority")));
    }
    else if (type == "scatterer_exp"){
        const char* attributes[] = {"emission", "colour", "emission_distance", "absorption_distance", "scattering_distance", "order", "scattering_angle", "ind", "priority"};
        require_attributes(xml_medium, attributes, 9);
        return std::unique_ptr<Medium_t>(
                    new ScattererExp_t(APTracer::get_colour(xml_medium->Attribute("emission")), APTracer::get_colour(xml_medium->Attribute("colour")),
                                xml_medium->DoubleAttribute("emission_distance"), xml_medium->DoubleAttribute("absorption_distance"),
                                xml_medium->DoubleAttribute("scattering_distance"), xml_medium->DoubleAttribute("order"), 
                                xml_medium->DoubleAttribute("scattering_angle"), xml_medium->DoubleAttribute("ind"), 
                                xml_medium->UnsignedAttribute("priority")));
    }
    else if (type == "scatterer_exp_full"){
        const char* attributes[] = {"emission", "colour", "scattering_emission", "scattering_colour", "emission_distance", "absorption_distance", "scattering_distance", "order", "scattering_angle", "ind", "priority"};
        require_attributes(xml_medium, attributes, 11);
        return std::unique_ptr<Medium_t>(
                    new ScattererExpFull_t(APTracer::get_colour(xml_medium->Attribute("emission")), APTracer::get_colour(xml_medium->Attribute("colour")),
                                APTracer::get_colour(xml_medium->Attribute("scattering_emission")), APTracer::get_colour(xml_medium->Attribute("scattering_colour")),
                                xml_medium->DoubleAttribute("emission_distance"), xml_medium->DoubleAttribute("absorption_distance"),
                                xml_medium->DoubleAttribute("scattering_distance"), xml_medium->DoubleAttribute("order"), 
                                xml_medium->DoubleAttribute("scattering_angle"), xml_medium->DoubleAttribute("ind"), 
                                xml_medium->UnsignedAttribute("priority")));
    }
    else if (type == "scatterer"){
        const char* attributes[] = {"emission", "colour", "emission_distance", "absorption_distance", "scattering_distance", "ind", "priority"};
        require_attributes(xml_medium, attributes, 7);
        return std::unique_ptr<Medium_t>(
                    new Scatterer_t(APTracer::get_colour(xml_medium->Attribute("emission")), APTracer::get_colour(xml_medium->Attribute("colour")),
                                xml_medium->DoubleAttribute("emission_distance"), xml_medium->DoubleAttribute("absorption_distance"),
                                xml_medium->DoubleAttribute("scattering_distance"), xml_medium->DoubleAttribute("ind"), 
                                xml_medium->UnsignedAttribute("priority")));
    }
    else if (type == "scatterer_full"){
        const char* attributes[] = {"emission", "colour", "scattering_emission", "scattering_colour", "emission_distance", "absorption_distance", "scattering_distance", "ind", "priority"};
        require_attributes(xml_medium, attributes, 9);
        return std::unique_ptr<Medium_t>(
                    new ScattererFull_t(APTracer::get_colour(xml_medium->Attribute("emission")), APTracer::get_colour(xml_medium->Attribute("colour")),
                                APTracer::get_colour(xml_medium->Attribute("scattering_emission")), APTracer::get_colour(xml_medium->Attribute("scattering_colour")),
                                xml_medium->DoubleAttribute("emission_distance"), xml_medium->DoubleAttribute("absorption_distance"),
                                xml_medium->DoubleAttribute("scattering_distance"), xml_medium->DoubleAttribute("ind"), 
                                xml_medium->UnsignedAttribute("priority")));
    }
    else{
        std::cerr << "Error, medium type '" << type << "' not implemented. Only 'absorber', 'nonabsorber', 'portal_scatterer', 'scatterer_exp', and 'scatterer' exists for now. Ignoring." << std::endl; 
        return std::unique_ptr<Medium_t>(new NonAbsorber_t(1.0, 0));
    }
}

std::unique_ptr<Material_t> APTracer::Entities::SceneContext_t::create_material(const tinyxml2::XMLElement* xml_material, std::unique_ptr<std::list<unsigned int>> &materials_medium_list, std::vector<unsigned int> &materials_mix_list, std::unique_ptr<std::tuple<std::unique_ptr<std::list<unsigned int>>, std::unique_ptr<std::list<std::string>>>> &materials_aggregate_list, const tinyxml2::XMLElement* xml_textures, const tinyxml2::XMLElement* xml_transform_matrices, const tinyxml2::XMLElement* xml_materials, const tinyxml2::XMLElement* xml_mediums) {
    std::string type;
    const char* type_char = xml_material->Attribute("type");
    if (type_char == nullptr) {
        std::cerr << "Error: XML materials should have a 'type' attribute. Using 'normal_material'." << std::endl;
        type = "normal_material";
    }
    else {
        type = type_char;
    }
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);

    if (type == "diffuse"){
        const char* attributes[] = {"emission", "colour", "roughness"};
        require_attributes(xml_material, attributes, 3);
        return std::unique_ptr<Material_t>(
                    new Diffuse_t(APTracer::get_colour(xml_material->Attribute("emission")), APTracer::get_colour(xml_material->Attribute("colour")), 
                                xml_material->DoubleAttribute("roughness")));
    }
    else if (type == "diffuse_normal"){
        const char* attributes[] = {"emission", "colour", "normal_map", "roughness"};
        require_attributes(xml_material, attributes, 4);
        return std::unique_ptr<Material_t>(
                    new DiffuseNormal_t(APTracer::get_colour(xml_material->Attribute("emission")), APTracer::get_colour(xml_material->Attribute("colour")), 
                                get_texture(xml_material->Attribute("normal_map"), xml_textures), xml_material->DoubleAttribute("roughness")));
    }
    else if (type == "diffuse_full"){
        const char* attributes[] = {"emission_map", "texture", "roughness"};
        require_attributes(xml_material, attributes, 3);
        return std::unique_ptr<Material_t>(
                    new DiffuseFull_t(get_texture(xml_material->Attribute("emission_map"), xml_textures), get_texture(xml_material->Attribute("texture"), xml_textures), 
                                xml_material->DoubleAttribute("roughness")));
    }
    else if (type == "diffuse_tex"){
        const char* attributes[] = {"emission", "texture", "roughness"};
        require_attributes(xml_material, attributes, 3);
        return std::unique_ptr<Material_t>(
                    new DiffuseTex_t(APTracer::get_colour(xml_material->Attribute("emission")), get_texture(xml_material->Attribute("texture"), xml_textures), 
                                xml_material->DoubleAttribute("roughness")));
    }
    else if (type == "diffuse_tex_normal"){
        const char* attributes[] = {"emission", "texture", "normal_map", "roughness"};
        require_attributes(xml_material, attributes, 4);
        return std::unique_ptr<Material_t>(
                    new DiffuseTexNormal_t(APTracer::get_colour(xml_material->Attribute("emission")), get_texture(xml_material->Attribute("texture"), xml_textures), 
                                get_texture(xml_material->Attribute("normal_map"), xml_textures), xml_material->DoubleAttribute("roughness")));
    }
    else if (type == "fresnelmix"){
        const char* attributes[] = {"material_refracted", "material_reflected", "ind"};
        require_attributes(xml_material, attributes, 3);
        materials_mix_list = get_material_mix(xml_material->Attribute("material_refracted"), xml_material->Attribute("material_reflected"), xml_materials);
        return std::unique_ptr<Material_t>(
                    new FresnelMix_t(nullptr, nullptr, xml_material->DoubleAttribute("ind")));
    }
    else if (type == "fresnelmix_normal"){
        const char* attributes[] = {"material_refracted", "material_reflected", "ind", "normal_map"};
        require_attributes(xml_material, attributes, 4);
        materials_mix_list = get_material_mix(xml_material->Attribute("material_refracted"), xml_material->Attribute("material_reflected"), xml_materials);
        return std::unique_ptr<Material_t>(
                    new FresnelMixNormal_t(nullptr, nullptr, xml_material->DoubleAttribute("ind"), get_texture(xml_material->Attribute("normal_map"), xml_textures)));
    }
    else if (type == "fresnelmix_in"){
        const char* attributes[] = {"material_refracted", "material_reflected", "ind"};
        require_attributes(xml_material, attributes, 3);
        materials_mix_list = get_material_mix(xml_material->Attribute("material_refracted"), xml_material->Attribute("material_reflected"), xml_materials);
        return std::unique_ptr<Material_t>(
                    new FresnelMixIn_t(nullptr, nullptr, xml_material->DoubleAttribute("ind")));
    }
    else if (type == "normal_material"){
        return std::unique_ptr<Material_t>(new NormalMaterial_t());
    }
    else if (type == "normal_diffuse_material"){
        const char* attributes[] = {"roughness"};
        require_attributes(xml_material, attributes, 1);
        return std::unique_ptr<Material_t>(new NormalDiffuseMaterial_t(xml_material->DoubleAttribute("roughness")));
    }
    else if (type == "portal"){
        const char* attributes[] = {"medium_list", "transform_matrix"};
        require_attributes(xml_material, attributes, 2);
        materials_medium_list = get_medium_index_list(xml_material->Attribute("medium_list"), xml_mediums);
        return std::unique_ptr<Material_t>(
                    new Portal_t(get_transform_matrix(xml_material->Attribute("transform_matrix"), xml_transform_matrices), std::list<Medium_t*>()));
    }
    else if (type == "portal_refractive"){
        std::cerr << "Error, refractive portal not implemented yet. Ignoring." << std::endl; 
        return std::unique_ptr<Material_t>(new Diffuse_t(Vec3f(), Vec3f(0.5), 1.0));
    }
    else if (type == "randommix"){
        const char* attributes[] = {"material_refracted", "material_reflected", "ratio"};
        require_attributes(xml_material, attributes, 3);
        materials_mix_list = get_material_mix(xml_material->Attribute("material_refracted"), xml_material->Attribute("material_reflected"), xml_materials);
        return std::unique_ptr<Material_t>(new RandomMix_t(nullptr, nullptr, xml_material->DoubleAttribute("ratio")));
    }
    else if (type == "randommix_in"){
        const char* attributes[] = {"material_refracted", "material_reflected", "ratio"};
        require_attributes(xml_material, attributes, 3);
        materials_mix_list = get_material_mix(xml_material->Attribute("material_refracted"), xml_material->Attribute("material_reflected"), xml_materials);
        return std::unique_ptr<Material_t>(new RandomMixIn_t(nullptr, nullptr, xml_material->DoubleAttribute("ratio")));
    }
    else if (type == "reflective"){
        const char* attributes[] = {"emission", "colour"};
        require_attributes(xml_material, attributes, 2);
        return std::unique_ptr<Material_t>(
                    new Reflective_t(APTracer::get_colour(xml_material->Attribute("emission")), APTracer::get_colour(xml_material->Attribute("colour"))));
    }
    else if (type == "reflective_normal"){
        const char* attributes[] = {"emission", "colour", "normal_map"};
        require_attributes(xml_material, attributes, 3);
        return std::unique_ptr<Material_t>(
                    new ReflectiveNormal_t(APTracer::get_colour(xml_material->Attribute("emission")), APTracer::get_colour(xml_material->Attribute("colour")), get_texture(xml_material->Attribute("normal_map"), xml_textures)));
    }
    else if (type == "reflective_fuzz"){
        const char* attributes[] = {"emission", "colour", "order", "diffusivity"};
        require_attributes(xml_material, attributes, 4);
        return std::unique_ptr<Material_t>(
                    new ReflectiveFuzz_t(APTracer::get_colour(xml_material->Attribute("emission")), APTracer::get_colour(xml_material->Attribute("colour")), xml_material->DoubleAttribute("order"), xml_material->DoubleAttribute("diffusivity")));
    }
    else if (type == "reflective_fuzz_normal"){
        const char* attributes[] = {"emission", "colour", "normal_map", "order", "diffusivity"};
        require_attributes(xml_material, attributes, 5);
        return std::unique_ptr<Material_t>(
                    new ReflectiveFuzzNormal_t(APTracer::get_colour(xml_material->Attribute("emission")), APTracer::get_colour(xml_material->Attribute("colour")), 
                    get_texture(xml_material->Attribute("normal_map"), xml_textures), xml_material->DoubleAttribute("order"), xml_material->DoubleAttribute("diffusivity")));
    }
    else if (type == "reflective_fuzz_tex"){
        const char* attributes[] = {"emission", "texture", "order", "diffusivity"};
        require_attributes(xml_material, attributes, 4);
        return std::unique_ptr<Material_t>(
                    new ReflectiveFuzzTex_t(APTracer::get_colour(xml_material->Attribute("emission")), get_texture(xml_material->Attribute("texture"), xml_textures), xml_material->DoubleAttribute("order"), xml_material->DoubleAttribute("diffusivity")));
    }
    else if (type == "reflective_fuzz_tex_normal"){
        const char* attributes[] = {"emission", "texture", "normal_map", "order", "diffusivity"};
        require_attributes(xml_material, attributes, 5);
        return std::unique_ptr<Material_t>(
                    new ReflectiveFuzzTexNormal_t(APTracer::get_colour(xml_material->Attribute("emission")), get_texture(xml_material->Attribute("texture"), xml_textures), 
                    get_texture(xml_material->Attribute("normal_map"), xml_textures), xml_material->DoubleAttribute("order"), xml_material->DoubleAttribute("diffusivity")));
    }
    else if (type == "reflective_refractive"){
        const char* attributes[] = {"emission", "colour", "medium"};
        require_attributes(xml_material, attributes, 3);
        return std::unique_ptr<Material_t>(
                    new ReflectiveRefractive_t(APTracer::get_colour(xml_material->Attribute("emission")), APTracer::get_colour(xml_material->Attribute("colour")), get_medium(xml_material->Attribute("medium"), xml_mediums)));
    }
    else if (type == "reflective_refractive_normal"){
        const char* attributes[] = {"emission", "colour", "normal_map", "medium"};
        require_attributes(xml_material, attributes, 4);
        return std::unique_ptr<Material_t>(
                    new ReflectiveRefractiveNormal_t(APTracer::get_colour(xml_material->Attribute("emission")), APTracer::get_colour(xml_material->Attribute("colour")), get_texture(xml_material->Attribute("normal_map"), xml_textures),
                    get_medium(xml_material->Attribute("medium"), xml_mediums)));
    }
    else if (type == "reflective_refractive_fuzz"){
        const char* attributes[] = {"emission", "colour", "medium", "order", "diffusivity"};
        require_attributes(xml_material, attributes, 5);
        return std::unique_ptr<Material_t>(
                    new ReflectiveRefractiveFuzz_t(APTracer::get_colour(xml_material->Attribute("emission")), APTracer::get_colour(xml_material->Attribute("colour")), xml_material->DoubleAttribute("order"), xml_material->DoubleAttribute("diffusivity"), get_medium(xml_material->Attribute("medium"), xml_mediums)));
    }
    else if (type == "refractive"){
        const char* attributes[] = {"emission", "colour", "medium"};
        require_attributes(xml_material, attributes, 3);
        return std::unique_ptr<Material_t>(
                    new Refractive_t(APTracer::get_colour(xml_material->Attribute("emission")), APTracer::get_colour(xml_material->Attribute("colour")), get_medium(xml_material->Attribute("medium"), xml_mediums)));
    }
    else if (type == "refractive_fuzz"){
        const char* attributes[] = {"emission", "colour", "medium", "order", "diffusivity"};
        require_attributes(xml_material, attributes, 5);
        return std::unique_ptr<Material_t>(
                    new RefractiveFuzz_t(APTracer::get_colour(xml_material->Attribute("emission")), APTracer::get_colour(xml_material->Attribute("colour")), xml_material->DoubleAttribute("order"), xml_material->DoubleAttribute("diffusivity"), get_medium(xml_material->Attribute("medium"), xml_mediums)));
    }
    else if (type == "transparent"){
        const char* attributes[] = {"medium"};
        require_attributes(xml_material, attributes, 1);
        return std::unique_ptr<Material_t>(
                    new Transparent_t(get_medium(xml_material->Attribute("medium"), xml_mediums)));
    }
    else if (type == "bounce_material"){
        const char* attributes[] = {"max_bounces"};
        require_attributes(xml_material, attributes, 1);
        return std::unique_ptr<Material_t>(
                    new BounceMaterial_t(xml_material->UnsignedAttribute("max_bounces")));
    }
    else if (type == "distance_material"){
        const char* attributes[] = {"focal_length", "exponent"};
        require_attributes(xml_material, attributes, 2);
        return std::unique_ptr<Material_t>(
                    new DistanceMaterial_t(xml_material->DoubleAttribute("focal_length"), xml_material->DoubleAttribute("exponent")));
    }
    else if (type == "toon"){
        const char* attributes[] = {"colour"};
        require_attributes(xml_material, attributes, 1);
        return std::unique_ptr<Material_t>(
                    new Toon_t(APTracer::get_colour(xml_material->Attribute("colour"))));
    }
    else if (type == "aggregate"){
        const char* attributes[] = {"materials_list", "materials_names"};
        require_attributes(xml_material, attributes, 2);
        materials_aggregate_list = std::unique_ptr<std::tuple<std::unique_ptr<std::list<unsigned int>>, std::unique_ptr<std::list<std::string>>>>(
                    new std::tuple<std::unique_ptr<std::list<unsigned int>>, std::unique_ptr<std::list<std::string>>>(get_material_index_list(xml_material->Attribute("materials_list"), xml_materials), get_medium_names(xml_material->Attribute("materials_names")))); // wtf
        return std::unique_ptr<Material_t>();
        // CHECK add aggregates
    }
    else{
        std::cerr << "Error, material type '" << type << "' not implemented. Ignoring." << std::endl; 
        return std::unique_ptr<Material_t>(new NormalMaterial_t());
    }
}

std::unique_ptr<MeshGeometry_t> APTracer::Entities::SceneContext_t::create_mesh_geometry(const tinyxml2::XMLElement* xml_mesh_geometry) const {
    std::string type;
    const char* type_char = xml_mesh_geometry->Attribute("type");
    if (type_char == nullptr) {
        std::cerr << "Error: XML mesh geometries should have a 'type' attribute. Using 'mesh_geometry'." << std::endl;
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

        return std::unique_ptr<MeshGeometry_t>(new MeshGeometry_t(filename));
    }
    else{
        std::cerr << "Error, mesh geometry type '" << type << "' not implemented. Only 'mesh_geometry' exists for now. Exiting." << std::endl; 
        exit(50);
    }
}

std::unique_ptr<Shape_t> APTracer::Entities::SceneContext_t::create_object(const tinyxml2::XMLElement* xml_object, std::unique_ptr<MeshTop_t> &mesh, const tinyxml2::XMLElement* xml_transform_matrices, const tinyxml2::XMLElement* xml_materials, const tinyxml2::XMLElement* xml_mesh_geometries) {
    std::string type;
    const char* type_char = xml_object->Attribute("type");
    if (type_char == nullptr) {
        std::cerr << "Error: XML objects should have a 'type' attribute. Using 'sphere'." << std::endl;
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
        if (material_aggregates_[material_index]){
            mesh = std::unique_ptr<MeshTop_t>(
                        new Mesh_t(material_aggregates_[material_index].get(), get_transform_matrix(xml_object->Attribute("transform_matrix"), xml_transform_matrices), get_mesh_geometry(xml_object->Attribute("mesh_geometry"), xml_mesh_geometries)));
            return nullptr;
        }
        else {
            mesh = std::unique_ptr<MeshTop_t>(
                        new Mesh_t(materials_[material_index].get(), get_transform_matrix(xml_object->Attribute("transform_matrix"), xml_transform_matrices), get_mesh_geometry(xml_object->Attribute("mesh_geometry"), xml_mesh_geometries)));
            return nullptr;
        }
    }
    else if (type == "mesh_motionblur"){
        const char* attributes[] = {"material", "transform_matrix", "mesh_geometry"};
        require_attributes(xml_object, attributes, 3);
        unsigned int material_index = get_material_index(xml_object->Attribute("material"), xml_materials);
        if (material_aggregates_[material_index]){
            mesh = std::unique_ptr<MeshTop_t>(
                        new MeshMotionblur_t(material_aggregates_[material_index].get(), get_transform_matrix(xml_object->Attribute("transform_matrix"), xml_transform_matrices), get_mesh_geometry(xml_object->Attribute("mesh_geometry"), xml_mesh_geometries)));
            return nullptr;
        }
        else {
            mesh = std::unique_ptr<MeshTop_t>(
                        new MeshMotionblur_t(materials_[material_index].get(), get_transform_matrix(xml_object->Attribute("transform_matrix"), xml_transform_matrices), get_mesh_geometry(xml_object->Attribute("mesh_geometry"), xml_mesh_geometries)));
            return nullptr;
        }
    }
    else if (type == "sphere"){
        const char* attributes[] = {"material", "transform_matrix"};
        require_attributes(xml_object, attributes, 2);
        return std::unique_ptr<Shape_t>(
                    new Sphere_t(get_material(xml_object->Attribute("material"), xml_materials), get_transform_matrix(xml_object->Attribute("transform_matrix"), xml_transform_matrices)));
    }
    else if (type == "sphere_motionblur"){
        const char* attributes[] = {"material", "transform_matrix"};
        require_attributes(xml_object, attributes, 2);
        return std::unique_ptr<Shape_t>(
                    new SphereMotionblur_t(get_material(xml_object->Attribute("material"), xml_materials), get_transform_matrix(xml_object->Attribute("transform_matrix"), xml_transform_matrices)));
    }
    else if (type == "triangle"){
        const char* attributes[] = {"material", "transform_matrix", "points", "normals", "texture_coordinates"};
        require_attributes(xml_object, attributes, 5);
        std::vector<Vec3f> points = get_points(xml_object->Attribute("points"));
        std::vector<Vec3f> normals = get_points(xml_object->Attribute("normals"));
        std::vector<std::vector<double>> texture_coordinates = get_texture_coordinates(xml_object->Attribute("texture_coordinates"));

        Vec3f* points_ptr = points.empty() ? nullptr : points.data();
        Vec3f* normals_ptr = normals.empty() ? nullptr : normals.data();

        double** texture_coordinates_ptr;
        double* texture_coordinates_data[3];
        if (texture_coordinates.empty()){
            texture_coordinates_ptr = nullptr;
        }
        else {
            texture_coordinates_data[0] = texture_coordinates[0].data();
            texture_coordinates_data[1] = texture_coordinates[1].data();
            texture_coordinates_data[2] = texture_coordinates[2].data();
            texture_coordinates_ptr = &(texture_coordinates_data[0]);
        }

        return std::unique_ptr<Shape_t>(
                    new Triangle_t(get_material(xml_object->Attribute("material"), xml_materials), get_transform_matrix(xml_object->Attribute("transform_matrix"), xml_transform_matrices), points_ptr, normals_ptr, texture_coordinates_ptr));
    }
    else if (type == "triangle_motionblur"){
        const char* attributes[] = {"material", "transform_matrix", "points", "normals", "texture_coordinates"};
        require_attributes(xml_object, attributes, 5);
        std::vector<Vec3f> points = get_points(xml_object->Attribute("points"));
        std::vector<Vec3f> normals = get_points(xml_object->Attribute("normals"));
        std::vector<std::vector<double>> texture_coordinates = get_texture_coordinates(xml_object->Attribute("texture_coordinates"));

        Vec3f* points_ptr = points.empty() ? nullptr : points.data();
        Vec3f* normals_ptr = normals.empty() ? nullptr : normals.data();

        double** texture_coordinates_ptr;
        double* texture_coordinates_data[3];
        if (texture_coordinates.empty()){
            texture_coordinates_ptr = nullptr;
        }
        else {
            texture_coordinates_data[0] = texture_coordinates[0].data();
            texture_coordinates_data[1] = texture_coordinates[1].data();
            texture_coordinates_data[2] = texture_coordinates[2].data();
            texture_coordinates_ptr = &(texture_coordinates_data[0]);
        }

        return std::unique_ptr<Shape_t>(
                    new TriangleMotionblur_t(get_material(xml_object->Attribute("material"), xml_materials), get_transform_matrix(xml_object->Attribute("transform_matrix"), xml_transform_matrices), points_ptr, normals_ptr, texture_coordinates_ptr));
    }
    else if (type == "triangle_mesh"){
        const char* attributes[] = {"material", "transform_matrix", "mesh_geometry", "index"};
        require_attributes(xml_object, attributes, 4);
        return std::unique_ptr<Shape_t>(
                    new TriangleMesh_t(get_material(xml_object->Attribute("material"), xml_materials), get_transform_matrix(xml_object->Attribute("transform_matrix"), xml_transform_matrices), get_mesh_geometry(xml_object->Attribute("mesh_geometry"), xml_mesh_geometries), xml_object->UnsignedAttribute("index")));
    }
    else if (type == "triangle_mesh_motionblur"){
        const char* attributes[] = {"material", "transform_matrix", "mesh_geometry", "index"};
        require_attributes(xml_object, attributes, 4);
        return std::unique_ptr<Shape_t>(
                    new TriangleMeshMotionblur_t(get_material(xml_object->Attribute("material"), xml_materials), get_transform_matrix(xml_object->Attribute("transform_matrix"), xml_transform_matrices), get_mesh_geometry(xml_object->Attribute("mesh_geometry"), xml_mesh_geometries), xml_object->UnsignedAttribute("index")));
    }
    else{
        std::cerr << "Error, object type '" << type << "' not implemented. Exiting." << std::endl; 
        exit(60);
    }
}

std::unique_ptr<DirectionalLight_t> APTracer::Entities::SceneContext_t::create_directional_light(const tinyxml2::XMLElement* xml_directional_light, const tinyxml2::XMLElement* xml_transform_matrices){
    std::string type;
    const char* type_char = xml_directional_light->Attribute("type");
    if (type_char == nullptr) {
        std::cerr << "Error: XML directional lights should have a 'type' attribute. Using 'directional_light'." << std::endl;
        type = "directional_light";
    }
    else {
        type = type_char;
    }
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);

    if (type == "directional_light"){
        const char* attributes[] = {"colour", "transform_matrix"};
        require_attributes(xml_directional_light, attributes, 2);
        return std::unique_ptr<DirectionalLight_t>(
                    new DirectionalLight_t(APTracer::get_colour(xml_directional_light->Attribute("colour")), get_transform_matrix(xml_directional_light->Attribute("transform_matrix"), xml_transform_matrices)));
    }
    else{
        std::cerr << "Error, directional light type '" << type << "' not implemented. Only 'directional_light' exists for now. Exiting." << std::endl; 
        exit(70);
    }
}

std::unique_ptr<Skybox_t> APTracer::Entities::SceneContext_t::create_skybox(const tinyxml2::XMLElement* xml_skybox, const tinyxml2::XMLElement* xml_textures, const tinyxml2::XMLElement* xml_transform_matrices, const tinyxml2::XMLElement* xml_directional_lights) {
    std::string type;
    const char* type_char = xml_skybox->Attribute("type");
    if (type_char == nullptr) {
        std::cerr << "Error: XML skyboxes should have a 'type' attribute. Using 'skybox_flat'." << std::endl;
        type = "skybox_flat";
    }
    else {
        type = type_char;
    }
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);

    if (type == "skybox_flat"){
        const char* attributes[] = {"colour"};
        require_attributes(xml_skybox, attributes, 1);
        return std::unique_ptr<Skybox_t>(
                    new SkyboxFlat_t(APTracer::get_colour(xml_skybox->Attribute("colour"))));
    }
    else if (type == "skybox_flat_sun"){
        const char* attributes[] = {"colour", "lights"};
        require_attributes(xml_skybox, attributes, 2);
        std::vector<DirectionalLight_t*> lights;

        get_lights(xml_skybox->Attribute("lights"), lights, xml_directional_lights);

        DirectionalLight_t** lights_ptr = lights.empty() ? nullptr : lights.data();
               
        return std::unique_ptr<Skybox_t>(
                    new SkyboxFlatSun_t(APTracer::get_colour(xml_skybox->Attribute("colour")), lights_ptr, lights.size()));
    }
    else if (type == "skybox_texture"){
        const char* attributes[] = {"texture"};
        require_attributes(xml_skybox, attributes, 1);
        return std::unique_ptr<Skybox_t>(
                    new SkyboxTexture_t(get_texture(xml_skybox->Attribute("texture"), xml_textures)));
    }
    else if (type == "skybox_texture_sun"){
        const char* attributes[] = {"texture", "light_position", "light_colour", "light_radius"};
        require_attributes(xml_skybox, attributes, 4);
        double sun_pos[2];
        get_xy(xml_skybox->Attribute("light_position"), sun_pos);
        return std::unique_ptr<Skybox_t>(
                    new SkyboxTextureSun_t(get_texture(xml_skybox->Attribute("texture"), xml_textures), sun_pos, APTracer::get_colour(xml_skybox->Attribute("light_colour")), xml_skybox->DoubleAttribute("light_radius")));
    }
    else if (type == "skybox_texture_transformation"){
        const char* attributes[] = {"texture", "transform_matrix"};
        require_attributes(xml_skybox, attributes, 2);
        return std::unique_ptr<Skybox_t>(
                    new SkyboxTextureTransformation_t(get_texture(xml_skybox->Attribute("texture"), xml_textures), get_transform_matrix(xml_skybox->Attribute("transform_matrix"), xml_transform_matrices)));
    }
    else if (type == "skybox_texture_transformation_sun"){
        const char* attributes[] = {"texture", "light_position", "light_colour", "light_radius", "transform_matrix"};
        require_attributes(xml_skybox, attributes, 5);
        double sun_pos[2];
        get_xy(xml_skybox->Attribute("light_position"), sun_pos);
        return std::unique_ptr<Skybox_t>(
                    new SkyboxTextureTransformationSun_t(get_texture(xml_skybox->Attribute("texture"), xml_textures), get_transform_matrix(xml_skybox->Attribute("transform_matrix"), xml_transform_matrices), sun_pos, APTracer::get_colour(xml_skybox->Attribute("light_colour")), xml_skybox->DoubleAttribute("light_radius")));
    }
    else{
        std::cerr << "Error, skybox type '" << type << "' not implemented. Ignoring." << std::endl; 
        return std::unique_ptr<Skybox_t>(new SkyboxFlat_t(Vec3f(0.5)));
    }
}

std::unique_ptr<ImgBuffer_t> APTracer::Entities::SceneContext_t::create_imgbuffer(const tinyxml2::XMLElement* xml_imgbuffer) {
    std::string type;
    const char* type_char = xml_imgbuffer->Attribute("type");
    if (type_char == nullptr) {
        std::cerr << "Error: XML imgbuffers should have a 'type' attribute. Using 'imgbuffer'." << std::endl;
        type = "imgbuffer";
    }
    else {
        type = type_char;
    }
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);

    if (type == "imgbuffer"){
        const char* attributes[] = {"resx", "resy"};
        require_attributes(xml_imgbuffer, attributes, 2);
        return std::unique_ptr<ImgBuffer_t>(
                    new ImgBuffer_t(xml_imgbuffer->UnsignedAttribute("resx"), xml_imgbuffer->UnsignedAttribute("resy")));
    }
    else if (type == "imgbuffer_opengl"){
        const char* attributes[] = {"resx", "resy"};
        require_attributes(xml_imgbuffer, attributes, 2);
        use_gl_ = true; 
        opengl_imgbuffer_ = new ImgBufferOpenGL_t(xml_imgbuffer->UnsignedAttribute("resx"), xml_imgbuffer->UnsignedAttribute("resy"));
        return std::unique_ptr<ImgBuffer_t>(opengl_imgbuffer_);
    }
    else{
        std::cerr << "Error, imgbuffer type '" << type << "' not implemented. Only 'imgbuffer', and 'imgbuffer_opengl' exist for now. Ignoring." << std::endl; 
        return std::unique_ptr<ImgBuffer_t>(new ImgBuffer_t(300, 200));
    }
}

std::unique_ptr<Camera_t> APTracer::Entities::SceneContext_t::create_camera(const tinyxml2::XMLElement* xml_camera, const std::string &next_filename, const tinyxml2::XMLElement* xml_transform_matrices, const tinyxml2::XMLElement* xml_mediums, const tinyxml2::XMLElement* xml_imgbuffers, const tinyxml2::XMLElement* xml_skyboxes) {
    std::string type;
    const char* type_char = xml_camera->Attribute("type");
    if (type_char == nullptr) {
        std::cerr << "Error: XML cameras should have a 'type' attribute. Using 'cam'." << std::endl;
        type = "cam";
    }
    else {
        type = type_char;
    }
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);
    std::string filename;
    const char* filename_char = xml_camera->Attribute("filename");
    if (filename_char == nullptr){
        std::cerr << "Error: XML cameras should have a 'filename' attribute. Using 'nan'." << std::endl;
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
        const char* attributes[] = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "medium_list", "skybox", "max_bounces", "gammaind"};
        require_attributes(xml_camera, attributes, 9);
        double fov[2];
        unsigned int subpix[2];
        get_xy(xml_camera->Attribute("fov"), fov);
        get_xy(xml_camera->Attribute("subpix"), subpix);

        return std::unique_ptr<Camera_t>(
                    new Cam_t(get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), 
                            fov, subpix, get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), xml_camera->UnsignedAttribute("max_bounces"), xml_camera->DoubleAttribute("gammaind")));
    }
    else if (type == "cam_aperture"){
        const char* attributes[] = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "medium_list", "skybox", "max_bounces", "gammaind", "focal_length", "aperture"};
        require_attributes(xml_camera, attributes, 11);
        double fov[2];
        unsigned int subpix[2];
        get_xy(xml_camera->Attribute("fov"), fov);
        get_xy(xml_camera->Attribute("subpix"), subpix);

        return std::unique_ptr<Camera_t>(
                    new CamAperture_t(get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), 
                            fov, subpix, get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), xml_camera->UnsignedAttribute("max_bounces"), xml_camera->DoubleAttribute("focal_length"),
                            xml_camera->DoubleAttribute("aperture"), xml_camera->DoubleAttribute("gammaind")));
    }
    else if (type == "cam_motionblur"){
        const char* attributes[] = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "medium_list", "skybox", "max_bounces", "gammaind", "time"};
        require_attributes(xml_camera, attributes, 10);
        double fov[2];
        unsigned int subpix[2];
        double time[2];
        get_xy(xml_camera->Attribute("fov"), fov);
        get_xy(xml_camera->Attribute("subpix"), subpix);
        get_xy(xml_camera->Attribute("time"), time);
        
        return std::unique_ptr<Camera_t>(
                    new CamMotionblur_t(get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), 
                            fov, subpix, get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), xml_camera->UnsignedAttribute("max_bounces"), time, xml_camera->DoubleAttribute("gammaind")));
    }
    else if (type == "cam_motionblur_aperture"){
        const char* attributes[] = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "medium_list", "skybox", "max_bounces", "gammaind", "focal_length", "aperture", "time"};
        require_attributes(xml_camera, attributes, 12);
        double fov[2];
        unsigned int subpix[2];
        double time[2];
        get_xy(xml_camera->Attribute("fov"), fov);
        get_xy(xml_camera->Attribute("subpix"), subpix);
        get_xy(xml_camera->Attribute("time"), time);
        
        return std::unique_ptr<Camera_t>(
                    new CamMotionblurAperture_t(get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), 
                            fov, subpix, get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), xml_camera->UnsignedAttribute("max_bounces"), xml_camera->DoubleAttribute("focal_length"),
                            xml_camera->DoubleAttribute("aperture"), time, xml_camera->DoubleAttribute("gammaind")));
    }
    else if (type == "isocam"){
        const char* attributes[] = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "medium_list", "skybox", "max_bounces", "gammaind"};
        require_attributes(xml_camera, attributes, 9);
        double fov[2];
        unsigned int subpix[2];
        get_xy(xml_camera->Attribute("fov"), fov);
        get_xy(xml_camera->Attribute("subpix"), subpix);

        return std::unique_ptr<Camera_t>(
                    new IsoCam_t(get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), 
                            fov, subpix, get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), xml_camera->UnsignedAttribute("max_bounces"), xml_camera->DoubleAttribute("gammaind")));
    }
    else if (type == "isocam_aperture"){
        const char* attributes[] = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "medium_list", "skybox", "max_bounces", "gammaind", "focal_length", "aperture"};
        require_attributes(xml_camera, attributes, 11);
        double fov[2];
        unsigned int subpix[2];
        get_xy(xml_camera->Attribute("fov"), fov);
        get_xy(xml_camera->Attribute("subpix"), subpix);

        return std::unique_ptr<Camera_t>(
                    new IsoCamAperture_t(get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), 
                            fov, subpix, get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), xml_camera->UnsignedAttribute("max_bounces"), xml_camera->DoubleAttribute("focal_length"),
                            xml_camera->DoubleAttribute("aperture"), xml_camera->DoubleAttribute("gammaind")));
    }
    else if (type == "isocam_motionblur"){
        const char* attributes[] = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "medium_list", "skybox", "max_bounces", "gammaind", "time"};
        require_attributes(xml_camera, attributes, 10);
        double fov[2];
        unsigned int subpix[2];
        double time[2];
        get_xy(xml_camera->Attribute("fov"), fov);
        get_xy(xml_camera->Attribute("subpix"), subpix);
        get_xy(xml_camera->Attribute("time"), time);
        
        return std::unique_ptr<Camera_t>(
                    new IsoCamMotionblur_t(get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), 
                            fov, subpix, get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), xml_camera->UnsignedAttribute("max_bounces"), time, xml_camera->DoubleAttribute("gammaind")));
    }
    else if (type == "isocam_motionblur_aperture"){
        const char* attributes[] = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "medium_list", "skybox", "max_bounces", "gammaind", "focal_length", "aperture", "time"};
        require_attributes(xml_camera, attributes, 12);
        double fov[2];
        unsigned int subpix[2];
        double time[2];
        get_xy(xml_camera->Attribute("fov"), fov);
        get_xy(xml_camera->Attribute("subpix"), subpix);
        get_xy(xml_camera->Attribute("time"), time);
        
        return std::unique_ptr<Camera_t>(
                    new IsoCamMotionblurAperture_t(get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), 
                            fov, subpix, get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), xml_camera->UnsignedAttribute("max_bounces"), xml_camera->DoubleAttribute("focal_length"),
                            xml_camera->DoubleAttribute("aperture"), time, xml_camera->DoubleAttribute("gammaind")));
    }
    else if (type == "reccam"){
        const char* attributes[] = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "medium_list", "skybox", "max_bounces", "gammaind"};
        require_attributes(xml_camera, attributes, 9);
        double fov[2];
        unsigned int subpix[2];
        get_xy(xml_camera->Attribute("fov"), fov);
        get_xy(xml_camera->Attribute("subpix"), subpix);

        return std::unique_ptr<Camera_t>(
                    new RecCam_t(get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), 
                            fov, subpix, get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), xml_camera->UnsignedAttribute("max_bounces"), xml_camera->DoubleAttribute("gammaind")));
    }
    else if (type == "reccam_aperture"){
        const char* attributes[] = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "medium_list", "skybox", "max_bounces", "gammaind", "focal_length", "aperture"};
        require_attributes(xml_camera, attributes, 11);
        double fov[2];
        unsigned int subpix[2];
        get_xy(xml_camera->Attribute("fov"), fov);
        get_xy(xml_camera->Attribute("subpix"), subpix);

        return std::unique_ptr<Camera_t>(
                    new RecCamAperture_t(get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), 
                            fov, subpix, get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), xml_camera->UnsignedAttribute("max_bounces"), xml_camera->DoubleAttribute("focal_length"),
                            xml_camera->DoubleAttribute("aperture"), xml_camera->DoubleAttribute("gammaind")));
    }
    else if (type == "reccam_motionblur"){
        const char* attributes[] = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "medium_list", "skybox", "max_bounces", "gammaind", "time"};
        require_attributes(xml_camera, attributes, 10);
        double fov[2];
        unsigned int subpix[2];
        double time[2];
        get_xy(xml_camera->Attribute("fov"), fov);
        get_xy(xml_camera->Attribute("subpix"), subpix);
        get_xy(xml_camera->Attribute("time"), time);
        
        return std::unique_ptr<Camera_t>(
                    new RecCamMotionblur_t(get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), 
                            fov, subpix, get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), xml_camera->UnsignedAttribute("max_bounces"), time, xml_camera->DoubleAttribute("gammaind")));
    }
    else if (type == "reccam_motionblur_aperture"){
        const char* attributes[] = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "medium_list", "skybox", "max_bounces", "gammaind", "focal_length", "aperture", "time"};
        require_attributes(xml_camera, attributes, 12);
        double fov[2];
        unsigned int subpix[2];
        double time[2];
        get_xy(xml_camera->Attribute("fov"), fov);
        get_xy(xml_camera->Attribute("subpix"), subpix);
        get_xy(xml_camera->Attribute("time"), time);
        
        return std::unique_ptr<Camera_t>(
                    new RecCamMotionblurAperture_t(get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), 
                            fov, subpix, get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), xml_camera->UnsignedAttribute("max_bounces"), xml_camera->DoubleAttribute("focal_length"),
                            xml_camera->DoubleAttribute("aperture"), time, xml_camera->DoubleAttribute("gammaind")));
    }
    else if (type == "cam_3d"){
        const char* attributes[] = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "imgbuffer_L", "imgbuffer_R", "eye_dist", "medium_list", "skybox", "max_bounces", "gammaind"};
        require_attributes(xml_camera, attributes, 12);
        double fov[2];
        unsigned int subpix[2];
        get_xy(xml_camera->Attribute("fov"), fov);
        get_xy(xml_camera->Attribute("subpix"), subpix);

        return std::unique_ptr<Camera_t>(
                    new Cam3D_t(get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), fov, subpix, 
                            get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_imgbuffer(xml_camera->Attribute("imgbuffer_L"), xml_imgbuffers), 
                            get_imgbuffer(xml_camera->Attribute("imgbuffer_R"), xml_imgbuffers), xml_camera->DoubleAttribute("eye_dist"), 
                            get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), 
                            xml_camera->UnsignedAttribute("max_bounces"), xml_camera->DoubleAttribute("focal_length"), xml_camera->DoubleAttribute("gammaind")));
    }
    else if (type == "cam_3d_aperture"){
        const char* attributes[] = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "imgbuffer_L", "imgbuffer_R", "eye_dist", "medium_list", "skybox", "max_bounces", "gammaind", "focal_length", "aperture"};
        require_attributes(xml_camera, attributes, 14);
        double fov[2];
        unsigned int subpix[2];
        get_xy(xml_camera->Attribute("fov"), fov);
        get_xy(xml_camera->Attribute("subpix"), subpix);

        return std::unique_ptr<Camera_t>(
                    new Cam3DAperture_t(get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), fov, subpix, 
                            get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_imgbuffer(xml_camera->Attribute("imgbuffer_L"), xml_imgbuffers), 
                            get_imgbuffer(xml_camera->Attribute("imgbuffer_R"), xml_imgbuffers), xml_camera->DoubleAttribute("eye_dist"), 
                            get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), 
                            xml_camera->UnsignedAttribute("max_bounces"), xml_camera->DoubleAttribute("focal_length"), xml_camera->DoubleAttribute("aperture"), 
                            xml_camera->DoubleAttribute("gammaind")));
    }
    else if (type == "cam_3d_motionblur"){
        const char* attributes[] = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "imgbuffer_L", "imgbuffer_R", "eye_dist", "medium_list", "skybox", "max_bounces", "gammaind", "time"};
        require_attributes(xml_camera, attributes, 13);
        double fov[2];
        unsigned int subpix[2];
        double time[2];
        get_xy(xml_camera->Attribute("fov"), fov);
        get_xy(xml_camera->Attribute("subpix"), subpix);
        get_xy(xml_camera->Attribute("time"), time);

        return std::unique_ptr<Camera_t>(
                    new Cam3DMotionblur_t(get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), fov, subpix, 
                            get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_imgbuffer(xml_camera->Attribute("imgbuffer_L"), xml_imgbuffers), 
                            get_imgbuffer(xml_camera->Attribute("imgbuffer_R"), xml_imgbuffers), xml_camera->DoubleAttribute("eye_dist"), 
                            get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), 
                            xml_camera->UnsignedAttribute("max_bounces"), xml_camera->DoubleAttribute("focal_length"), time, xml_camera->DoubleAttribute("gammaind")));
    }
    else if (type == "cam_3d_motionblur_aperture"){
        const char* attributes[] = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "imgbuffer_L", "imgbuffer_R", "eye_dist", "medium_list", "skybox", "max_bounces", "gammaind", "focal_length", "aperture", "time"};
        require_attributes(xml_camera, attributes, 15);
        double fov[2];
        unsigned int subpix[2];
        double time[2];
        get_xy(xml_camera->Attribute("fov"), fov);
        get_xy(xml_camera->Attribute("subpix"), subpix);
        get_xy(xml_camera->Attribute("time"), time);

        return std::unique_ptr<Camera_t>(
                    new Cam3DMotionblurAperture_t(get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), fov, subpix, 
                            get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_imgbuffer(xml_camera->Attribute("imgbuffer_L"), xml_imgbuffers), 
                            get_imgbuffer(xml_camera->Attribute("imgbuffer_R"), xml_imgbuffers), xml_camera->DoubleAttribute("eye_dist"), 
                            get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), 
                            xml_camera->UnsignedAttribute("max_bounces"), xml_camera->DoubleAttribute("focal_length"), xml_camera->DoubleAttribute("aperture"), 
                            time, xml_camera->DoubleAttribute("gammaind")));
    }
    else{
        std::cerr << "Error, camera type '" << type << "' not implemented. Exiting." << std::endl; 
        exit(100);
    }
}

void APTracer::Entities::SceneContext_t::create_acceleration_structure(const tinyxml2::XMLElement* xml_acceleration_structure) {
    if (xml_acceleration_structure == nullptr){
        scene_->acc_ = new AccelerationMultiGridVector_t(scene_->geometry_, scene_->n_obj_);
        return;
    }

    std::string type;
    const char* type_char = xml_acceleration_structure->Attribute("type");
    if (type_char == nullptr) {
        std::cerr << "Error: XML acceleration structures should have a 'type' attribute. Using 'multi_grid_vector'." << std::endl;
        type = "multi_grid_vector";
    }
    else {
        type = type_char;
    }
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);

    if (type == "grid"){
        scene_->acc_ = new AccelerationGrid_t(scene_->geometry_, scene_->n_obj_, nullptr, 0, xml_acceleration_structure->UnsignedAttribute("min_resolution", 1), xml_acceleration_structure->UnsignedAttribute("max_resolution", 128));
    }
    else if (type == "grid_array"){
        scene_->acc_ = new AccelerationGridArray_t(scene_->geometry_, scene_->n_obj_, nullptr, 0, xml_acceleration_structure->UnsignedAttribute("min_resolution", 1), xml_acceleration_structure->UnsignedAttribute("max_resolution", 128));
    }
    else if (type == "grid_vector"){
        scene_->acc_ = new AccelerationGridVector_t(scene_->geometry_, scene_->n_obj_, nullptr, 0, xml_acceleration_structure->UnsignedAttribute("min_resolution", 1), xml_acceleration_structure->UnsignedAttribute("max_resolution", 128));
    }
    else if (type == "multi_grid"){
        scene_->acc_ = new AccelerationMultiGrid_t(scene_->geometry_, scene_->n_obj_, nullptr, 0, xml_acceleration_structure->UnsignedAttribute("min_resolution", 1), xml_acceleration_structure->UnsignedAttribute("max_resolution", 128), xml_acceleration_structure->UnsignedAttribute("max_cell_content", 32), xml_acceleration_structure->UnsignedAttribute("max_grid_level", 1));
    }
    else if (type == "multi_grid_array"){
        scene_->acc_ = new AccelerationMultiGridArray_t(scene_->geometry_, scene_->n_obj_, nullptr, 0, xml_acceleration_structure->UnsignedAttribute("min_resolution", 1), xml_acceleration_structure->UnsignedAttribute("max_resolution", 128), xml_acceleration_structure->UnsignedAttribute("max_cell_content", 32), xml_acceleration_structure->UnsignedAttribute("max_grid_level", 1));
    }
    else if (type == "multi_grid_vector"){
        scene_->acc_ = new AccelerationMultiGridVector_t(scene_->geometry_, scene_->n_obj_, nullptr, 0, xml_acceleration_structure->UnsignedAttribute("min_resolution", 1), xml_acceleration_structure->UnsignedAttribute("max_resolution", 128), xml_acceleration_structure->UnsignedAttribute("max_cell_content", 32), xml_acceleration_structure->UnsignedAttribute("max_grid_level", 1));
    }
    else{
        std::cerr << "Error, acceleration structure type '" << type << "' not implemented. Using 'multi_grid_vector'." << std::endl; 
        scene_->acc_ = new AccelerationMultiGridVector_t(scene_->geometry_, scene_->n_obj_);
    }
}

TransformMatrix_t* APTracer::Entities::SceneContext_t::get_transform_matrix(std::string transform_matrix, const tinyxml2::XMLElement* xml_transform_matrices){
    std::transform(transform_matrix.begin(), transform_matrix.end(), transform_matrix.begin(), ::tolower);
    if (transform_matrix == "nan"){
        transform_matrices_[index_transform_matrices_] = std::unique_ptr<TransformMatrix_t>(new TransformMatrix_t());
        ++index_transform_matrices_;
        return transform_matrices_[index_transform_matrices_ - 1].get();
    }
    else if (is_number(transform_matrix)) {
        return transform_matrices_[std::stoi(transform_matrix) - 1].get();
    }
    else {
        if (xml_transform_matrices != nullptr){
            unsigned int index = 0;
            for (const tinyxml2::XMLElement* xml_transform_matrix = xml_transform_matrices->FirstChildElement("transform_matrix"); xml_transform_matrix; xml_transform_matrix = xml_transform_matrix->NextSiblingElement("transform_matrix")){
                const char* transform_matrix_char = xml_transform_matrix->Attribute("name");
                if (transform_matrix_char != nullptr){
                    std::string name_transform_matrix = transform_matrix_char;
                    std::transform(name_transform_matrix.begin(), name_transform_matrix.end(), name_transform_matrix.begin(), ::tolower);
                    if (name_transform_matrix == transform_matrix){
                        return transform_matrices_[index].get();
                    }
                }                
                ++index;
            }
        }
    }
    std::cerr << "Error, transformation matrix '" << transform_matrix << "' not found. Ignoring. This causes a memory leak." << std::endl;
    return new TransformMatrix_t();
}

std::unique_ptr<std::list<unsigned int>> APTracer::Entities::SceneContext_t::get_material_index_list(std::string string_material_list, const tinyxml2::XMLElement* xml_materials) const {
    std::unique_ptr<std::list<unsigned int>> material_list = std::unique_ptr<std::list<unsigned int>>(new std::list<unsigned int>());
    std::string delimiter = ", ";
    size_t pos = 0;
    std::string token;

    while ((pos = string_material_list.find(delimiter)) != std::string::npos) {
        token = string_material_list.substr(0, pos);

        if (is_number(token)) {
            material_list->push_back(std::stoi(token) - 1);
        }
        else {
            if (xml_materials != nullptr){
                bool missing = true;
                std::transform(token.begin(), token.end(), token.begin(), ::tolower);
                unsigned int index = 0;
                for (const tinyxml2::XMLElement* xml_material = xml_materials->FirstChildElement("material"); xml_material; xml_material = xml_material->NextSiblingElement("material")){
                    const char* material_char = xml_material->Attribute("name");
                    if (material_char != nullptr){
                        std::string name_material = material_char;
                        std::transform(name_material.begin(), name_material.end(), name_material.begin(), ::tolower);
                        if (name_material == token){
                            material_list->push_back(index);
                            missing = false;
                            break;
                        }
                    }                    
                    ++index;
                }
                if (missing){
                    std::cerr << "Error: material '" << token << "' not found, exiting." << std::endl;
                    exit(498);
                }
            }
            else {
                std::cerr << "Error: no materials, material '" << token << "' not found, exiting." << std::endl;
                exit(499);
            }
        }
        // CHECK this should check for errors.

        string_material_list.erase(0, pos + delimiter.length());
    }
    if (is_number(string_material_list)) {
        material_list->push_back(std::stoi(string_material_list) - 1);
    }
    else {
        if (xml_materials != nullptr){
            bool missing = true;
            std::transform(string_material_list.begin(), string_material_list.end(), string_material_list.begin(), ::tolower);
            unsigned int index = 0;
            for (const tinyxml2::XMLElement* xml_material = xml_materials->FirstChildElement("material"); xml_material; xml_material = xml_material->NextSiblingElement("material")){
                const char* material_char = xml_material->Attribute("name");
                if (material_char != nullptr){
                    std::string name_material = material_char;
                    std::transform(name_material.begin(), name_material.end(), name_material.begin(), ::tolower);
                    if (name_material == string_material_list){
                        material_list->push_back(index);
                        missing = false;
                        break;
                    }
                }                
                ++index;
            }
            if (missing){
                std::cerr << "Error: material '" << string_material_list << "' not found, exiting." << std::endl;
                exit(498);
            }
        }
        else {
            std::cerr << "Error: no materials, material '" << string_material_list << "' not found, exiting." << std::endl;
            exit(499);
        }
    }
    return material_list;
}

std::unique_ptr<std::list<unsigned int>> APTracer::Entities::SceneContext_t::get_medium_index_list(std::string string_medium_list, const tinyxml2::XMLElement* xml_mediums) const {
    std::unique_ptr<std::list<unsigned int>> medium_list = std::unique_ptr<std::list<unsigned int>>(new std::list<unsigned int>());
    std::string delimiter = ", ";
    size_t pos = 0;
    std::string token;

    while ((pos = string_medium_list.find(delimiter)) != std::string::npos) {
        token = string_medium_list.substr(0, pos);

        if (is_number(token)) {
            medium_list->push_back(std::stoi(token) - 1);
        }
        else {
            if (xml_mediums != nullptr){
                bool missing = true;
                std::transform(token.begin(), token.end(), token.begin(), ::tolower);
                unsigned int index = 0;
                for (const tinyxml2::XMLElement* xml_medium = xml_mediums->FirstChildElement("medium"); xml_medium; xml_medium = xml_medium->NextSiblingElement("medium")){
                    const char* medium_char = xml_medium->Attribute("name");
                    if (medium_char != nullptr){
                        std::string name_medium = medium_char;
                        std::transform(name_medium.begin(), name_medium.end(), name_medium.begin(), ::tolower);
                        if (name_medium == token){
                            medium_list->push_back(index);
                            missing = false;
                            break;
                        }
                    }                    
                    ++index;
                }
                if (missing){
                    std::cerr << "Error: medium '" << token << "' not found, exiting." << std::endl;
                    exit(498);
                }
            }
            else {
                std::cerr << "Error: no mediums, medium '" << token << "' not found, exiting." << std::endl;
                exit(499);
            }
        }
        // CHECK this should check for errors.

        string_medium_list.erase(0, pos + delimiter.length());
    }
    if (is_number(string_medium_list)) {
        medium_list->push_back(std::stoi(string_medium_list) - 1);
    }
    else {
        if (xml_mediums != nullptr){
            bool missing = true;
            std::transform(string_medium_list.begin(), string_medium_list.end(), string_medium_list.begin(), ::tolower);
            unsigned int index = 0;
            for (const tinyxml2::XMLElement* xml_medium = xml_mediums->FirstChildElement("medium"); xml_medium; xml_medium = xml_medium->NextSiblingElement("medium")){
                const char* medium_char = xml_medium->Attribute("name");
                if (medium_char != nullptr){
                    std::string name_medium = medium_char;
                    std::transform(name_medium.begin(), name_medium.end(), name_medium.begin(), ::tolower);
                    if (name_medium == string_medium_list){
                        medium_list->push_back(index);
                        missing = false;
                        break;
                    }
                }                
                ++index;
            }
            if (missing){
                std::cerr << "Error: medium '" << string_medium_list << "' not found, exiting." << std::endl;
                exit(498);
            }
        }
        else {
            std::cerr << "Error: no mediums, medium '" << string_medium_list << "' not found, exiting." << std::endl;
            exit(499);
        }
    }
    return medium_list;
}

std::list<Medium_t*> APTracer::Entities::SceneContext_t::get_medium_list(std::string string_medium_list, const tinyxml2::XMLElement* xml_mediums) const {
    std::list<Medium_t*> medium_list = std::list<Medium_t*>(); // CHECK full of dynamic casts with no checks... should maybe check for errors
    std::string delimiter = ", ";
    size_t pos = 0;
    std::string token;

    while ((pos = string_medium_list.find(delimiter)) != std::string::npos) {
        token = string_medium_list.substr(0, pos);

        if (is_number(token)) {
            medium_list.push_back(mediums_[std::stoi(token) - 1].get());
        }
        else {
            if (xml_mediums != nullptr){
                bool missing = true;
                std::transform(token.begin(), token.end(), token.begin(), ::tolower);
                unsigned int index = 0;
                for (const tinyxml2::XMLElement* xml_medium = xml_mediums->FirstChildElement("medium"); xml_medium; xml_medium = xml_medium->NextSiblingElement("medium")){
                    const char* medium_char = xml_medium->Attribute("name");
                    if (medium_char != nullptr){
                        std::string name_medium = medium_char;
                        std::transform(name_medium.begin(), name_medium.end(), name_medium.begin(), ::tolower);
                        if (name_medium == token){
                            medium_list.push_back(mediums_[index].get());
                            missing = false;
                            break;
                        }
                    }
                    ++index;
                }
                if (missing){
                    std::cerr << "Error: medium '" << token << "' not found, exiting." << std::endl;
                    exit(496);
                }
            }
            else {
                std::cerr << "Error: no mediums, medium '" << token << "' not found, exiting." << std::endl;
                exit(497);
            }
        }
        // CHECK this should check for errors.

        string_medium_list.erase(0, pos + delimiter.length());
    }
    if (is_number(string_medium_list)) {
        medium_list.push_back(mediums_[std::stoi(string_medium_list) - 1].get());
    }
    else {
        if (xml_mediums != nullptr){
            bool missing = true;
            std::transform(string_medium_list.begin(), string_medium_list.end(), string_medium_list.begin(), ::tolower);
            unsigned int index = 0;
            for (const tinyxml2::XMLElement* xml_medium = xml_mediums->FirstChildElement("medium"); xml_medium; xml_medium = xml_medium->NextSiblingElement("medium")){
                const char* medium_char = xml_medium->Attribute("name");
                if (medium_char != nullptr){
                    std::string name_medium = medium_char;
                    std::transform(name_medium.begin(), name_medium.end(), name_medium.begin(), ::tolower);
                    if (name_medium == string_medium_list){
                        medium_list.push_back(mediums_[index].get());
                        missing = false;
                        break;
                    }
                }
                ++index;
            }
            if (missing){
                std::cerr << "Error: medium '" << string_medium_list << "' not found, exiting." << std::endl;
                exit(496);
            }
        }
        else {
            std::cerr << "Error: no mediums, medium '" << string_medium_list << "' not found, exiting." << std::endl;
            exit(497);
        }
    }
    return medium_list;
}

Texture_t* APTracer::Entities::SceneContext_t::get_texture(std::string texture, const tinyxml2::XMLElement* xml_textures) const {
    if (is_number(texture)) {
        return textures_[std::stoi(texture) - 1].get();
    }
    else {
        if (xml_textures != nullptr){
            std::transform(texture.begin(), texture.end(), texture.begin(), ::tolower);
            unsigned int index = 0;
            for (const tinyxml2::XMLElement* xml_texture = xml_textures->FirstChildElement("texture"); xml_texture; xml_texture = xml_texture->NextSiblingElement("texture")){
                const char* name_char = xml_texture->Attribute("name");
                if (name_char != nullptr){
                    std::string name_texture = name_char;
                    std::transform(name_texture.begin(), name_texture.end(), name_texture.begin(), ::tolower);
                    if (name_texture == texture){
                        return textures_[index].get();
                    }
                }
                ++index;
            }
        }
    }
    std::cerr << "Error, texture '" << texture << "' not found. Exiting." << std::endl;
    exit(21); 
}

std::vector<unsigned int> APTracer::Entities::SceneContext_t::get_material_mix(std::string material_refracted, std::string material_reflected, const tinyxml2::XMLElement* xml_materials) const {
    //std::unique_ptr<unsigned int> output_materials = std::unique_ptr<unsigned int>(new unsigned int[2]);
    std::vector<unsigned int> output_materials = std::vector<unsigned int>(2);
    
    if (is_number(material_refracted)) {
        output_materials[0] = std::stoi(material_refracted) - 1;
    } 
    else {
        if (xml_materials != nullptr){
            std::transform(material_refracted.begin(), material_refracted.end(), material_refracted.begin(), ::tolower);
            bool material_missing = true;
            unsigned int index = 0;
            for (const tinyxml2::XMLElement* xml_material = xml_materials->FirstChildElement("material"); xml_material; xml_material = xml_material->NextSiblingElement("material")){
                const char* name_char = xml_material->Attribute("name");
                if (name_char != nullptr){
                    std::string name_material = name_char;
                    std::transform(name_material.begin(), name_material.end(), name_material.begin(), ::tolower);
                    if (material_refracted == name_material){
                        output_materials[0] = index;
                        material_missing = false;
                        break;
                    }
                }                
                ++index;
            }
            if (material_missing){
                std::cerr << "Error, material '" << material_refracted << "' not found. Exiting." << std::endl;
                exit(41); 
            }
        }
        else{
            std::cerr << "Error, no materials, '" << material_refracted << "' not found. Exiting." << std::endl;
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
                const char* name_char = xml_material->Attribute("name");
                if (name_char != nullptr){
                    std::string name_material = name_char;
                    std::transform(name_material.begin(), name_material.end(), name_material.begin(), ::tolower);
                    if (material_reflected == name_material){
                        output_materials[1] = index;
                        material_missing = false;
                        break;
                    }
                }
                ++index;
            }
            if (material_missing){
                std::cerr << "Error, material '" << material_reflected << "' not found. Exiting." << std::endl;
                exit(41); 
            }
        }
        else{
            std::cerr << "Error, no materials, '" << material_reflected << "' not found. Exiting." << std::endl;
            exit(47);
        }        
    }

    return output_materials;
}

Medium_t* APTracer::Entities::SceneContext_t::get_medium(std::string medium, const tinyxml2::XMLElement* xml_mediums) const {
    if (is_number(medium)) {
        return mediums_[std::stoi(medium) - 1].get();
    }
    else {
        if (xml_mediums != nullptr){
            std::transform(medium.begin(), medium.end(), medium.begin(), ::tolower);
            unsigned int index = 0;
            for (const tinyxml2::XMLElement* xml_medium = xml_mediums->FirstChildElement("medium"); xml_medium; xml_medium = xml_medium->NextSiblingElement("medium")){
                const char* name_char = xml_medium->Attribute("name");
                if (name_char != nullptr){
                    std::string name_medium = name_char;
                    std::transform(name_medium.begin(), name_medium.end(), name_medium.begin(), ::tolower);
                    if (name_medium == medium){
                        return mediums_[index].get();
                    }
                }                
                ++index;
            }
        }
    }
    std::cerr << "Error, medium '" << medium << "' not found. Ignoring. This Causes a memory leak." << std::endl;
    return new NonAbsorber_t(1.0, 0);
}

MeshGeometry_t* APTracer::Entities::SceneContext_t::get_mesh_geometry(std::string mesh_geometry, const tinyxml2::XMLElement* xml_mesh_geometries) const {
    if (is_number(mesh_geometry)) {
        return mesh_geometries_[std::stoi(mesh_geometry) - 1].get();
    }
    else {
        if (xml_mesh_geometries != nullptr){
            std::transform(mesh_geometry.begin(), mesh_geometry.end(), mesh_geometry.begin(), ::tolower);
            unsigned int index = 0;
            for (const tinyxml2::XMLElement* xml_mesh_geometry = xml_mesh_geometries->FirstChildElement("mesh_geometry"); xml_mesh_geometry; xml_mesh_geometry = xml_mesh_geometry->NextSiblingElement("mesh_geometry")){
                const char* name_char = xml_mesh_geometry->Attribute("name");
                if (name_char != nullptr){
                    std::string name_mesh_geometry = name_char;
                    std::transform(name_mesh_geometry.begin(), name_mesh_geometry.end(), name_mesh_geometry.begin(), ::tolower);
                    if (name_mesh_geometry == mesh_geometry){
                        return mesh_geometries_[index].get();
                    }
                }                
                ++index;
            }
        }
    }
    std::cerr << "Error, mesh geometry '" << mesh_geometry << "' not found. Exiting." << std::endl;
    exit(51);
}

unsigned int APTracer::Entities::SceneContext_t::get_material_index(std::string material, const tinyxml2::XMLElement* xml_materials) const {
    if (is_number(material)) {
        return std::stoi(material) - 1;
    }
    else {
        if (xml_materials != nullptr){
            std::transform(material.begin(), material.end(), material.begin(), ::tolower);
            unsigned int index = 0;
            for (const tinyxml2::XMLElement* xml_material = xml_materials->FirstChildElement("material"); xml_material; xml_material = xml_material->NextSiblingElement("material")){
                const char* name_char = xml_material->Attribute("name");
                if (name_char != nullptr){
                    std::string name_material = name_char;
                    std::transform(name_material.begin(), name_material.end(), name_material.begin(), ::tolower);
                    if (name_material == material){
                        return index;
                    }
                }                
                ++index;
            }
        }
    }
    std::cerr << "Error, material '" << material << "' not found. Exiting." << std::endl;
    exit(41);
}

Material_t* APTracer::Entities::SceneContext_t::get_material(std::string material, const tinyxml2::XMLElement* xml_materials) const {
    if (is_number(material)) {
        return materials_[std::stoi(material) - 1].get();
    }
    else {
        if (xml_materials != nullptr){
            std::transform(material.begin(), material.end(), material.begin(), ::tolower);
            unsigned int index = 0;
            for (const tinyxml2::XMLElement* xml_material = xml_materials->FirstChildElement("material"); xml_material; xml_material = xml_material->NextSiblingElement("material")){
                const char* name_char = xml_material->Attribute("name");
                if (name_char != nullptr){
                    std::string name_material = name_char;
                    std::transform(name_material.begin(), name_material.end(), name_material.begin(), ::tolower);
                    if (name_material == material){
                        return materials_[index].get();
                    }
                }
                ++index;
            }
        }
    }
    std::cerr << "Error, material '" << material << "' not found. Exiting." << std::endl;
    exit(41);
}

void APTracer::Entities::SceneContext_t::get_lights(std::string lights_string, std::vector<DirectionalLight_t*> &lights, const tinyxml2::XMLElement* xml_directional_lights) const {
    std::list<DirectionalLight_t*> lights_list = std::list<DirectionalLight_t*>();
    std::string delimiter = ", ";
    size_t pos = 0;
    std::string token;

    while ((pos = lights_string.find(delimiter)) != std::string::npos) {
        token = lights_string.substr(0, pos);

        if (is_number(token)) {
            lights_list.push_back(directional_lights_[std::stoi(token) - 1].get());
        }
        else {
            if (xml_directional_lights != nullptr){
                bool missing = true;
                std::transform(token.begin(), token.end(), token.begin(), ::tolower);
                unsigned int index = 0;
                for (const tinyxml2::XMLElement* xml_directional_light = xml_directional_lights->FirstChildElement("directional_light"); xml_directional_light; xml_directional_light = xml_directional_light->NextSiblingElement("directional_light")){
                    const char* name_char = xml_directional_light->Attribute("name");
                    if (name_char != nullptr){
                        std::string name_light = name_char;
                        std::transform(name_light.begin(), name_light.end(), name_light.begin(), ::tolower);
                        if (name_light == token){
                            lights_list.push_back(directional_lights_[index].get());
                            missing = false;
                            break;
                        }
                    }
                    ++index;
                }
                if (missing){
                    std::cerr << "Error: light '" << token << "' not found, ignoring." << std::endl;
                }
            }
            else {
                std::cerr << "Error: no directional lights, light '" << token << "' not found, ignoring." << std::endl;
            }            
        }
        // CHECK this should check for errors.

        lights_string.erase(0, pos + delimiter.length());
    }
    if (is_number(lights_string)) {
        lights_list.push_back(directional_lights_[std::stoi(lights_string) - 1].get());
    }
    else {
        if (xml_directional_lights != nullptr){
            bool missing = true;
            std::transform(lights_string.begin(), lights_string.end(), lights_string.begin(), ::tolower);
            unsigned int index = 0;
            for (const tinyxml2::XMLElement* xml_directional_light = xml_directional_lights->FirstChildElement("directional_light"); xml_directional_light; xml_directional_light = xml_directional_light->NextSiblingElement("directional_light")){
                const char* name_char = xml_directional_light->Attribute("name");
                if (name_char != nullptr){
                    std::string name_light = name_char;
                    std::transform(name_light.begin(), name_light.end(), name_light.begin(), ::tolower);
                    if (name_light == lights_string){
                        lights_list.push_back(directional_lights_[index].get());
                        missing = false;
                        break;
                    }
                }                
                ++index;
            }
            if (missing){
                std::cerr << "Error: light '" << lights_string << "' not found, ignoring." << std::endl;
            }
        }
        else {
            std::cerr << "Error: no directional lights, light '" << lights_string << "' not found, ignoring." << std::endl;
        }
    }

    lights = std::vector<DirectionalLight_t*>(lights_list.size());
    unsigned int index = 0;

    for (auto it = lights_list.begin(); it != lights_list.end(); ++it){
        lights[index] = *it;
        ++index;
    }
}

ImgBuffer_t* APTracer::Entities::SceneContext_t::get_imgbuffer(std::string imgbuffer, const tinyxml2::XMLElement* xml_imgbuffers) const {
    if (is_number(imgbuffer)) {
        return imgbuffers_[std::stoi(imgbuffer) - 1].get();
    }
    else {
        if (xml_imgbuffers != nullptr){
            std::transform(imgbuffer.begin(), imgbuffer.end(), imgbuffer.begin(), ::tolower);
            unsigned int index = 0;
            for (const tinyxml2::XMLElement* xml_imgbuffer = xml_imgbuffers->FirstChildElement("imgbuffer"); xml_imgbuffer; xml_imgbuffer = xml_imgbuffer->NextSiblingElement("imgbuffer")){
                const char* name_char = xml_imgbuffer->Attribute("name");
                if (name_char != nullptr){
                    std::string name_imgbuffer = name_char;
                    std::transform(name_imgbuffer.begin(), name_imgbuffer.end(), name_imgbuffer.begin(), ::tolower);
                    if (name_imgbuffer == imgbuffer){
                        return imgbuffers_[index].get();
                    }
                }
                ++index;
            }
        }
    }
    std::cerr << "Error, image buffer '" << imgbuffer << "' not found. Exiting." << std::endl;
    exit(91); 
}

Skybox_t* APTracer::Entities::SceneContext_t::get_skybox(std::string skybox, const tinyxml2::XMLElement* xml_skyboxes) const {
    if (is_number(skybox)) {
        return skyboxes_[std::stoi(skybox) - 1].get();
    }
    else {
        if (xml_skyboxes != nullptr){
            std::transform(skybox.begin(), skybox.end(), skybox.begin(), ::tolower);
            unsigned int index = 0;
            for (const tinyxml2::XMLElement* xml_skybox = xml_skyboxes->FirstChildElement("skybox"); xml_skybox; xml_skybox = xml_skybox->NextSiblingElement("skybox")){
                const char* name_char = xml_skybox->Attribute("name");
                if (name_char != nullptr){
                    std::string name_skybox = name_char;
                    std::transform(name_skybox.begin(), name_skybox.end(), name_skybox.begin(), ::tolower);
                    if (name_skybox == skybox){
                        return skyboxes_[index].get();
                    }
                }
                ++index;
            }
        }
    }
    std::cerr << "Error, skybox '" << skybox << "' not found. Exiting." << std::endl;
    exit(81); 
}

void APTracer::Entities::SceneContext_t::get_objects(std::string objects_string, std::vector<Shape_t*> &shapes, std::vector<MeshTop_t*> &meshes, const tinyxml2::XMLElement* xml_objects) const {
    std::list<unsigned int> objects_list = std::list<unsigned int>();
    std::string delimiter = ", ";
    size_t pos = 0;
    std::string token;
    unsigned int n_shapes = 0;
    unsigned int n_meshes = 0;

    while ((pos = objects_string.find(delimiter)) != std::string::npos) {
        token = objects_string.substr(0, pos);

        if (is_number(token)) {
            objects_list.push_back(std::stoi(token) - 1);
        }
        else {
            if (xml_objects != nullptr){
                bool missing = true;
                std::transform(token.begin(), token.end(), token.begin(), ::tolower);
                unsigned int index = 0;
                for (const tinyxml2::XMLElement* xml_object = xml_objects->FirstChildElement("object"); xml_object; xml_object = xml_object->NextSiblingElement("object")){
                    const char* name_char = xml_object->Attribute("name");
                    if (name_char != nullptr){
                        std::string name_object = name_char;
                        std::transform(name_object.begin(), name_object.end(), name_object.begin(), ::tolower);
                        if (name_object == token){
                            objects_list.push_back(index);
                            missing = false;
                            break;
                        }
                    }
                    ++index;
                }
                if (missing){
                    std::cerr << "Error: object '" << token << "' not found, ignoring." << std::endl;
                }
            }
            else {
                std::cerr << "Error: no objects, object '" << token << "' not found, ignoring." << std::endl;
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
            bool missing = true;
            std::transform(objects_string.begin(), objects_string.end(), objects_string.begin(), ::tolower);
            unsigned int index = 0;
            for (const tinyxml2::XMLElement* xml_object = xml_objects->FirstChildElement("object"); xml_object; xml_object = xml_object->NextSiblingElement("object")){
                const char* name_char = xml_object->Attribute("name");
                if (name_char != nullptr){
                    std::string name_object = name_char;
                    std::transform(name_object.begin(), name_object.end(), name_object.begin(), ::tolower);
                    if (name_object == objects_string){
                        objects_list.push_back(index);
                        missing = false;
                        break;
                    }
                }
                ++index;
            }
            if (missing){
                std::cerr << "Error: object '" << objects_string << "' not found, ignoring." << std::endl;
            }
        }
        else {
            std::cerr << "Error: no objects, object '" << objects_string << "' not found, ignoring." << std::endl;
        }
    }

    for (auto object_list_item : objects_list){
        if (objects_[object_list_item]){
            ++n_shapes;
        }
        else {
            ++n_meshes;
        }
    }

    shapes = std::vector<Shape_t*>(n_shapes);
    meshes = std::vector<MeshTop_t*>(n_meshes);
    unsigned int index_shapes = 0;
    unsigned int index_meshes = 0;

    for (auto objects_list_item : objects_list){
        if (objects_[objects_list_item]){
            shapes[index_shapes] = objects_[objects_list_item].get();
            ++index_shapes;
        }
        else {
            meshes[index_meshes] = meshes_[objects_list_item].get();
            ++index_meshes;
        }
    }
}

void APTracer::Entities::SceneContext_t::get_objects(std::vector<Shape_t*> &shapes, std::vector<MeshTop_t*> &meshes) const {
    unsigned int n_shapes = 0;
    unsigned int n_meshes = 0;

    for (auto const &object: objects_){
        if (object){
            ++n_shapes;
        }
        else {
            ++n_meshes;
        }
    }

    shapes = std::vector<Shape_t*>(n_shapes);
    meshes = std::vector<MeshTop_t*>(n_meshes);
    unsigned int index_shapes = 0;
    unsigned int index_meshes = 0;

    for (unsigned int i = 0; i < objects_.size(); ++i){
        if (objects_[i]){
            shapes[index_shapes] = objects_[i].get();
            ++index_shapes;
        }
        else {
            meshes[index_meshes] = meshes_[i].get();
            ++index_meshes;
        }
    }
}

Vec3f APTracer::get_colour(std::string colour) {
    std::transform(colour.begin(), colour.end(), colour.begin(), ::tolower);
    
    auto it = APTracer::Colours::colours.find(colour);
    if (it != APTracer::Colours::colours.end()) {
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
            return Vec3f(values[0]);
        }
        else if (count != 3) {
            std::cerr << "Error, colour should be 1 or 3 values seperated by spaces, or a string. Current number of values is " << count << ", colour is '" << colour << "'. Ignoring." << std::endl;
            return Vec3f(0.5);
        }
        else{
            return Vec3f(values[0], values[1], values[2]);
        }
	}
}

std::vector<Vec3f> APTracer::get_points(std::string points_string) {
    std::vector<Vec3f> points;
    std::transform(points_string.begin(), points_string.end(), points_string.begin(), ::tolower);

    if (points_string != "nan"){
        points = std::vector<Vec3f>(3);
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
            std::cerr << "Error, triangle points should be 9 values seperated by spaces, or nan. Current number of values is " << count << ". Exiting." << std::endl;
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

std::vector<std::vector<double>> APTracer::get_texture_coordinates(std::string texture_coordinates_string) {
    std::vector<std::vector<double>> texture_coordinates;

    std::transform(texture_coordinates_string.begin(), texture_coordinates_string.end(), texture_coordinates_string.begin(), ::tolower);

    if (texture_coordinates_string != "nan"){
        texture_coordinates = std::vector<std::vector<double>>(3);
        texture_coordinates[0] =std::vector<double>(2);
        texture_coordinates[1] = std::vector<double>(2);
        texture_coordinates[2] = std::vector<double>(2);
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
            std::cerr << "Error, triangle texture coordinates should be 6 values seperated by spaces, or nan. Current number of values is " << count << ". Exiting." << std::endl;
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

std::unique_ptr<std::list<std::string>> APTracer::get_medium_names(std::string string_medium_names) {
    std::unique_ptr<std::list<std::string>> medium_names = std::unique_ptr<std::list<std::string>>(new std::list<std::string>());
    std::string delimiter = ", ";
    size_t pos = 0;

    while ((pos = string_medium_names.find(delimiter)) != std::string::npos) {
        medium_names->push_back(string_medium_names.substr(0, pos));
        string_medium_names.erase(0, pos + delimiter.length());
    }
    medium_names->push_back(string_medium_names);

    return medium_names;
}

void APTracer::get_xy(const std::string &string_value, double (&value)[2]) {
    unsigned int count = 0;
    std::stringstream ss(string_value);

    for(std::string s; ss >> s; ){
        if (count < 2){
            value[count] = std::stod(s);
        }
        ++count;
    }

    if (count != 2) {
        std::cerr << "Error, xy should be 2 values seperated by spaces. Current number of values is " << count << ", string is '" << string_value << "'. Ignoring." << std::endl;
    }
}

void APTracer::get_xy(const std::string &string_value, unsigned int (&value)[2]) {
    unsigned int count = 0;
    std::stringstream ss(string_value);

    for(std::string s; ss >> s; ){
        if (count < 2){
            value[count] = std::stoi(s);
        }
        ++count;
    }

    if (count != 2) {
        std::cerr << "Error, xy should be 2 values seperated by spaces. Current number of values is " << count << ", string is '" << string_value << "'. Ignoring." << std::endl;
    }
}

bool APTracer::is_number(const std::string& s) {
    return !s.empty() && std::find_if(s.begin(), 
        s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

void APTracer::apply_transformation(TransformMatrix_t* transform_matrix, const tinyxml2::XMLElement* transform) {
    std::string type;
    const char* type_char = transform->Attribute("type");
    if (type_char == nullptr) {
        std::cerr << "Error: XML transforms should have a 'type' attribute. Ignoring." << std::endl;
        return;
    }
    else {
        type = type_char;
    }
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);

    if (type == "rotatexaxis"){
        const char* attributes[] = {"value"};
        require_attributes(transform, attributes, 1);
        transform_matrix->rotateXAxis(transform->DoubleAttribute("value"));
    }
    else if (type == "rotateyaxis"){
        const char* attributes[] = {"value"};
        require_attributes(transform, attributes, 1);
        transform_matrix->rotateYAxis(transform->DoubleAttribute("value"));
    }
    else if (type == "rotatezaxis"){
        const char* attributes[] = {"value"};
        require_attributes(transform, attributes, 1);
        transform_matrix->rotateZAxis(transform->DoubleAttribute("value"));
    }
    else if (type == "rotatex"){
        const char* attributes[] = {"value"};
        require_attributes(transform, attributes, 1);
        transform_matrix->rotateX(transform->DoubleAttribute("value"));
    }
    else if (type == "rotatey"){
        const char* attributes[] = {"value"};
        require_attributes(transform, attributes, 1);
        transform_matrix->rotateY(transform->DoubleAttribute("value"));
    }
    else if (type == "rotatez"){
        const char* attributes[] = {"value"};
        require_attributes(transform, attributes, 1);
        transform_matrix->rotateZ(transform->DoubleAttribute("value"));
    }
    else if (type == "rotateaxis"){
        const char* attributes[] = {"value", "axis"};
        require_attributes(transform, attributes, 2);
        transform_matrix->rotateAxis(APTracer::get_colour(transform->Attribute("axis")), transform->DoubleAttribute("value"));
    }
    else if (type == "rotate"){
        const char* attributes[] = {"value", "axis"};
        require_attributes(transform, attributes, 2);
        transform_matrix->rotate(APTracer::get_colour(transform->Attribute("axis")), transform->DoubleAttribute("value"));
    }
    else if (type == "translate"){
        const char* attributes[] = {"value"};
        require_attributes(transform, attributes, 1);
        transform_matrix->translate(APTracer::get_colour(transform->Attribute("value")));
    }
    else if (type == "scaleaxis"){
        const char* attributes[] = {"value"};
        require_attributes(transform, attributes, 1);
        transform_matrix->scaleAxis(APTracer::get_colour(transform->Attribute("value")));
    }
    else if (type == "uniformscaleaxis"){
        const char* attributes[] = {"value"};
        require_attributes(transform, attributes, 1);
        transform_matrix->scaleAxis(transform->DoubleAttribute("value"));
    }
    else if (type == "scale"){
        const char* attributes[] = {"value"};
        require_attributes(transform, attributes, 1);
        transform_matrix->scale(APTracer::get_colour(transform->Attribute("value")));
    }
    else if (type == "uniformscale"){
        const char* attributes[] = {"value"};
        require_attributes(transform, attributes, 1);
        transform_matrix->scale(transform->DoubleAttribute("value"));
    }
    else if (type == "reflect"){
        const char* attributes[] = {"value"};
        require_attributes(transform, attributes, 1);
        transform_matrix->reflect(APTracer::get_colour(transform->Attribute("value")));
    }
    else if (type == "shear"){
        const char* attributes[] = {"value"};
        require_attributes(transform, attributes, 1);
        transform_matrix->shear(APTracer::get_colour(transform->Attribute("value")));
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
        std::cerr << "Error, transform type '" << type << "' not implemented. Ignoring." << std::endl; 
    }
}

void APTracer::require_attributes(const tinyxml2::XMLElement* element, const char** attributes, unsigned int n) {
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
                std::cerr << "Error, " << element->Name() << " XML element with name '" << name << "' and type '" << type << "' has the following attributes missing:" << std::endl;
            }
            std::cout << attributes[i] << std::endl;
        }        
    }

    if (missing) {
        exit(2); 
    }
}