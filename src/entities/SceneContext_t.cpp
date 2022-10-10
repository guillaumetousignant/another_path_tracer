#include "entities/SceneContext_t.h"

#include <iostream>
#include <string>
#include <sstream> 
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
#include "materials/Portal_t.h"
#include "materials/NormalMaterial_t.h"
#include "materials/NormalDiffuseMaterial_t.h"
#include "materials/FresnelMix_t.h"
#include "materials/FresnelMixNormal_t.h"
#include "materials/FresnelMixIn_t.h"
#include "materials/RandomMix_t.h"
#include "materials/RandomMixIn_t.h"
#include "materials/BounceMaterial_t.h"
#include "materials/DistanceMaterial_t.h"
#include "materials/TextureMix_t.h"
#include "materials/TextureMixIn_t.h"
#include "materials/Toon_t.h"
#include "entities/MaterialMap_t.h"

#include "materials/NonAbsorber_t.h"
#include "materials/Absorber_t.h"
#include "materials/Scatterer_t.h"
#include "materials/ScattererExp_t.h"
#include "materials/ScattererFull_t.h"
#include "materials/ScattererExpFull_t.h"
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
#include "cameras/FishCam_t.h"
#include "cameras/FishCamAperture_t.h"
#include "cameras/FishCamMotionblur_t.h"
#include "cameras/FishCamMotionblurAperture_t.h"
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

APTracer::Entities::SceneContext_t::SceneContext_t() :
    use_gl_(false), opengl_renderer_(), opengl_imgbuffer_(nullptr), 
    opengl_camera_(nullptr), scene_(), index_transform_matrices_(0), index_textures_(0), 
    index_mediums_(0), index_materials_(0), index_mesh_geometries_(0), index_objects_(0), 
    index_directional_lights_(0), index_skyboxes_(0), index_imgbuffers_(0), index_cameras_(0)
    {}

APTracer::Entities::SceneContext_t::~SceneContext_t() {
    reset();
}

auto APTracer::Entities::SceneContext_t::readXML(const std::string &filename) -> void {
    reset();

    std::string new_filename;
    std::string folder;

    #ifdef _WIN32
        folder = "images\\";
    #else
        folder = "images/";
    #endif

    tinyxml2::XMLDocument xml_scene;
	xml_scene.LoadFile(filename.c_str());
    if (xml_scene.Error()) {
        std::cerr << "Error: There was an error while opening or parsing file '" << filename << "'. Exiting." << std::endl;
        return;
    }

    tinyxml2::XMLElement* xml_top = xml_scene.FirstChildElement();
    const char* temp_name = xml_top->Attribute("name");
    if (temp_name == nullptr) {
        std::cerr << "Warning: Scene name not found. XML scene should have a 'name' attribute. Using 'null'." << std::endl;
        scene_name_ = "null";
    }
    else {
        scene_name_ = temp_name;
    }
    new_filename = next_filename(folder + scene_name_ + ".png");

    std::cout << "Scene name: '" << scene_name_ << "'." << std::endl;
    std::cout << "Next filename: '" << new_filename << "'." << std::endl;

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

    size_t n_transform_matrices = 0;
    size_t n_textures = 0;
    size_t n_mediums = 0;
    size_t n_materials = 0;
    size_t n_mesh_geometries = 0;
    size_t n_objects = 0;
    size_t n_directional_lights = 0;
    size_t n_skyboxes = 0;
    size_t n_imgbuffers = 0;
    size_t n_cameras = 0;          
    
    // Counts
    if (xml_transform_matrices != nullptr) {
        for (tinyxml2::XMLElement* xml_transform_matrix = xml_transform_matrices->FirstChildElement("transform_matrix"); xml_transform_matrix; xml_transform_matrix = xml_transform_matrix->NextSiblingElement("transform_matrix")) {
            ++n_transform_matrices;
        }
    }

    if (xml_textures != nullptr) {
        for (tinyxml2::XMLElement* xml_texture = xml_textures->FirstChildElement("texture"); xml_texture; xml_texture = xml_texture->NextSiblingElement("texture")) {
            ++n_textures;
        }
    }

    if (xml_mediums != nullptr) {
        for (tinyxml2::XMLElement* xml_medium = xml_mediums->FirstChildElement("medium"); xml_medium; xml_medium = xml_medium->NextSiblingElement("medium")) {
            ++n_mediums;
            const char* char_transform_matrix = xml_medium->Attribute("transform_matrix");
            if (char_transform_matrix != nullptr) {
                std::string string_transform_matrix = char_transform_matrix;
                std::transform(string_transform_matrix.begin(), string_transform_matrix.end(), string_transform_matrix.begin(), ::tolower);
                if (string_transform_matrix == "nan") {
                    ++n_transform_matrices;
                }
            }
        }
    }

    if (xml_materials != nullptr) {
        for (tinyxml2::XMLElement* xml_material = xml_materials->FirstChildElement("material"); xml_material; xml_material = xml_material->NextSiblingElement("material")) {
            ++n_materials;
            const char* char_transform_matrix = xml_material->Attribute("transform_matrix");
            if (char_transform_matrix != nullptr) {
                std::string string_transform_matrix = char_transform_matrix;
                std::transform(string_transform_matrix.begin(), string_transform_matrix.end(), string_transform_matrix.begin(), ::tolower);
                if (string_transform_matrix == "nan") {
                    ++n_transform_matrices;
                }
            }
        }
    }

    if (xml_mesh_geometries != nullptr) {
        for (tinyxml2::XMLElement* xml_mesh_geometry = xml_mesh_geometries->FirstChildElement("mesh_geometry"); xml_mesh_geometry; xml_mesh_geometry = xml_mesh_geometry->NextSiblingElement("mesh_geometry")) {
            ++n_mesh_geometries;
        }
    }

    if (xml_objects != nullptr) {
        for (tinyxml2::XMLElement* xml_object = xml_objects->FirstChildElement("object"); xml_object; xml_object = xml_object->NextSiblingElement("object")) {
            ++n_objects;
            const char* char_transform_matrix = xml_object->Attribute("transform_matrix");
            if (char_transform_matrix != nullptr) {
                std::string string_transform_matrix = char_transform_matrix;
                std::transform(string_transform_matrix.begin(), string_transform_matrix.end(), string_transform_matrix.begin(), ::tolower);
                if (string_transform_matrix == "nan") {
                    ++n_transform_matrices;
                }
            }
        }
    }

    if (xml_directional_lights != nullptr) {
        for (tinyxml2::XMLElement* xml_directional_light = xml_directional_lights->FirstChildElement("directional_light"); xml_directional_light; xml_directional_light = xml_directional_light->NextSiblingElement("directional_light")) {
            ++n_directional_lights;
            const char* char_transform_matrix = xml_directional_light->Attribute("transform_matrix");
            if (char_transform_matrix != nullptr) {
                std::string string_transform_matrix = char_transform_matrix;
                std::transform(string_transform_matrix.begin(), string_transform_matrix.end(), string_transform_matrix.begin(), ::tolower);
                if (string_transform_matrix == "nan") {
                    ++n_transform_matrices;
                }
            }
        }
    }

    if (xml_skyboxes != nullptr) {
        for (tinyxml2::XMLElement* xml_skybox = xml_skyboxes->FirstChildElement("skybox"); xml_skybox; xml_skybox = xml_skybox->NextSiblingElement("skybox")) {
            ++n_skyboxes;
            const char* char_transform_matrix = xml_skybox->Attribute("transform_matrix");
            if (char_transform_matrix != nullptr) {
                std::string string_transform_matrix = char_transform_matrix;
                std::transform(string_transform_matrix.begin(), string_transform_matrix.end(), string_transform_matrix.begin(), ::tolower);
                if (string_transform_matrix == "nan") {
                    ++n_transform_matrices;
                }
            }
        }
    }

    if (xml_imgbuffers != nullptr) {
        for (tinyxml2::XMLElement* xml_imgbuffer = xml_imgbuffers->FirstChildElement("imgbuffer"); xml_imgbuffer; xml_imgbuffer = xml_imgbuffer->NextSiblingElement("imgbuffer")) {
            ++n_imgbuffers;
        }
    }

    if (xml_cameras != nullptr) {
        for (tinyxml2::XMLElement* xml_camera = xml_cameras->FirstChildElement("camera"); xml_camera; xml_camera = xml_camera->NextSiblingElement("camera")) {
            ++n_cameras;
            const char* char_transform_matrix = xml_camera->Attribute("transform_matrix");
            if (char_transform_matrix != nullptr) {
                std::string string_transform_matrix = char_transform_matrix;
                std::transform(string_transform_matrix.begin(), string_transform_matrix.end(), string_transform_matrix.begin(), ::tolower);
                if (string_transform_matrix == "nan") {
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
 
    std::list<std::pair<APTracer::Materials::PortalScatterer_t*, std::list<size_t>>> mediums_medium_list;  
    MaterialMixLists material_mix_list;
    std::list<std::pair<APTracer::Materials::Portal_t*, std::list<size_t>>> materials_medium_list;
    std::vector<
        std::unique_ptr<std::tuple<
            std::unique_ptr<std::list<size_t>>, 
            std::unique_ptr<std::list<std::string>>
        >>
    > materials_aggregate_list (n_materials);
    MaterialTransformations material_transformations;
    MediumTransformations medium_transformations;

    std::cout << "Buffers allocated." << std::endl << std::endl;

    // Filling buffers
    // Transform matrices (1)
    if (xml_transform_matrices != nullptr) {
        for (tinyxml2::XMLElement* xml_transform_matrix = xml_transform_matrices->FirstChildElement("transform_matrix"); xml_transform_matrix; xml_transform_matrix = xml_transform_matrix->NextSiblingElement("transform_matrix")) {
            transform_matrices_[index_transform_matrices_] = create_transform_matrix(xml_transform_matrix);
            ++index_transform_matrices_;
        }
        std::cout << "Transform matrices created." << std::endl;
    }

    // Textures (2)
    if (xml_textures != nullptr) {
        for (tinyxml2::XMLElement* xml_texture = xml_textures->FirstChildElement("texture"); xml_texture; xml_texture = xml_texture->NextSiblingElement("texture")) {
            textures_[index_textures_] = create_texture(xml_texture);
            ++index_textures_;
        }
        std::cout << "Textures created." << std::endl;
    }

    // Mediums (3)
    if (xml_mediums != nullptr) {
        for (tinyxml2::XMLElement* xml_medium = xml_mediums->FirstChildElement("medium"); xml_medium; xml_medium = xml_medium->NextSiblingElement("medium")) {
            mediums_[index_mediums_] = create_medium(xml_medium, mediums_medium_list, medium_transformations, xml_transform_matrices, xml_mediums);
            ++index_mediums_;
        }
        std::cout << "Mediums created." << std::endl;
    }

    // Materials (4)
    if (xml_materials != nullptr) {
        for (tinyxml2::XMLElement* xml_material = xml_materials->FirstChildElement("material"); xml_material; xml_material = xml_material->NextSiblingElement("material")) {
            materials_[index_materials_] = create_material(xml_material, materials_medium_list, material_mix_list, materials_aggregate_list[index_materials_], material_transformations, xml_textures, xml_transform_matrices, xml_materials, xml_mediums);
            ++index_materials_;
        }
        std::cout << "Materials created." << std::endl;
    }

    // Fixes 
    // Material mixes fix
    for (const auto& fresnel_mix_list: material_mix_list.fresnel_mix) {
        auto* material_mix = fresnel_mix_list.first;
        material_mix->material_refracted_ = materials_[fresnel_mix_list.second[0]].get();
        material_mix->material_reflected_ = materials_[fresnel_mix_list.second[1]].get();
    }
    for (const auto& fresnel_mix_list: material_mix_list.fresnel_mix_in) {
        auto* material_mix = fresnel_mix_list.first;
        material_mix->material_refracted_ = materials_[fresnel_mix_list.second[0]].get();
        material_mix->material_reflected_ = materials_[fresnel_mix_list.second[1]].get();
    }
    for (const auto& fresnel_mix_list: material_mix_list.fresnel_mix_normal) {
        auto* material_mix = fresnel_mix_list.first;
        material_mix->material_refracted_ = materials_[fresnel_mix_list.second[0]].get();
        material_mix->material_reflected_ = materials_[fresnel_mix_list.second[1]].get();
    }
    for (const auto& fresnel_mix_list: material_mix_list.random_mix) {
        auto* material_mix = fresnel_mix_list.first;
        material_mix->first_material_ = materials_[fresnel_mix_list.second[0]].get();
        material_mix->second_material_ = materials_[fresnel_mix_list.second[1]].get();
    }
    for (const auto& fresnel_mix_list: material_mix_list.random_mix_in) {
        auto* material_mix = fresnel_mix_list.first;
        material_mix->first_material_ = materials_[fresnel_mix_list.second[0]].get();
        material_mix->second_material_ = materials_[fresnel_mix_list.second[1]].get();
    }
    for (const auto& fresnel_mix_list: material_mix_list.texture_mix) {
        auto* material_mix = fresnel_mix_list.first;
        material_mix->first_material_ = materials_[fresnel_mix_list.second[0]].get();
        material_mix->second_material_ = materials_[fresnel_mix_list.second[1]].get();
    }
    for (const auto& fresnel_mix_list: material_mix_list.texture_mix_in) {
        auto* material_mix = fresnel_mix_list.first;
        material_mix->first_material_ = materials_[fresnel_mix_list.second[0]].get();
        material_mix->second_material_ = materials_[fresnel_mix_list.second[1]].get();
    }

    // Materials medium list fix
    for (const auto& portal_list: materials_medium_list) {
        auto* portal = portal_list.first;
        for (auto medium_index: portal_list.second) {
            portal->medium_list_.push_back(mediums_[medium_index].get());
        }
    }

    // Mediums medium list fix
    for (const auto& portal_scatterer_list: mediums_medium_list) {
        auto* portal_scatterer = portal_scatterer_list.first;
        for (auto medium_index: portal_scatterer_list.second) {
            portal_scatterer->medium_list_.push_back(mediums_[medium_index].get());
        }
    }

    // Material aggregates fix
    for (size_t i = 0; i < materials_.size(); i++) {
        if (materials_aggregate_list[i]) {
            size_t n = std::get<0>(*materials_aggregate_list[i])->size();
            std::vector<std::string> names(n);
            std::vector<Material_t*> materials(n);

            size_t index = 0;
            for  (auto material_index: *std::get<0>(*materials_aggregate_list[i])) {
                materials[index] = materials_[material_index].get();
                ++index;
            }

            index = 0;
            for (const auto &material_name : *std::get<1>(*materials_aggregate_list[i])) {
                names[index] = material_name;
                ++index;
            }
            
            material_aggregates_[i] = std::make_unique<MaterialMap_t>(names.data(), materials.data(), n);
            //materials_[i] = std::unique_ptr<Material_t>(material_aggregates_[i]->getFirst()); // CHECK this is not intended, will delete stuff from material list
            materials_[i] = std::unique_ptr<Material_t>();
        }
    }

    std::cout << "\tFixes done." << std::endl;

    // Filling buffers again
    // Mesh geometries (5)
    if (xml_mesh_geometries != nullptr) {
        for (tinyxml2::XMLElement* xml_mesh_geometry = xml_mesh_geometries->FirstChildElement("mesh_geometry"); xml_mesh_geometry; xml_mesh_geometry = xml_mesh_geometry->NextSiblingElement("mesh_geometry")) {
            mesh_geometries_[index_mesh_geometries_] = create_mesh_geometry(xml_mesh_geometry);
            ++index_mesh_geometries_;
        }
        std::cout << "Mesh geometries created." << std::endl;
    }

    // Objects (6)
    if (xml_objects != nullptr) {
        for (tinyxml2::XMLElement* xml_object = xml_objects->FirstChildElement("object"); xml_object; xml_object = xml_object->NextSiblingElement("object")) {
            objects_[index_objects_] = create_object(xml_object, meshes_[index_objects_], xml_transform_matrices, xml_materials, xml_mesh_geometries);
            ++index_objects_;
        }
        std::cout << "Objects created." << std::endl;
    }

    // Directional lights (7)
    if (xml_directional_lights != nullptr) {
        for (tinyxml2::XMLElement* xml_directional_light = xml_directional_lights->FirstChildElement("directional_light"); xml_directional_light; xml_directional_light = xml_directional_light->NextSiblingElement("directional_light")) {
            directional_lights_[index_directional_lights_] = create_directional_light(xml_directional_light, xml_transform_matrices);
            ++index_directional_lights_;
        }
        std::cout << "Directional lights created." << std::endl;
    }

    // Skyboxes (8)
    if (xml_skyboxes != nullptr) {
        for (tinyxml2::XMLElement* xml_skybox = xml_skyboxes->FirstChildElement("skybox"); xml_skybox; xml_skybox = xml_skybox->NextSiblingElement("skybox")) {
            skyboxes_[index_skyboxes_] = create_skybox(xml_skybox, xml_textures, xml_transform_matrices, xml_directional_lights);
            ++index_skyboxes_;
        }
        std::cout << "Skyboxes created." << std::endl;
    }

    // Image buffers (9)
    if (xml_imgbuffers != nullptr) {
        for (tinyxml2::XMLElement* xml_imgbuffer = xml_imgbuffers->FirstChildElement("imgbuffer"); xml_imgbuffer; xml_imgbuffer = xml_imgbuffer->NextSiblingElement("imgbuffer")) {
            imgbuffers_[index_imgbuffers_] = create_imgbuffer(xml_imgbuffer);
            ++index_imgbuffers_;
        }
        std::cout << "Image buffers created." << std::endl;
    }

    // Cameras (10)
    if (xml_cameras != nullptr) {
        for (tinyxml2::XMLElement* xml_camera = xml_cameras->FirstChildElement("camera"); xml_camera; xml_camera = xml_camera->NextSiblingElement("camera")) {
            cameras_[index_cameras_] = create_camera(xml_camera, new_filename, xml_transform_matrices, xml_mediums, xml_imgbuffers, xml_skyboxes);
            ++index_cameras_;
        }
        std::cout << "Cameras created." << std::endl;
    }
    std::cout << std::endl;

    // Updating pre
    // Transform matrices
    if (xml_transform_matrices != nullptr) {
        size_t index = 0;
        for (tinyxml2::XMLElement* xml_transform_matrix = xml_transform_matrices->FirstChildElement("transform_matrix"); xml_transform_matrix; xml_transform_matrix = xml_transform_matrix->NextSiblingElement("transform_matrix")) {
            tinyxml2::XMLElement* transformations_pre = xml_transform_matrix->FirstChildElement("transformations_pre");
            if (transformations_pre != nullptr) {
                for (tinyxml2::XMLElement* transformation_pre = transformations_pre->FirstChildElement("transformation_pre"); transformation_pre; transformation_pre = transformation_pre->NextSiblingElement("transformation_pre")) {
                    apply_transformation(transform_matrices_[index].get(), transformation_pre);
                }
            }
            ++index;
        }
        std::cout << "Transform matrices transformed." << std::endl;
    }

    // Materials
    if (!material_transformations.transformations_pre.empty()) {
        for (const auto& transformations_pre: material_transformations.transformations_pre) {
            auto* portal = transformations_pre.first;
            for (const tinyxml2::XMLElement* transformation_pre = transformations_pre.second->FirstChildElement("transformation_pre"); transformation_pre; transformation_pre = transformation_pre->NextSiblingElement("transformation_pre")) {
                apply_transformation(portal->transformation_, transformation_pre);
            }
        }
        std::cout << "Materials transformed." << std::endl;
    }

    // Mediums
    if (!medium_transformations.transformations_pre.empty()) {
        for (const auto& transformations_pre: medium_transformations.transformations_pre) {
            auto* portal_scatterer = transformations_pre.first;
            for (const tinyxml2::XMLElement* transformation_pre = transformations_pre.second->FirstChildElement("transformation_pre"); transformation_pre; transformation_pre = transformation_pre->NextSiblingElement("transformation_pre")) {
                apply_transformation(portal_scatterer->transformation_, transformation_pre);
            }
        }
        std::cout << "Mediums transformed." << std::endl;
    }

    // Directional lights
    if (xml_directional_lights != nullptr) {
        size_t index = 0;
        for (tinyxml2::XMLElement* xml_directional_light = xml_directional_lights->FirstChildElement("directional_light"); xml_directional_light; xml_directional_light = xml_directional_light->NextSiblingElement("directional_light")) {
            tinyxml2::XMLElement* transformations_pre = xml_directional_light->FirstChildElement("transformations_pre");
            if (transformations_pre != nullptr) {
                for (tinyxml2::XMLElement* transformation_pre = transformations_pre->FirstChildElement("transformation_pre"); transformation_pre; transformation_pre = transformation_pre->NextSiblingElement("transformation_pre")) {
                    apply_transformation(directional_lights_[index]->transformation_, transformation_pre);
                }
            }
            ++index;
        }
        std::cout << "Directional lights transformed." << std::endl;
    }

    // Objects
    if (xml_objects != nullptr) {
        size_t index = 0;
        for (tinyxml2::XMLElement* xml_object = xml_objects->FirstChildElement("object"); xml_object; xml_object = xml_object->NextSiblingElement("object")) {
            tinyxml2::XMLElement* transformations_pre = xml_object->FirstChildElement("transformations_pre");
            if (transformations_pre != nullptr) {
                TransformMatrix_t* transform_matrix = (objects_[index] != nullptr) ? objects_[index]->transformation_ : meshes_[index]->transformation_;
                for (tinyxml2::XMLElement* transformation_pre = transformations_pre->FirstChildElement("transformation_pre"); transformation_pre; transformation_pre = transformation_pre->NextSiblingElement("transformation_pre")) {
                    apply_transformation(transform_matrix, transformation_pre);
                }
            }
            ++index;
        }
        std::cout << "Shapes transformed." << std::endl;
    }

    // Cameras
    if (xml_cameras != nullptr) {
        size_t index = 0;
        for (tinyxml2::XMLElement* xml_camera = xml_cameras->FirstChildElement("camera"); xml_camera; xml_camera = xml_camera->NextSiblingElement("camera")) {
            tinyxml2::XMLElement* transformations_pre = xml_camera->FirstChildElement("transformations_pre");
            if (transformations_pre != nullptr) {
                for (tinyxml2::XMLElement* transformation_pre = transformations_pre->FirstChildElement("transformation_pre"); transformation_pre; transformation_pre = transformation_pre->NextSiblingElement("transformation_pre")) {
                    apply_transformation(cameras_[index]->transformation_, transformation_pre);
                }
            }
            ++index;
        }
        std::cout << "Cameras transformed." << std::endl;
    }
    std::cout << std::endl;

    // Scene building
    scene_ = std::make_unique<Scene_t>();
    const char* object_list = xml_top->Attribute("object_list");
    std::cout << "Scene created." << std::endl;
    
    if (!objects_.empty()) {
        std::vector<Shape_t*> shapes;
        std::vector<MeshTop_t*> meshes;

        if (object_list != nullptr) {
            get_objects(object_list, shapes, meshes, xml_objects);
        }
        else {
            get_objects(shapes, meshes);
        }

        scene_->add(shapes);
        scene_->add(meshes);

        std::cout << "Primitives added." << std::endl;
    }

    // Update
    std::cout << std::endl << "Updating scene..." << std::endl;
    auto t_start = std::chrono::high_resolution_clock::now();
    scene_->update();
    for (auto &camera : cameras_) {
        camera->update();
    }
    auto t_end = std::chrono::high_resolution_clock::now();
    std::cout << "Scene updated in " 
        << std::chrono::duration<double, std::milli>(t_end-t_start).count()/1000.0 
        << "s." << std::endl << std::endl;

    // Updating post
    // Transform matrices
    if (xml_transform_matrices != nullptr) {
        size_t index = 0;
        for (tinyxml2::XMLElement* xml_transform_matrix = xml_transform_matrices->FirstChildElement("transform_matrix"); xml_transform_matrix; xml_transform_matrix = xml_transform_matrix->NextSiblingElement("transform_matrix")) {
            tinyxml2::XMLElement* transformations_post = xml_transform_matrix->FirstChildElement("transformations_post");
            if (transformations_post != nullptr) {
                for (tinyxml2::XMLElement* transformation_post = transformations_post->FirstChildElement("transformation_post"); transformation_post; transformation_post = transformation_post->NextSiblingElement("transformation_post")) {
                    apply_transformation(transform_matrices_[index].get(), transformation_post);
                }
            }
            ++index;
        }
        std::cout << "Transform matrices transformed post." << std::endl;
    }

    // Materials
    if (!material_transformations.transformations_post.empty()) {
        for (const auto& transformations_post: material_transformations.transformations_post) {
            auto* portal = transformations_post.first;
            for (const tinyxml2::XMLElement* transformation_post = transformations_post.second->FirstChildElement("transformation_post"); transformation_post; transformation_post = transformation_post->NextSiblingElement("transformation_post")) {
                apply_transformation(portal->transformation_, transformation_post);
            }
        }
        std::cout << "Materials transformed post." << std::endl;
    }

    // Mediums
    if (!medium_transformations.transformations_post.empty()) {
        for (const auto& transformations_post: medium_transformations.transformations_post) {
            auto* portal_scatterer = transformations_post.first;
            for (const tinyxml2::XMLElement* transformation_post = transformations_post.second->FirstChildElement("transformation_post"); transformation_post; transformation_post = transformation_post->NextSiblingElement("transformation_post")) {
                apply_transformation(portal_scatterer->transformation_, transformation_post);
            }
        }
        std::cout << "Materials transformed post." << std::endl;
    }

    // Directional lights
    if (xml_directional_lights != nullptr) {
        size_t index = 0;
        for (tinyxml2::XMLElement* xml_directional_light = xml_directional_lights->FirstChildElement("directional_light"); xml_directional_light; xml_directional_light = xml_directional_light->NextSiblingElement("directional_light")) {
            tinyxml2::XMLElement* transformations_post = xml_directional_light->FirstChildElement("transformations_post");
            if (transformations_post != nullptr) {
                for (tinyxml2::XMLElement* transformation_post = transformations_post->FirstChildElement("transformation_post"); transformation_post; transformation_post = transformation_post->NextSiblingElement("transformation_post")) {
                    apply_transformation(directional_lights_[index]->transformation_, transformation_post);
                }
            }
            ++index;
        }
        std::cout << "Directional lights transformed post." << std::endl;
    }

    // Objects
    if (xml_objects != nullptr) {
        size_t index = 0;
        for (tinyxml2::XMLElement* xml_object = xml_objects->FirstChildElement("object"); xml_object; xml_object = xml_object->NextSiblingElement("object")) {
            tinyxml2::XMLElement* transformations_post = xml_object->FirstChildElement("transformations_post");
            if (transformations_post != nullptr) {
                TransformMatrix_t* transform_matrix = (objects_[index] != nullptr) ? objects_[index]->transformation_ : meshes_[index]->transformation_;
                for (tinyxml2::XMLElement* transformation_post = transformations_post->FirstChildElement("transformation_post"); transformation_post; transformation_post = transformation_post->NextSiblingElement("transformation_post")) {
                    apply_transformation(transform_matrix, transformation_post);
                }
            }
            ++index;
        }
        std::cout << "Shapes transformed post." << std::endl;
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
    const tinyxml2::XMLElement* xml_acceleration_structure = (xml_acceleration_structures != nullptr) ? xml_acceleration_structures->FirstChildElement("acceleration_structure") : nullptr;

    t_start = std::chrono::high_resolution_clock::now();
    create_acceleration_structure(xml_acceleration_structure);
    t_end = std::chrono::high_resolution_clock::now();

    std::cout << "Acceleration structure built in " 
        << std::chrono::duration<double, std::milli>(t_end-t_start).count()/1000.0 
        << "s." << std::endl << std::endl;

    // Autofocus
    if (xml_cameras != nullptr) {
        size_t index = 0;
        for (tinyxml2::XMLElement* xml_camera = xml_cameras->FirstChildElement("camera"); xml_camera; xml_camera = xml_camera->NextSiblingElement("camera")) {
            const char* focus_distance_char = xml_camera->Attribute("focus_distance");
            if (focus_distance_char != nullptr) {
                std::string focus_distance = focus_distance_char;
                std::transform(focus_distance.begin(), focus_distance.end(), focus_distance.begin(), ::tolower);
                if (focus_distance == "nan") {
                    const char* focus_position_char = xml_camera->Attribute("focus_position");
                    std::string focus_position;
                    if (focus_position_char == nullptr) {
                        std::cerr << "Warning: Cameras with nan as 'focus_distance' attribute should have a 'focus_position' attribute. Using 0.5 0.5." << std::endl;
                        focus_position = "0.5 0.5";
                    }
                    else {
                        focus_position = focus_position_char;
                    }
                    cameras_[index]->autoFocus(scene_.get(), get_xy<double>(focus_position));
                    cameras_[index]->update();
                    std::cout << "Camera #" << index << " autofocus." << std::endl;
                }
            }
            ++index;
        }
    }

    // Cameras
    if (xml_cameras != nullptr) {
        size_t index = 0;
        for (tinyxml2::XMLElement* xml_camera = xml_cameras->FirstChildElement("camera"); xml_camera; xml_camera = xml_camera->NextSiblingElement("camera")) {
            tinyxml2::XMLElement* transformations_post = xml_camera->FirstChildElement("transformations_post");
            if (transformations_post != nullptr) {
                for (tinyxml2::XMLElement* transformation_post = transformations_post->FirstChildElement("transformation_post"); transformation_post; transformation_post = transformation_post->NextSiblingElement("transformation_post")) {
                    apply_transformation(cameras_[index]->transformation_, transformation_post);
                }
            }
            ++index;
        }
        std::cout << "Cameras transformed post." << std::endl;
    }

    for (auto &camera : cameras_) {
        camera->update();
    }

    // Running modes
    if (xml_cameras != nullptr) {
        if (use_gl_) {
            opengl_camera_ = cameras_[0].get(); // CHECK dunno how to fix this
            opengl_renderer_ = std::make_unique<OpenGLRenderer_t>(scene_.get(), opengl_camera_, opengl_imgbuffer_);
            opengl_renderer_->initialise();
            std::cout << "OpenGL initialised." << std::endl;
        }

        camera_rendermode_ = std::vector<std::string>(cameras_.size(), "");
        camera_n_iter_ = std::vector<unsigned int>(cameras_.size(), 0);
        camera_write_interval_ = std::vector<unsigned int>(cameras_.size(), 0);

        size_t index = 0;
        for (tinyxml2::XMLElement* xml_camera = xml_cameras->FirstChildElement("camera"); xml_camera; xml_camera = xml_camera->NextSiblingElement("camera")) {
            std::string render_mode;
            const char* render_mode_char = xml_camera->Attribute("rendermode");
            if (render_mode_char == nullptr) {
                std::cerr << "Warning: XML cameras should have a 'rendermode' attribute. Using 'single'." << std::endl;
                render_mode = "single";
            }
            else {
                render_mode = render_mode_char;
            }

            std::transform(render_mode.begin(), render_mode.end(), render_mode.begin(), ::tolower);
            
            if (render_mode == "accumulation") {
                camera_rendermode_[index] = "accumulation";
                std::string n_iter;
                const char* n_iter_char = xml_camera->Attribute("n_iter");
                if (n_iter_char == nullptr) {
                    std::cerr << "Warning: XML cameras in accumulation mode should have a 'n_iter' attribute. Using 1." << std::endl;
                    n_iter = "1";
                }
                else {
                    n_iter = n_iter_char;
                }

                std::transform(n_iter.begin(), n_iter.end(), n_iter.begin(), ::tolower);
                if (n_iter == "inf") {
                    camera_n_iter_[index] = 1000000000;
                }
                else {
                    camera_n_iter_[index] = std::stoi(n_iter);
                }              
            }
            else if (render_mode == "accumulation_write") {
                std::string n_iter;
                std::string write_interval;
                const char* n_iter_char = xml_camera->Attribute("n_iter");
                const char* write_interval_char = xml_camera->Attribute("write_interval");
                if (n_iter_char == nullptr) {
                    std::cerr << "Warning: XML cameras in accumulation mode should have a 'n_iter' attribute. Using 1." << std::endl;
                    n_iter = "1";
                }
                else {
                    n_iter = n_iter_char;
                }
                if (write_interval_char == nullptr) {
                    std::cerr << "Warning: XML cameras in accumulation_write mode should have a 'write_interval' attribute. Using 1." << std::endl;
                    write_interval = "1";
                }
                else {
                    write_interval = write_interval_char;
                }

                camera_rendermode_[index] = "accumulation_write";

                std::transform(n_iter.begin(), n_iter.end(), n_iter.begin(), ::tolower);
                if (n_iter == "inf") {
                    camera_n_iter_[index] = 0;
                }
                else {
                    camera_n_iter_[index] = std::stoi(n_iter);
                }

                std::transform(write_interval.begin(), write_interval.end(), write_interval.begin(), ::tolower);
                if (write_interval == "inf") {
                    camera_write_interval_[index] = 0;
                }
                else {
                    camera_write_interval_[index] = std::stoi(write_interval);
                }
            }
            else if (render_mode == "single") {
                camera_rendermode_[index] = "single";
            }
            else if (render_mode == "motion") {
                std::cerr << "Warning: Motion render mode not implemented yet. Single frame render fallback." << std::endl;
                camera_rendermode_[index] = "single";
            }
            else {
                std::cerr << "Warning: Render mode '" << render_mode << "', used by camera #" << index << ", is unknown. Only 'accumulation', 'accumulation_write', 'single', and 'motion' exist for now. Ignoring." << std::endl;
                camera_rendermode_[index] = "";
            }

            ++index;
        }
        std::cout << "Camera render modes set." << std::endl;
    }
    std::cout << std::endl << "ReadXML done." << std::endl << std::endl;
}    

auto APTracer::Entities::SceneContext_t::render() -> void {
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
            std::cerr << "Warning: Motion render mode not implemented yet. Accumulation render fallback." << std::endl;
            opengl_renderer_->render();
        }
        else if (render_mode.empty()) {

        }
        else {
            std::cerr << "Warning: Render mode '" << render_mode << "', used by camera #" << 0 << ", is unknown. Only 'accumulation', 'accumulation_write', 'single', and 'motion' exist for now. Ignoring." << std::endl;
        }
    }
    else {
        for (size_t i = 0; i < cameras_.size(); i++) {
            std::string render_mode = camera_rendermode_[i];
            std::transform(render_mode.begin(), render_mode.end(), render_mode.begin(), ::tolower);

            std::cout << "Camera #" << i << " rendering scene '" << scene_name_ << "' in '" <<  render_mode << "' mode." << std::endl;

            if (render_mode == "accumulation") {
                if (camera_n_iter_[i] != 0) {
                    cameras_[i]->accumulate(scene_.get(), camera_n_iter_[i]);
                }
                else {
                    cameras_[i]->accumulate(scene_.get());
                }
                cameras_[i]->write();
            }
            else if (render_mode == "accumulation_write") {
                if (camera_n_iter_[i] != 0) {
                    cameras_[i]->accumulateWrite(scene_.get(), camera_n_iter_[i], camera_write_interval_[i]);
                }
                else {
                    cameras_[i]->accumulateWrite(scene_.get(), camera_write_interval_[i]);
                }
                cameras_[i]->write();
            }
            else if (render_mode == "single") {
                cameras_[i]->raytrace(scene_.get());
                cameras_[i]->write();
            }
            else if (render_mode == "motion") {
                std::cerr << "Warning: Motion render mode not implemented yet. Single frame render fallback." << std::endl;
                cameras_[i]->raytrace(scene_.get());
                cameras_[i]->write();
            }
            else if (render_mode.empty()) {

            }
            else {
                std::cerr << "Warning: Render mode '" << render_mode << "', used by camera #" << i << ", is unknown. Only 'accumulation', 'accumulation_write', 'single', and 'motion' exist for now. Ignoring." << std::endl;
            }
        }
    }
}

auto APTracer::Entities::SceneContext_t::reset() -> void {
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

auto APTracer::Entities::SceneContext_t::create_transform_matrix(const tinyxml2::XMLElement* xml_transform_matrix) -> std::unique_ptr<TransformMatrix_t> {
    std::string string_transform_matrix;
    const char* transform_matrix_char = xml_transform_matrix->Attribute("value");
    if (transform_matrix_char == nullptr) {
        std::cerr << "Warning: XML transform matrices should have a 'value' attribute. Using identity." << std::endl;
        string_transform_matrix = "nan";
    }
    else {
        string_transform_matrix = transform_matrix_char;
    }

    std::transform(string_transform_matrix.begin(), string_transform_matrix.end(), string_transform_matrix.begin(), ::tolower);

    if (string_transform_matrix == "nan") {
        return std::make_unique<TransformMatrix_t>();
    }

    std::array<double, 16> values;
    unsigned int count = 0;
    std::stringstream ss(string_transform_matrix);
    
    for(std::string s; ss >> s; ) {
        if (count < 16) {
            values[count] = std::stod(s);
        }
        ++count;
    }
    if (count != 16) {
        std::cerr << "Warning: Transform matrix value should be 16 values seperated by spaces, or nan. Current number of values is " << count << ". Using identity." << std::endl;
        return std::make_unique<TransformMatrix_t>();
    }
    
    return std::make_unique<TransformMatrix_t>(values);
}

auto APTracer::Entities::SceneContext_t::create_texture(const tinyxml2::XMLElement* xml_texture) -> std::unique_ptr<Texture_t>{
    std::string type;
    const char* type_char = xml_texture->Attribute("type");
    if (type_char == nullptr) {
        std::cerr << "Warning: XML textures should have a 'type' attribute. Using 'texture'." << std::endl;
        type = "texture";
    }
    else {
        type = type_char;
    }
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);

    if (type == "texture") {
        const std::vector<const char*> attributes = {"filename"};
        require_attributes(xml_texture, attributes);
        std::string filename = xml_texture->Attribute("filename");
        for (auto& character: filename) {
            #ifdef _WIN32
                if (character == '/') {
                    character = '\\';
                }
            #else
                if (character == '\\') {
                    character = '/';
                }
            #endif
        }
        
        return std::make_unique<Texture_t>(filename);
    }

    std::cerr << "Error: Texture type '" << type << "' not implemented. Only 'texture' exists for now. Exiting." << std::endl; 
    exit(20);
}

auto APTracer::Entities::SceneContext_t::create_medium(const tinyxml2::XMLElement* xml_medium, std::list<std::pair<APTracer::Materials::PortalScatterer_t*, std::list<size_t>>> &mediums_medium_list,  MediumTransformations &medium_transformations, const tinyxml2::XMLElement* xml_transform_matrices, const tinyxml2::XMLElement* xml_mediums) -> std::unique_ptr<Medium_t> {
    std::string type;
    const char* type_char = xml_medium->Attribute("type");
    if (type_char == nullptr) {
        std::cerr << "Warning: XML mediums should have a 'type' attribute. Using 'nonabsorber'." << std::endl;
        type = "nonabsorber";
    }
    else {
        type = type_char;
    }
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);

    if (type == "absorber") {
        const std::vector<const char*> attributes = {"emission", "colour", "emission_distance", "absorption_distance", "ind", "priority"};
        require_attributes(xml_medium, attributes);
        return std::make_unique<APTracer::Materials::Absorber_t>(
                                APTracer::get_colour(xml_medium->Attribute("emission")), APTracer::get_colour(xml_medium->Attribute("colour")), 
                                xml_medium->DoubleAttribute("emission_distance"), xml_medium->DoubleAttribute("absorption_distance"),
                                xml_medium->DoubleAttribute("ind"), xml_medium->UnsignedAttribute("priority"));
    }
    if (type == "nonabsorber") {
        const std::vector<const char*> attributes = {"ind", "priority"};
        require_attributes(xml_medium, attributes);
        return std::make_unique<APTracer::Materials::NonAbsorber_t>(xml_medium->DoubleAttribute("ind"), xml_medium->UnsignedAttribute("priority"));
    }
    if (type == "portal_scatterer") {
        // CHECK add medium_list stuff
        const std::vector<const char*> attributes = {"medium_list", "transform_matrix", "scattering_distance", "ind", "priority"};
        require_attributes(xml_medium, attributes);
        std::unique_ptr<APTracer::Materials::PortalScatterer_t> medium(std::make_unique<APTracer::Materials::PortalScatterer_t>(get_transform_matrix(xml_medium->Attribute("transform_matrix"), xml_transform_matrices), std::list<Medium_t*>(), xml_medium->DoubleAttribute("scattering_distance"),
                                xml_medium->DoubleAttribute("ind"), xml_medium->UnsignedAttribute("priority")));
        mediums_medium_list.emplace_back(medium.get(), get_medium_index_list(xml_medium->Attribute("medium_list"), xml_mediums));
        const tinyxml2::XMLElement* transformations_pre = xml_medium->FirstChildElement("transformations_pre");
        if (transformations_pre != nullptr) {
            medium_transformations.transformations_pre.emplace_back(medium.get(), transformations_pre);
        }
        const tinyxml2::XMLElement* transformations_post = xml_medium->FirstChildElement("transformations_post");
        if (transformations_post != nullptr) {
            medium_transformations.transformations_post.emplace_back(medium.get(), transformations_post);
        }
        return medium;
    }
    if (type == "scatterer_exp") {
        const std::vector<const char*> attributes = {"emission", "colour", "emission_distance", "absorption_distance", "scattering_distance", "order", "scattering_angle", "ind", "priority"};
        require_attributes(xml_medium, attributes);
        return std::make_unique<APTracer::Materials::ScattererExp_t>(
                                APTracer::get_colour(xml_medium->Attribute("emission")), APTracer::get_colour(xml_medium->Attribute("colour")),
                                xml_medium->DoubleAttribute("emission_distance"), xml_medium->DoubleAttribute("absorption_distance"),
                                xml_medium->DoubleAttribute("scattering_distance"), xml_medium->DoubleAttribute("order"), 
                                xml_medium->DoubleAttribute("scattering_angle"), xml_medium->DoubleAttribute("ind"), 
                                xml_medium->UnsignedAttribute("priority"));
    }
    if (type == "scatterer_exp_full") {
        const std::vector<const char*> attributes = {"emission", "colour", "scattering_emission", "scattering_colour", "emission_distance", "absorption_distance", "scattering_distance", "order", "scattering_angle", "ind", "priority"};
        require_attributes(xml_medium, attributes);
        return std::make_unique<APTracer::Materials::ScattererExpFull_t>(
                                APTracer::get_colour(xml_medium->Attribute("emission")), APTracer::get_colour(xml_medium->Attribute("colour")),
                                APTracer::get_colour(xml_medium->Attribute("scattering_emission")), APTracer::get_colour(xml_medium->Attribute("scattering_colour")),
                                xml_medium->DoubleAttribute("emission_distance"), xml_medium->DoubleAttribute("absorption_distance"),
                                xml_medium->DoubleAttribute("scattering_distance"), xml_medium->DoubleAttribute("order"), 
                                xml_medium->DoubleAttribute("scattering_angle"), xml_medium->DoubleAttribute("ind"), 
                                xml_medium->UnsignedAttribute("priority"));
    }
    if (type == "scatterer") {
        const std::vector<const char*> attributes = {"emission", "colour", "emission_distance", "absorption_distance", "scattering_distance", "ind", "priority"};
        require_attributes(xml_medium, attributes);
        return std::make_unique<APTracer::Materials::Scatterer_t>(
                                APTracer::get_colour(xml_medium->Attribute("emission")), APTracer::get_colour(xml_medium->Attribute("colour")),
                                xml_medium->DoubleAttribute("emission_distance"), xml_medium->DoubleAttribute("absorption_distance"),
                                xml_medium->DoubleAttribute("scattering_distance"), xml_medium->DoubleAttribute("ind"), 
                                xml_medium->UnsignedAttribute("priority"));
    }
    if (type == "scatterer_full") {
        const std::vector<const char*> attributes = {"emission", "colour", "scattering_emission", "scattering_colour", "emission_distance", "absorption_distance", "scattering_distance", "ind", "priority"};
        require_attributes(xml_medium, attributes);
        return std::make_unique<APTracer::Materials::ScattererFull_t>(
                                APTracer::get_colour(xml_medium->Attribute("emission")), APTracer::get_colour(xml_medium->Attribute("colour")),
                                APTracer::get_colour(xml_medium->Attribute("scattering_emission")), APTracer::get_colour(xml_medium->Attribute("scattering_colour")),
                                xml_medium->DoubleAttribute("emission_distance"), xml_medium->DoubleAttribute("absorption_distance"),
                                xml_medium->DoubleAttribute("scattering_distance"), xml_medium->DoubleAttribute("ind"), 
                                xml_medium->UnsignedAttribute("priority"));
    }

    std::cerr << "Warning: Medium type '" << type << "' not implemented. Only 'absorber', 'nonabsorber', 'portal_scatterer', 'scatterer_exp', and 'scatterer' exists for now. Using 'nonabsorber'." << std::endl; 
    return std::make_unique<APTracer::Materials::NonAbsorber_t>(1.0, 0);
}

auto APTracer::Entities::SceneContext_t::create_material(const tinyxml2::XMLElement* xml_material, std::list<std::pair<APTracer::Materials::Portal_t*, std::list<size_t>>> &materials_medium_list, MaterialMixLists &material_mix_lists, std::unique_ptr<std::tuple<std::unique_ptr<std::list<size_t>>, std::unique_ptr<std::list<std::string>>>> &materials_aggregate_list, MaterialTransformations &material_transformations, const tinyxml2::XMLElement* xml_textures, const tinyxml2::XMLElement* xml_transform_matrices, const tinyxml2::XMLElement* xml_materials, const tinyxml2::XMLElement* xml_mediums) -> std::unique_ptr<Material_t> {
    std::string type;
    const char* type_char = xml_material->Attribute("type");
    if (type_char == nullptr) {
        std::cerr << "Warning: XML materials should have a 'type' attribute. Using 'normal_material'." << std::endl;
        type = "normal_material";
    }
    else {
        type = type_char;
    }
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);

    if (type == "diffuse") {
        const std::vector<const char*> attributes = {"emission", "colour", "roughness"};
        require_attributes(xml_material, attributes);
        return std::make_unique<APTracer::Materials::Diffuse_t>(
                                APTracer::get_colour(xml_material->Attribute("emission")), APTracer::get_colour(xml_material->Attribute("colour")), 
                                xml_material->DoubleAttribute("roughness"));
    }
    if (type == "diffuse_normal") {
        const std::vector<const char*> attributes = {"emission", "colour", "normal_map", "roughness"};
        require_attributes(xml_material, attributes);
        return std::make_unique<APTracer::Materials::DiffuseNormal_t>(
                                APTracer::get_colour(xml_material->Attribute("emission")), APTracer::get_colour(xml_material->Attribute("colour")), 
                                get_texture(xml_material->Attribute("normal_map"), xml_textures), xml_material->DoubleAttribute("roughness"));
    }
    if (type == "diffuse_full") {
        const std::vector<const char*> attributes = {"emission_map", "texture", "roughness"};
        require_attributes(xml_material, attributes);
        return std::make_unique<APTracer::Materials::DiffuseFull_t>(
                                get_texture(xml_material->Attribute("emission_map"), xml_textures), get_texture(xml_material->Attribute("texture"), xml_textures), 
                                xml_material->DoubleAttribute("roughness"));
    }
    if (type == "diffuse_tex") {
        const std::vector<const char*> attributes = {"emission", "texture", "roughness"};
        require_attributes(xml_material, attributes);
        return std::make_unique<APTracer::Materials::DiffuseTex_t>(
                                APTracer::get_colour(xml_material->Attribute("emission")), get_texture(xml_material->Attribute("texture"), xml_textures), 
                                xml_material->DoubleAttribute("roughness"));
    }
    if (type == "diffuse_tex_normal") {
        const std::vector<const char*> attributes = {"emission", "texture", "normal_map", "roughness"};
        require_attributes(xml_material, attributes);
        return std::make_unique<APTracer::Materials::DiffuseTexNormal_t>(
                                APTracer::get_colour(xml_material->Attribute("emission")), get_texture(xml_material->Attribute("texture"), xml_textures), 
                                get_texture(xml_material->Attribute("normal_map"), xml_textures), xml_material->DoubleAttribute("roughness"));
    }
    if (type == "fresnelmix") {
        const std::vector<const char*> attributes = {"material_refracted", "material_reflected", "ind"};
        require_attributes(xml_material, attributes);
        std::unique_ptr<APTracer::Materials::FresnelMix_t> material(
                    std::make_unique<APTracer::Materials::FresnelMix_t>(nullptr, nullptr, xml_material->DoubleAttribute("ind")));
        material_mix_lists.fresnel_mix.emplace_back(material.get(), get_material_mix(xml_material->Attribute("material_refracted"), xml_material->Attribute("material_reflected"), xml_materials));
        return material;
    }
    if (type == "fresnelmix_normal") {
        const std::vector<const char*> attributes = {"material_refracted", "material_reflected", "ind", "normal_map"};
        require_attributes(xml_material, attributes);
        std::unique_ptr<APTracer::Materials::FresnelMixNormal_t> material(
                    std::make_unique<APTracer::Materials::FresnelMixNormal_t>(nullptr, nullptr, xml_material->DoubleAttribute("ind"), get_texture(xml_material->Attribute("normal_map"), xml_textures)));
        material_mix_lists.fresnel_mix_normal.emplace_back(material.get(), get_material_mix(xml_material->Attribute("material_refracted"), xml_material->Attribute("material_reflected"), xml_materials));
        return material;
    }
    if (type == "fresnelmix_in") {
        const std::vector<const char*> attributes = {"material_refracted", "material_reflected", "ind"};
        require_attributes(xml_material, attributes);
        std::unique_ptr<APTracer::Materials::FresnelMixIn_t> material(
                    std::make_unique<APTracer::Materials::FresnelMixIn_t>(nullptr, nullptr, xml_material->DoubleAttribute("ind")));
        material_mix_lists.fresnel_mix_in.emplace_back(material.get(), get_material_mix(xml_material->Attribute("material_refracted"), xml_material->Attribute("material_reflected"), xml_materials));
        return material;
    }
    if (type == "normal_material") {
        return std::make_unique<APTracer::Materials::NormalMaterial_t>();
    }
    if (type == "normal_diffuse_material") {
        const std::vector<const char*> attributes = {"roughness"};
        require_attributes(xml_material, attributes);
        return std::make_unique<APTracer::Materials::NormalDiffuseMaterial_t>(xml_material->DoubleAttribute("roughness"));
    }
    if (type == "portal") {
        const std::vector<const char*> attributes = {"medium_list", "transform_matrix"};
        require_attributes(xml_material, attributes);
        std::unique_ptr<APTracer::Materials::Portal_t> material(
                    std::make_unique<APTracer::Materials::Portal_t>(get_transform_matrix(xml_material->Attribute("transform_matrix"), xml_transform_matrices), std::list<Medium_t*>()));
        materials_medium_list.emplace_back(material.get(), get_medium_index_list(xml_material->Attribute("medium_list"), xml_mediums));
        const tinyxml2::XMLElement* transformations_pre = xml_material->FirstChildElement("transformations_pre");
        if (transformations_pre != nullptr) {
            material_transformations.transformations_pre.emplace_back(material.get(), transformations_pre);
        }
        const tinyxml2::XMLElement* transformations_post = xml_material->FirstChildElement("transformations_post");
        if (transformations_post != nullptr) {
            material_transformations.transformations_post.emplace_back(material.get(), transformations_post);
        }
        return material;
    }
    if (type == "portal_refractive") {
        std::cerr << "Warning: refractive portal not implemented yet. Using 'diffuse'." << std::endl; 
        return std::make_unique<APTracer::Materials::Diffuse_t>(Vec3f(), Vec3f(0.5), 1.0);
    }
    if (type == "randommix") {
        const std::vector<const char*> attributes = {"first_material", "second_material", "ratio"};
        require_attributes(xml_material, attributes);
        std::unique_ptr<APTracer::Materials::RandomMix_t> material(
                    std::make_unique<APTracer::Materials::RandomMix_t>(nullptr, nullptr, xml_material->DoubleAttribute("ratio")));
        material_mix_lists.random_mix.emplace_back(material.get(), get_material_mix(xml_material->Attribute("first_material"), xml_material->Attribute("second_material"), xml_materials));
        return material;
    }
    if (type == "randommix_in") {
        const std::vector<const char*> attributes = {"first_material", "second_material", "ratio"};
        require_attributes(xml_material, attributes);
        std::unique_ptr<APTracer::Materials::RandomMixIn_t> material(
                    std::make_unique<APTracer::Materials::RandomMixIn_t>(nullptr, nullptr, xml_material->DoubleAttribute("ratio")));
        material_mix_lists.random_mix_in.emplace_back(material.get(), get_material_mix(xml_material->Attribute("first_material"), xml_material->Attribute("second_material"), xml_materials));
        return material;
    }
    if (type == "reflective") {
        const std::vector<const char*> attributes = {"emission", "colour"};
        require_attributes(xml_material, attributes);
        return std::make_unique<APTracer::Materials::Reflective_t>(
                    APTracer::get_colour(xml_material->Attribute("emission")), APTracer::get_colour(xml_material->Attribute("colour")));
    }
    if (type == "reflective_normal") {
        const std::vector<const char*> attributes = {"emission", "colour", "normal_map"};
        require_attributes(xml_material, attributes);
        return std::make_unique<APTracer::Materials::ReflectiveNormal_t>(
                    APTracer::get_colour(xml_material->Attribute("emission")), APTracer::get_colour(xml_material->Attribute("colour")), get_texture(xml_material->Attribute("normal_map"), xml_textures));
    }
    if (type == "reflective_fuzz") {
        const std::vector<const char*> attributes = {"emission", "colour", "order", "diffusivity"};
        require_attributes(xml_material, attributes);
        return std::make_unique<APTracer::Materials::ReflectiveFuzz_t>(
                    APTracer::get_colour(xml_material->Attribute("emission")), APTracer::get_colour(xml_material->Attribute("colour")), xml_material->DoubleAttribute("order"), xml_material->DoubleAttribute("diffusivity"));
    }
    if (type == "reflective_fuzz_normal") {
        const std::vector<const char*> attributes = {"emission", "colour", "normal_map", "order", "diffusivity"};
        require_attributes(xml_material, attributes);
        return std::make_unique<APTracer::Materials::ReflectiveFuzzNormal_t>(
                    APTracer::get_colour(xml_material->Attribute("emission")), APTracer::get_colour(xml_material->Attribute("colour")), 
                    get_texture(xml_material->Attribute("normal_map"), xml_textures), xml_material->DoubleAttribute("order"), xml_material->DoubleAttribute("diffusivity"));
    }
    if (type == "reflective_fuzz_tex") {
        const std::vector<const char*> attributes = {"emission", "texture", "order", "diffusivity"};
        require_attributes(xml_material, attributes);
        return std::make_unique<APTracer::Materials::ReflectiveFuzzTex_t>(
                    APTracer::get_colour(xml_material->Attribute("emission")), get_texture(xml_material->Attribute("texture"), xml_textures), xml_material->DoubleAttribute("order"), xml_material->DoubleAttribute("diffusivity"));
    }
    if (type == "reflective_fuzz_tex_normal") {
        const std::vector<const char*> attributes = {"emission", "texture", "normal_map", "order", "diffusivity"};
        require_attributes(xml_material, attributes);
        return std::make_unique<APTracer::Materials::ReflectiveFuzzTexNormal_t>(
                    APTracer::get_colour(xml_material->Attribute("emission")), get_texture(xml_material->Attribute("texture"), xml_textures), 
                    get_texture(xml_material->Attribute("normal_map"), xml_textures), xml_material->DoubleAttribute("order"), xml_material->DoubleAttribute("diffusivity"));
    }
    if (type == "reflective_refractive") {
        const std::vector<const char*> attributes = {"emission", "colour", "medium"};
        require_attributes(xml_material, attributes);
        return std::make_unique<APTracer::Materials::ReflectiveRefractive_t>(
                    APTracer::get_colour(xml_material->Attribute("emission")), APTracer::get_colour(xml_material->Attribute("colour")), get_medium(xml_material->Attribute("medium"), xml_mediums));
    }
    if (type == "reflective_refractive_normal") {
        const std::vector<const char*> attributes = {"emission", "colour", "normal_map", "medium"};
        require_attributes(xml_material, attributes);
        return std::make_unique<APTracer::Materials::ReflectiveRefractiveNormal_t>(
                    APTracer::get_colour(xml_material->Attribute("emission")), APTracer::get_colour(xml_material->Attribute("colour")), get_texture(xml_material->Attribute("normal_map"), xml_textures),
                    get_medium(xml_material->Attribute("medium"), xml_mediums));
    }
    if (type == "reflective_refractive_fuzz") {
        const std::vector<const char*> attributes = {"emission", "colour", "medium", "order", "diffusivity"};
        require_attributes(xml_material, attributes);
        return std::make_unique<APTracer::Materials::ReflectiveRefractiveFuzz_t>(
                    APTracer::get_colour(xml_material->Attribute("emission")), APTracer::get_colour(xml_material->Attribute("colour")), xml_material->DoubleAttribute("order"), xml_material->DoubleAttribute("diffusivity"), get_medium(xml_material->Attribute("medium"), xml_mediums));
    }
    if (type == "refractive") {
        const std::vector<const char*> attributes = {"emission", "colour", "medium"};
        require_attributes(xml_material, attributes);
        return std::make_unique<APTracer::Materials::Refractive_t>(
                    APTracer::get_colour(xml_material->Attribute("emission")), APTracer::get_colour(xml_material->Attribute("colour")), get_medium(xml_material->Attribute("medium"), xml_mediums));
    }
    if (type == "refractive_fuzz") {
        const std::vector<const char*> attributes = {"emission", "colour", "medium", "order", "diffusivity"};
        require_attributes(xml_material, attributes);
        return std::make_unique<APTracer::Materials::RefractiveFuzz_t>(
                    APTracer::get_colour(xml_material->Attribute("emission")), APTracer::get_colour(xml_material->Attribute("colour")), xml_material->DoubleAttribute("order"), xml_material->DoubleAttribute("diffusivity"), get_medium(xml_material->Attribute("medium"), xml_mediums));
    }
    if (type == "transparent") {
        const std::vector<const char*> attributes = {"medium"};
        require_attributes(xml_material, attributes);
        return std::make_unique<APTracer::Materials::Transparent_t>(
                    get_medium(xml_material->Attribute("medium"), xml_mediums));
    }
    if (type == "bounce_material") {
        const std::vector<const char*> attributes = {"max_bounces"};
        require_attributes(xml_material, attributes);
        return std::make_unique<APTracer::Materials::BounceMaterial_t>(
                    xml_material->UnsignedAttribute("max_bounces"));
    }
    if (type == "distance_material") {
        const std::vector<const char*> attributes = {"focus_distance", "exponent"};
        require_attributes(xml_material, attributes);
        return std::make_unique<APTracer::Materials::DistanceMaterial_t>(
                    xml_material->DoubleAttribute("focus_distance"), xml_material->DoubleAttribute("exponent"));
    }
    if (type == "texturemix") {
        const std::vector<const char*> attributes = {"first_material", "second_material", "texture"};
        require_attributes(xml_material, attributes);
        std::unique_ptr<APTracer::Materials::TextureMix_t> material(
                    std::make_unique<APTracer::Materials::TextureMix_t>(nullptr, nullptr, get_texture(xml_material->Attribute("texture"), xml_textures)));
        material_mix_lists.texture_mix.emplace_back(material.get(), get_material_mix(xml_material->Attribute("first_material"), xml_material->Attribute("second_material"), xml_materials));
        return material;
    }
    if (type == "texturemixin") {
        const std::vector<const char*> attributes = {"first_material", "second_material", "texture"};
        require_attributes(xml_material, attributes);
        std::unique_ptr<APTracer::Materials::TextureMixIn_t> material(
                    std::make_unique<APTracer::Materials::TextureMixIn_t>(nullptr, nullptr, get_texture(xml_material->Attribute("texture"), xml_textures)));
        material_mix_lists.texture_mix_in.emplace_back(material.get(), get_material_mix(xml_material->Attribute("first_material"), xml_material->Attribute("second_material"), xml_materials));
        return material;
    }
    if (type == "toon") {
        const std::vector<const char*> attributes = {"colour"};
        require_attributes(xml_material, attributes);
        return std::make_unique<APTracer::Materials::Toon_t>(
                    APTracer::get_colour(xml_material->Attribute("colour")));
    }
    if (type == "aggregate") {
        const std::vector<const char*> attributes = {"materials_list", "materials_names"};
        require_attributes(xml_material, attributes);
        materials_aggregate_list = std::make_unique<std::tuple<std::unique_ptr<std::list<size_t>>, std::unique_ptr<std::list<std::string>>>>(
                    get_material_index_list(xml_material->Attribute("materials_list"), xml_materials), get_medium_names(xml_material->Attribute("materials_names"))); // wtf
        return std::unique_ptr<Material_t>();
        // CHECK add aggregates
    }

    std::cerr << "Warning: Material type '" << type << "' not implemented. Using 'normal_material'." << std::endl; 
    return std::make_unique<APTracer::Materials::NormalMaterial_t>();
}

auto APTracer::Entities::SceneContext_t::create_mesh_geometry(const tinyxml2::XMLElement* xml_mesh_geometry) -> std::unique_ptr<MeshGeometry_t> {
    std::string type;
    const char* type_char = xml_mesh_geometry->Attribute("type");
    if (type_char == nullptr) {
        std::cerr << "Warning: XML mesh geometries should have a 'type' attribute. Using 'mesh_geometry'." << std::endl;
        type = "mesh_geometry";
    }
    else {
        type = type_char;
    }
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);

    if (type == "mesh_geometry") {
        const std::vector<const char*> attributes = {"filename"};
        require_attributes(xml_mesh_geometry, attributes);
        std::string filename = xml_mesh_geometry->Attribute("filename");
        for (auto& character: filename) {
            #ifdef _WIN32
                if (character == '/') {
                    character = '\\';
                }
            #else
                if (character == '\\') {
                    character = '/';
                }
            #endif
        } 

        return std::make_unique<MeshGeometry_t>(filename);
    }

    std::cerr << "Error: Mesh geometry type '" << type << "' not implemented. Only 'mesh_geometry' exists for now. Exiting." << std::endl; 
    exit(50);
}

auto APTracer::Entities::SceneContext_t::create_object(const tinyxml2::XMLElement* xml_object, std::unique_ptr<MeshTop_t> &mesh, const tinyxml2::XMLElement* xml_transform_matrices, const tinyxml2::XMLElement* xml_materials, const tinyxml2::XMLElement* xml_mesh_geometries) -> std::unique_ptr<Shape_t>{
    std::string type;
    const char* type_char = xml_object->Attribute("type");
    if (type_char == nullptr) {
        std::cerr << "Warning: XML objects should have a 'type' attribute. Using 'sphere'." << std::endl;
        type = "sphere";
    }
    else {
        type = type_char;
    }
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);

    if (type == "mesh") {
        const std::vector<const char*> attributes = {"material", "transform_matrix", "mesh_geometry"};
        require_attributes(xml_object, attributes);
        size_t material_index = get_material_index(xml_object->Attribute("material"), xml_materials);
        if (material_aggregates_[material_index]) {
            mesh = std::make_unique<Mesh_t>(
                        material_aggregates_[material_index].get(), get_transform_matrix(xml_object->Attribute("transform_matrix"), xml_transform_matrices), get_mesh_geometry(xml_object->Attribute("mesh_geometry"), xml_mesh_geometries));
            return nullptr;
        }
        
        mesh = std::make_unique<Mesh_t>(
                    materials_[material_index].get(), get_transform_matrix(xml_object->Attribute("transform_matrix"), xml_transform_matrices), get_mesh_geometry(xml_object->Attribute("mesh_geometry"), xml_mesh_geometries));
        return nullptr;
    }
    if (type == "mesh_motionblur") {
        const std::vector<const char*> attributes = {"material", "transform_matrix", "mesh_geometry"};
        require_attributes(xml_object, attributes);
        size_t material_index = get_material_index(xml_object->Attribute("material"), xml_materials);
        if (material_aggregates_[material_index]) {
            mesh = std::make_unique<MeshMotionblur_t>(
                        material_aggregates_[material_index].get(), get_transform_matrix(xml_object->Attribute("transform_matrix"), xml_transform_matrices), get_mesh_geometry(xml_object->Attribute("mesh_geometry"), xml_mesh_geometries));
            return nullptr;
        }
        
        mesh = std::make_unique<MeshMotionblur_t>(
                    materials_[material_index].get(), get_transform_matrix(xml_object->Attribute("transform_matrix"), xml_transform_matrices), get_mesh_geometry(xml_object->Attribute("mesh_geometry"), xml_mesh_geometries));
        return nullptr;
    }
    if (type == "sphere") {
        const std::vector<const char*> attributes = {"material", "transform_matrix"};
        require_attributes(xml_object, attributes);
        return std::make_unique<Sphere_t>(
                    get_material(xml_object->Attribute("material"), xml_materials), get_transform_matrix(xml_object->Attribute("transform_matrix"), xml_transform_matrices));
    }
    if (type == "sphere_motionblur") {
        const std::vector<const char*> attributes = {"material", "transform_matrix"};
        require_attributes(xml_object, attributes);
        return std::make_unique<SphereMotionblur_t>(
                    get_material(xml_object->Attribute("material"), xml_materials), get_transform_matrix(xml_object->Attribute("transform_matrix"), xml_transform_matrices));
    }
    if (type == "triangle") {
        const std::vector<const char*> attributes = {"material", "transform_matrix", "points", "normals", "texture_coordinates"};
        require_attributes(xml_object, attributes);
        std::vector<Vec3f> points_vec = get_points(xml_object->Attribute("points"));
        std::vector<Vec3f> normals = get_points(xml_object->Attribute("normals"));
        std::vector<double> texture_coordinates = get_texture_coordinates(xml_object->Attribute("texture_coordinates"));
        std::array<Vec3f, 3> points;

        if (points_vec.empty()) {
            std::cerr << "Error: Triangle points should not be empty. Exiting." << std::endl;
            exit(69);
        }

        for (size_t i = 0; i < points.size(); ++i) {
            points[i] = points_vec[i];
        }

        const Vec3f* normals_ptr = normals.empty() ? nullptr : normals.data();
        const double* texture_coordinates_ptr = texture_coordinates.empty() ? nullptr : texture_coordinates.data();

        return std::make_unique<Triangle_t>(
                    get_material(xml_object->Attribute("material"), xml_materials), get_transform_matrix(xml_object->Attribute("transform_matrix"), xml_transform_matrices), points, normals_ptr, texture_coordinates_ptr);
    }
    if (type == "triangle_motionblur") {
        const std::vector<const char*> attributes = {"material", "transform_matrix", "points", "normals", "texture_coordinates"};
        require_attributes(xml_object, attributes);
        std::vector<Vec3f> points_vec = get_points(xml_object->Attribute("points"));
        std::vector<Vec3f> normals = get_points(xml_object->Attribute("normals"));
        std::vector<double> texture_coordinates = get_texture_coordinates(xml_object->Attribute("texture_coordinates"));
        std::array<Vec3f, 3> points;

        if (points_vec.empty()) {
            std::cerr << "Error: Triangle points should not be empty. Exiting." << std::endl;
            exit(69);
        }

        for (size_t i = 0; i < points.size(); ++i) {
            points[i] = points_vec[i];
        }

        const Vec3f* normals_ptr = normals.empty() ? nullptr : normals.data();
        const double* texture_coordinates_ptr = texture_coordinates.empty() ? nullptr : texture_coordinates.data();

        return std::make_unique<TriangleMotionblur_t>(
                    get_material(xml_object->Attribute("material"), xml_materials), get_transform_matrix(xml_object->Attribute("transform_matrix"), xml_transform_matrices), points, normals_ptr, texture_coordinates_ptr);
    }
    if (type == "triangle_mesh") {
        const std::vector<const char*> attributes = {"material", "transform_matrix", "mesh_geometry", "index"};
        require_attributes(xml_object, attributes);
        return std::make_unique<TriangleMesh_t>(
                    get_material(xml_object->Attribute("material"), xml_materials), get_transform_matrix(xml_object->Attribute("transform_matrix"), xml_transform_matrices), get_mesh_geometry(xml_object->Attribute("mesh_geometry"), xml_mesh_geometries), xml_object->UnsignedAttribute("index"));
    }
    if (type == "triangle_mesh_motionblur") {
        const std::vector<const char*> attributes = {"material", "transform_matrix", "mesh_geometry", "index"};
        require_attributes(xml_object, attributes);
        return std::make_unique<TriangleMeshMotionblur_t>(
                    get_material(xml_object->Attribute("material"), xml_materials), get_transform_matrix(xml_object->Attribute("transform_matrix"), xml_transform_matrices), get_mesh_geometry(xml_object->Attribute("mesh_geometry"), xml_mesh_geometries), xml_object->UnsignedAttribute("index"));
    }
    
    std::cerr << "Error: Object type '" << type << "' not implemented. Exiting." << std::endl; 
    exit(60);
}

auto APTracer::Entities::SceneContext_t::create_directional_light(const tinyxml2::XMLElement* xml_directional_light, const tinyxml2::XMLElement* xml_transform_matrices) -> std::unique_ptr<DirectionalLight_t>{
    std::string type;
    const char* type_char = xml_directional_light->Attribute("type");
    if (type_char == nullptr) {
        std::cerr << "Warning: XML directional lights should have a 'type' attribute. Using 'directional_light'." << std::endl;
        type = "directional_light";
    }
    else {
        type = type_char;
    }
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);

    if (type == "directional_light") {
        const std::vector<const char*> attributes = {"colour", "transform_matrix"};
        require_attributes(xml_directional_light, attributes);
        return std::make_unique<DirectionalLight_t>(
                    APTracer::get_colour(xml_directional_light->Attribute("colour")), get_transform_matrix(xml_directional_light->Attribute("transform_matrix"), xml_transform_matrices));
    }

    std::cerr << "Error: Directional light type '" << type << "' not implemented. Only 'directional_light' exists for now. Exiting." << std::endl; 
    exit(70);
}

auto APTracer::Entities::SceneContext_t::create_skybox(const tinyxml2::XMLElement* xml_skybox, const tinyxml2::XMLElement* xml_textures, const tinyxml2::XMLElement* xml_transform_matrices, const tinyxml2::XMLElement* xml_directional_lights) -> std::unique_ptr<Skybox_t> {
    std::string type;
    const char* type_char = xml_skybox->Attribute("type");
    if (type_char == nullptr) {
        std::cerr << "Warning: XML skyboxes should have a 'type' attribute. Using 'skybox_flat'." << std::endl;
        type = "skybox_flat";
    }
    else {
        type = type_char;
    }
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);

    if (type == "skybox_flat") {
        const std::vector<const char*> attributes = {"colour"};
        require_attributes(xml_skybox, attributes);
        return std::make_unique<APTracer::Skyboxes::SkyboxFlat_t>(
                    APTracer::get_colour(xml_skybox->Attribute("colour")));
    }
    if (type == "skybox_flat_sun") {
        const std::vector<const char*> attributes = {"colour", "lights"};
        require_attributes(xml_skybox, attributes);

        std::vector<DirectionalLight_t*> lights = get_lights(xml_skybox->Attribute("lights"), xml_directional_lights);

        DirectionalLight_t** lights_ptr = lights.empty() ? nullptr : lights.data();
               
        return std::make_unique<APTracer::Skyboxes::SkyboxFlatSun_t>(
                    APTracer::get_colour(xml_skybox->Attribute("colour")), lights_ptr, lights.size());
    }
    if (type == "skybox_texture") {
        const std::vector<const char*> attributes = {"texture"};
        require_attributes(xml_skybox, attributes);
        return std::make_unique<APTracer::Skyboxes::SkyboxTexture_t>(
                    get_texture(xml_skybox->Attribute("texture"), xml_textures));
    }
    if (type == "skybox_texture_sun") {
        const std::vector<const char*> attributes = {"texture", "light_position", "light_colour", "light_radius"};
        require_attributes(xml_skybox, attributes);
        return std::make_unique<APTracer::Skyboxes::SkyboxTextureSun_t>(
                    get_texture(xml_skybox->Attribute("texture"), xml_textures), get_xy<double>(xml_skybox->Attribute("light_position")), APTracer::get_colour(xml_skybox->Attribute("light_colour")), xml_skybox->DoubleAttribute("light_radius"));
    }
    if (type == "skybox_texture_transformation") {
        const std::vector<const char*> attributes = {"texture", "transform_matrix"};
        require_attributes(xml_skybox, attributes);
        return std::make_unique<APTracer::Skyboxes::SkyboxTextureTransformation_t>(
                    get_texture(xml_skybox->Attribute("texture"), xml_textures), get_transform_matrix(xml_skybox->Attribute("transform_matrix"), xml_transform_matrices));
    }
    if (type == "skybox_texture_transformation_sun") {
        const std::vector<const char*> attributes = {"texture", "light_position", "light_colour", "light_radius", "transform_matrix"};
        require_attributes(xml_skybox, attributes);
        return std::make_unique<APTracer::Skyboxes::SkyboxTextureTransformationSun_t>(
                    get_texture(xml_skybox->Attribute("texture"), xml_textures), get_transform_matrix(xml_skybox->Attribute("transform_matrix"), xml_transform_matrices), get_xy<double>(xml_skybox->Attribute("light_position")), APTracer::get_colour(xml_skybox->Attribute("light_colour")), xml_skybox->DoubleAttribute("light_radius"));
    }

    std::cerr << "Warning: Skybox type '" << type << "' not implemented. Using 'skybox_flat'." << std::endl; 
    return std::make_unique<APTracer::Skyboxes::SkyboxFlat_t>(Vec3f(0.5));
}

auto APTracer::Entities::SceneContext_t::create_imgbuffer(const tinyxml2::XMLElement* xml_imgbuffer) -> std::unique_ptr<ImgBuffer_t> {
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

    if (type == "imgbuffer") {
        const std::vector<const char*> attributes = {"resx", "resy"};
        require_attributes(xml_imgbuffer, attributes);
        return std::make_unique<ImgBuffer_t>(
                    xml_imgbuffer->UnsignedAttribute("resx"), xml_imgbuffer->UnsignedAttribute("resy"));
    }
    if (type == "imgbuffer_opengl") {
        const std::vector<const char*> attributes = {"resx", "resy"};
        require_attributes(xml_imgbuffer, attributes);
        use_gl_ = true; 
        std::unique_ptr<ImgBufferOpenGL_t> opengl_imgbuffer = std::make_unique<ImgBufferOpenGL_t>(xml_imgbuffer->UnsignedAttribute("resx"), xml_imgbuffer->UnsignedAttribute("resy"));
        opengl_imgbuffer_ = opengl_imgbuffer.get();
        return opengl_imgbuffer;
    }

    std::cerr << "Warning: Imgbuffer type '" << type << "' not implemented. Only 'imgbuffer', and 'imgbuffer_opengl' exist for now. Using 'imgbuffer'." << std::endl; 
    return std::make_unique<ImgBuffer_t>(300, 200);
}

auto APTracer::Entities::SceneContext_t::create_camera(const tinyxml2::XMLElement* xml_camera, const std::string &next_filename, const tinyxml2::XMLElement* xml_transform_matrices, const tinyxml2::XMLElement* xml_mediums, const tinyxml2::XMLElement* xml_imgbuffers, const tinyxml2::XMLElement* xml_skyboxes) -> std::unique_ptr<Camera_t> {
    std::string type;
    const char* type_char = xml_camera->Attribute("type");
    if (type_char == nullptr) {
        std::cerr << "Warning: XML cameras should have a 'type' attribute. Using 'cam'." << std::endl;
        type = "cam";
    }
    else {
        type = type_char;
    }
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);
    std::string filename;
    const char* filename_char = xml_camera->Attribute("filename");
    if (filename_char == nullptr) {
        std::cerr << "Error: XML cameras should have a 'filename' attribute. Using 'nan'." << std::endl;
        filename = "nan";
    }
    else {
        filename = filename_char;
        for (auto& character: filename) {
            #ifdef _WIN32
                if (character == '/') {
                    character = '\\';
                }
            #else
                if (character == '\\') {
                    character = '/';
                }
            #endif
        }
    }
    std::string filename_check = filename;
    std::transform(filename_check.begin(), filename_check.end(), filename_check.begin(), ::tolower);

    if (filename_check == "nan") {
        filename = next_filename;
    }

    if (type == "cam") {
        const std::vector<const char*> attributes = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "medium_list", "skybox", "max_bounces", "gammaind"};
        require_attributes(xml_camera, attributes);
        return std::make_unique<APTracer::Cameras::Cam_t>(
                    get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), 
                            get_xy<double>(xml_camera->Attribute("fov")), get_xy<unsigned int>(xml_camera->Attribute("subpix")), get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), xml_camera->UnsignedAttribute("max_bounces"), xml_camera->DoubleAttribute("gammaind"));
    }
    if (type == "cam_aperture") {
        const std::vector<const char*> attributes = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "medium_list", "skybox", "max_bounces", "gammaind", "focus_distance", "aperture"};
        require_attributes(xml_camera, attributes);
        return std::make_unique<APTracer::Cameras::CamAperture_t>(
                    get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), 
                            get_xy<double>(xml_camera->Attribute("fov")), get_xy<unsigned int>(xml_camera->Attribute("subpix")), get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), xml_camera->UnsignedAttribute("max_bounces"), xml_camera->DoubleAttribute("focus_distance"),
                            xml_camera->DoubleAttribute("aperture"), xml_camera->DoubleAttribute("gammaind"));
    }
    if (type == "cam_motionblur") {
        const std::vector<const char*> attributes = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "medium_list", "skybox", "max_bounces", "gammaind", "time"};
        require_attributes(xml_camera, attributes);       
        return std::make_unique<APTracer::Cameras::CamMotionblur_t>(
                    get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), 
                            get_xy<double>(xml_camera->Attribute("fov")), get_xy<unsigned int>(xml_camera->Attribute("subpix")), get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), xml_camera->UnsignedAttribute("max_bounces"), get_xy<double>(xml_camera->Attribute("time")), xml_camera->DoubleAttribute("gammaind"));
    }
    if (type == "cam_motionblur_aperture") {
        const std::vector<const char*> attributes = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "medium_list", "skybox", "max_bounces", "gammaind", "focus_distance", "aperture", "time"};
        require_attributes(xml_camera, attributes);
        return std::make_unique<APTracer::Cameras::CamMotionblurAperture_t>(
                    get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), 
                            get_xy<double>(xml_camera->Attribute("fov")), get_xy<unsigned int>(xml_camera->Attribute("subpix")), get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), xml_camera->UnsignedAttribute("max_bounces"), xml_camera->DoubleAttribute("focus_distance"),
                            xml_camera->DoubleAttribute("aperture"), get_xy<double>(xml_camera->Attribute("time")), xml_camera->DoubleAttribute("gammaind"));
    }
    if (type == "fishcam") {
        const std::vector<const char*> attributes = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "medium_list", "skybox", "max_bounces", "gammaind"};
        require_attributes(xml_camera, attributes);
        return std::make_unique<APTracer::Cameras::FishCam_t>(
                    get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), 
                            get_xy<double>(xml_camera->Attribute("fov")), get_xy<unsigned int>(xml_camera->Attribute("subpix")), get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), xml_camera->UnsignedAttribute("max_bounces"), xml_camera->DoubleAttribute("gammaind"));
    }
    if (type == "fishcam_aperture") {
        const std::vector<const char*> attributes = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "medium_list", "skybox", "max_bounces", "gammaind", "focus_distance", "aperture"};
        require_attributes(xml_camera, attributes);
        return std::make_unique<APTracer::Cameras::FishCamAperture_t>(
                    get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), 
                            get_xy<double>(xml_camera->Attribute("fov")), get_xy<unsigned int>(xml_camera->Attribute("subpix")), get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), xml_camera->UnsignedAttribute("max_bounces"), xml_camera->DoubleAttribute("focus_distance"),
                            xml_camera->DoubleAttribute("aperture"), xml_camera->DoubleAttribute("gammaind"));
    }
    if (type == "fishcam_motionblur") {
        const std::vector<const char*> attributes = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "medium_list", "skybox", "max_bounces", "gammaind", "time"};
        require_attributes(xml_camera, attributes);
        return std::make_unique<APTracer::Cameras::FishCamMotionblur_t>(
                    get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), 
                            get_xy<double>(xml_camera->Attribute("fov")), get_xy<unsigned int>(xml_camera->Attribute("subpix")), get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), xml_camera->UnsignedAttribute("max_bounces"), get_xy<double>(xml_camera->Attribute("time")), xml_camera->DoubleAttribute("gammaind"));
    }
    if (type == "fishcam_motionblur_aperture") {
        const std::vector<const char*> attributes = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "medium_list", "skybox", "max_bounces", "gammaind", "focus_distance", "aperture", "time"};
        require_attributes(xml_camera, attributes);
        return std::make_unique<APTracer::Cameras::FishCamMotionblurAperture_t>(
                    get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), 
                            get_xy<double>(xml_camera->Attribute("fov")), get_xy<unsigned int>(xml_camera->Attribute("subpix")), get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), xml_camera->UnsignedAttribute("max_bounces"), xml_camera->DoubleAttribute("focus_distance"),
                            xml_camera->DoubleAttribute("aperture"), get_xy<double>(xml_camera->Attribute("time")), xml_camera->DoubleAttribute("gammaind"));
    }
    if (type == "isocam") {
        const std::vector<const char*> attributes = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "medium_list", "skybox", "max_bounces", "gammaind"};
        require_attributes(xml_camera, attributes);
        return std::make_unique<APTracer::Cameras::IsoCam_t>(
                    get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), 
                            get_xy<double>(xml_camera->Attribute("fov")), get_xy<unsigned int>(xml_camera->Attribute("subpix")), get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), xml_camera->UnsignedAttribute("max_bounces"), xml_camera->DoubleAttribute("gammaind"));
    }
    if (type == "isocam_aperture") {
        const std::vector<const char*> attributes = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "medium_list", "skybox", "max_bounces", "gammaind", "focus_distance", "aperture"};
        require_attributes(xml_camera, attributes);
        return std::make_unique<APTracer::Cameras::IsoCamAperture_t>(
                    get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), 
                            get_xy<double>(xml_camera->Attribute("fov")), get_xy<unsigned int>(xml_camera->Attribute("subpix")), get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), xml_camera->UnsignedAttribute("max_bounces"), xml_camera->DoubleAttribute("focus_distance"),
                            xml_camera->DoubleAttribute("aperture"), xml_camera->DoubleAttribute("gammaind"));
    }
    if (type == "isocam_motionblur") {
        const std::vector<const char*> attributes = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "medium_list", "skybox", "max_bounces", "gammaind", "time"};
        require_attributes(xml_camera, attributes);
        return std::make_unique<APTracer::Cameras::IsoCamMotionblur_t>(
                    get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), 
                            get_xy<double>(xml_camera->Attribute("fov")), get_xy<unsigned int>(xml_camera->Attribute("subpix")), get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), xml_camera->UnsignedAttribute("max_bounces"), get_xy<double>(xml_camera->Attribute("time")), xml_camera->DoubleAttribute("gammaind"));
    }
    if (type == "isocam_motionblur_aperture") {
        const std::vector<const char*> attributes = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "medium_list", "skybox", "max_bounces", "gammaind", "focus_distance", "aperture", "time"};
        require_attributes(xml_camera, attributes);
        return std::make_unique<APTracer::Cameras::IsoCamMotionblurAperture_t>(
                    get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), 
                            get_xy<double>(xml_camera->Attribute("fov")), get_xy<unsigned int>(xml_camera->Attribute("subpix")), get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), xml_camera->UnsignedAttribute("max_bounces"), xml_camera->DoubleAttribute("focus_distance"),
                            xml_camera->DoubleAttribute("aperture"), get_xy<double>(xml_camera->Attribute("time")), xml_camera->DoubleAttribute("gammaind"));
    }
    if (type == "reccam") {
        const std::vector<const char*> attributes = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "medium_list", "skybox", "max_bounces", "gammaind"};
        require_attributes(xml_camera, attributes);
        return std::make_unique<APTracer::Cameras::RecCam_t>(
                    get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), 
                            get_xy<double>(xml_camera->Attribute("fov")), get_xy<unsigned int>(xml_camera->Attribute("subpix")), get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), xml_camera->UnsignedAttribute("max_bounces"), xml_camera->DoubleAttribute("gammaind"));
    }
    if (type == "reccam_aperture") {
        const std::vector<const char*> attributes = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "medium_list", "skybox", "max_bounces", "gammaind", "focus_distance", "aperture"};
        require_attributes(xml_camera, attributes);
        return std::make_unique<APTracer::Cameras::RecCamAperture_t>(
                    get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), 
                            get_xy<double>(xml_camera->Attribute("fov")), get_xy<unsigned int>(xml_camera->Attribute("subpix")), get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), xml_camera->UnsignedAttribute("max_bounces"), xml_camera->DoubleAttribute("focus_distance"),
                            xml_camera->DoubleAttribute("aperture"), xml_camera->DoubleAttribute("gammaind"));
    }
    if (type == "reccam_motionblur") {
        const std::vector<const char*> attributes = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "medium_list", "skybox", "max_bounces", "gammaind", "time"};
        require_attributes(xml_camera, attributes);
        return std::make_unique<APTracer::Cameras::RecCamMotionblur_t>(
                    get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), 
                            get_xy<double>(xml_camera->Attribute("fov")), get_xy<unsigned int>(xml_camera->Attribute("subpix")), get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), xml_camera->UnsignedAttribute("max_bounces"), get_xy<double>(xml_camera->Attribute("time")), xml_camera->DoubleAttribute("gammaind"));
    }
    if (type == "reccam_motionblur_aperture") {
        const std::vector<const char*> attributes = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "medium_list", "skybox", "max_bounces", "gammaind", "focus_distance", "aperture", "time"};
        require_attributes(xml_camera, attributes);
        return std::make_unique<APTracer::Cameras::RecCamMotionblurAperture_t>(
                    get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), 
                            get_xy<double>(xml_camera->Attribute("fov")), get_xy<unsigned int>(xml_camera->Attribute("subpix")), get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), 
                            get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), xml_camera->UnsignedAttribute("max_bounces"), xml_camera->DoubleAttribute("focus_distance"),
                            xml_camera->DoubleAttribute("aperture"), get_xy<double>(xml_camera->Attribute("time")), xml_camera->DoubleAttribute("gammaind"));
    }
    if (type == "cam_3d") {
        const std::vector<const char*> attributes = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "imgbuffer_L", "imgbuffer_R", "eye_dist", "medium_list", "skybox", "max_bounces", "gammaind"};
        require_attributes(xml_camera, attributes);
        return std::make_unique<APTracer::Cameras::Cam3D_t>(
                    get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), get_xy<double>(xml_camera->Attribute("fov")), get_xy<unsigned int>(xml_camera->Attribute("subpix")), 
                            get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_imgbuffer(xml_camera->Attribute("imgbuffer_L"), xml_imgbuffers), 
                            get_imgbuffer(xml_camera->Attribute("imgbuffer_R"), xml_imgbuffers), xml_camera->DoubleAttribute("eye_dist"), 
                            get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), 
                            xml_camera->UnsignedAttribute("max_bounces"), xml_camera->DoubleAttribute("focus_distance"), xml_camera->DoubleAttribute("gammaind"));
    }
    if (type == "cam_3d_aperture") {
        const std::vector<const char*> attributes = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "imgbuffer_L", "imgbuffer_R", "eye_dist", "medium_list", "skybox", "max_bounces", "gammaind", "focus_distance", "aperture"};
        require_attributes(xml_camera, attributes);
        return std::make_unique<APTracer::Cameras::Cam3DAperture_t>(
                    get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), get_xy<double>(xml_camera->Attribute("fov")), get_xy<unsigned int>(xml_camera->Attribute("subpix")), 
                            get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_imgbuffer(xml_camera->Attribute("imgbuffer_L"), xml_imgbuffers), 
                            get_imgbuffer(xml_camera->Attribute("imgbuffer_R"), xml_imgbuffers), xml_camera->DoubleAttribute("eye_dist"), 
                            get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), 
                            xml_camera->UnsignedAttribute("max_bounces"), xml_camera->DoubleAttribute("focus_distance"), xml_camera->DoubleAttribute("aperture"), 
                            xml_camera->DoubleAttribute("gammaind"));
    }
    if (type == "cam_3d_motionblur") {
        const std::vector<const char*> attributes = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "imgbuffer_L", "imgbuffer_R", "eye_dist", "medium_list", "skybox", "max_bounces", "gammaind", "time"};
        require_attributes(xml_camera, attributes);
        return std::make_unique<APTracer::Cameras::Cam3DMotionblur_t>(
                    get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), get_xy<double>(xml_camera->Attribute("fov")), get_xy<unsigned int>(xml_camera->Attribute("subpix")), 
                            get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_imgbuffer(xml_camera->Attribute("imgbuffer_L"), xml_imgbuffers), 
                            get_imgbuffer(xml_camera->Attribute("imgbuffer_R"), xml_imgbuffers), xml_camera->DoubleAttribute("eye_dist"), 
                            get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), 
                            xml_camera->UnsignedAttribute("max_bounces"), xml_camera->DoubleAttribute("focus_distance"), get_xy<double>(xml_camera->Attribute("time")), xml_camera->DoubleAttribute("gammaind"));
    }
    if (type == "cam_3d_motionblur_aperture") {
        const std::vector<const char*> attributes = {"fov", "subpix", "transform_matrix", "up", "imgbuffer", "imgbuffer_L", "imgbuffer_R", "eye_dist", "medium_list", "skybox", "max_bounces", "gammaind", "focus_distance", "aperture", "time"};
        require_attributes(xml_camera, attributes);
        return std::make_unique<APTracer::Cameras::Cam3DMotionblurAperture_t>(
                    get_transform_matrix(xml_camera->Attribute("transform_matrix"), xml_transform_matrices), filename, APTracer::get_colour(xml_camera->Attribute("up")), get_xy<double>(xml_camera->Attribute("fov")), get_xy<unsigned int>(xml_camera->Attribute("subpix")), 
                            get_imgbuffer(xml_camera->Attribute("imgbuffer"), xml_imgbuffers), get_imgbuffer(xml_camera->Attribute("imgbuffer_L"), xml_imgbuffers), 
                            get_imgbuffer(xml_camera->Attribute("imgbuffer_R"), xml_imgbuffers), xml_camera->DoubleAttribute("eye_dist"), 
                            get_medium_list(xml_camera->Attribute("medium_list"), xml_mediums), get_skybox(xml_camera->Attribute("skybox"), xml_skyboxes), 
                            xml_camera->UnsignedAttribute("max_bounces"), xml_camera->DoubleAttribute("focus_distance"), xml_camera->DoubleAttribute("aperture"), 
                            get_xy<double>(xml_camera->Attribute("time")), xml_camera->DoubleAttribute("gammaind"));
    }

    std::cerr << "Error: Camera type '" << type << "' not implemented. Exiting." << std::endl; 
    exit(100);
}

auto APTracer::Entities::SceneContext_t::create_acceleration_structure(const tinyxml2::XMLElement* xml_acceleration_structure) -> void {
    if (xml_acceleration_structure == nullptr) {
        scene_->acc_ = std::make_unique<APTracer::Acceleration::AccelerationMultiGridVector_t>(
                            scene_->geometry_, 1, 128, 32, 1);
        return;
    }

    std::string type;
    const char* type_char = xml_acceleration_structure->Attribute("type");
    if (type_char == nullptr) {
        std::cerr << "Warning: XML acceleration structures should have a 'type' attribute. Using 'multi_grid_vector'." << std::endl;
        type = "multi_grid_vector";
    }
    else {
        type = type_char;
    }
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);

    if (type == "grid") {
        scene_->acc_ = std::make_unique<APTracer::Acceleration::AccelerationGrid_t>(
                        scene_->geometry_, xml_acceleration_structure->UnsignedAttribute("min_resolution", 1), xml_acceleration_structure->UnsignedAttribute("max_resolution", 128));
    }
    else if (type == "grid_array") {
        scene_->acc_ = std::make_unique<APTracer::Acceleration::AccelerationGridArray_t>(
                        scene_->geometry_, xml_acceleration_structure->UnsignedAttribute("min_resolution", 1), xml_acceleration_structure->UnsignedAttribute("max_resolution", 128));
    }
    else if (type == "grid_vector") {
        scene_->acc_ = std::make_unique<APTracer::Acceleration::AccelerationGridVector_t>(
                        scene_->geometry_, xml_acceleration_structure->UnsignedAttribute("min_resolution", 1), xml_acceleration_structure->UnsignedAttribute("max_resolution", 128));
    }
    else if (type == "multi_grid") {
        scene_->acc_ = std::make_unique<APTracer::Acceleration::AccelerationMultiGrid_t>(
                        scene_->geometry_, xml_acceleration_structure->UnsignedAttribute("min_resolution", 1), xml_acceleration_structure->UnsignedAttribute("max_resolution", 128), xml_acceleration_structure->UnsignedAttribute("max_cell_content", 32), xml_acceleration_structure->UnsignedAttribute("max_grid_level", 1));
    }
    else if (type == "multi_grid_array") {
        scene_->acc_ = std::make_unique<APTracer::Acceleration::AccelerationMultiGridArray_t>(
                        scene_->geometry_, xml_acceleration_structure->UnsignedAttribute("min_resolution", 1), xml_acceleration_structure->UnsignedAttribute("max_resolution", 128), xml_acceleration_structure->UnsignedAttribute("max_cell_content", 32), xml_acceleration_structure->UnsignedAttribute("max_grid_level", 1));
    }
    else if (type == "multi_grid_vector") {
        scene_->acc_ = std::make_unique<APTracer::Acceleration::AccelerationMultiGridVector_t>(
                        scene_->geometry_, xml_acceleration_structure->UnsignedAttribute("min_resolution", 1), xml_acceleration_structure->UnsignedAttribute("max_resolution", 128), xml_acceleration_structure->UnsignedAttribute("max_cell_content", 32), xml_acceleration_structure->UnsignedAttribute("max_grid_level", 1));
    }
    else {
        std::cerr << "Warning: Acceleration structure type '" << type << "' not implemented. Using 'multi_grid_vector'." << std::endl; 
        scene_->acc_ = std::make_unique<APTracer::Acceleration::AccelerationMultiGridVector_t>(
                        scene_->geometry_, 1, 128, 32, 1);
    }
}

auto APTracer::Entities::SceneContext_t::get_transform_matrix(std::string transform_matrix, const tinyxml2::XMLElement* xml_transform_matrices) -> TransformMatrix_t* {
    std::transform(transform_matrix.begin(), transform_matrix.end(), transform_matrix.begin(), ::tolower);
    if (transform_matrix == "nan") {
        transform_matrices_[index_transform_matrices_] = std::make_unique<TransformMatrix_t>();
        ++index_transform_matrices_;
        return transform_matrices_[index_transform_matrices_ - 1].get();
    }
    if (is_number(transform_matrix)) {
        return transform_matrices_[std::stoi(transform_matrix) - 1].get();
    }

    if (xml_transform_matrices != nullptr) {
        size_t index = 0;
        for (const tinyxml2::XMLElement* xml_transform_matrix = xml_transform_matrices->FirstChildElement("transform_matrix"); xml_transform_matrix; xml_transform_matrix = xml_transform_matrix->NextSiblingElement("transform_matrix")) {
            const char* transform_matrix_char = xml_transform_matrix->Attribute("name");
            if (transform_matrix_char != nullptr) {
                std::string name_transform_matrix = transform_matrix_char;
                std::transform(name_transform_matrix.begin(), name_transform_matrix.end(), name_transform_matrix.begin(), ::tolower);
                if (name_transform_matrix == transform_matrix) {
                    return transform_matrices_[index].get();
                }
            }                
            ++index;
        }
    }

    std::cerr << "Warning: Transformation matrix '" << transform_matrix << "' not found. Using identity. This causes a memory leak." << std::endl;
    return new TransformMatrix_t();
}

auto APTracer::Entities::SceneContext_t::get_material_index_list(std::string string_material_list, const tinyxml2::XMLElement* xml_materials) -> std::unique_ptr<std::list<size_t>> {
    std::unique_ptr<std::list<size_t>> material_list = std::make_unique<std::list<size_t>>();
    std::string delimiter = ", ";
    size_t pos = 0;
    std::string token;

    while ((pos = string_material_list.find(delimiter)) != std::string::npos) {
        token = string_material_list.substr(0, pos);

        if (is_number(token)) {
            material_list->push_back(std::stoi(token) - 1);
        }
        else {
            if (xml_materials != nullptr) {
                bool missing = true;
                std::transform(token.begin(), token.end(), token.begin(), ::tolower);
                size_t index = 0;
                for (const tinyxml2::XMLElement* xml_material = xml_materials->FirstChildElement("material"); xml_material; xml_material = xml_material->NextSiblingElement("material")) {
                    const char* material_char = xml_material->Attribute("name");
                    if (material_char != nullptr) {
                        std::string name_material = material_char;
                        std::transform(name_material.begin(), name_material.end(), name_material.begin(), ::tolower);
                        if (name_material == token) {
                            material_list->push_back(index);
                            missing = false;
                            break;
                        }
                    }                    
                    ++index;
                }
                if (missing) {
                    std::cerr << "Error: Material '" << token << "' not found. Exiting." << std::endl;
                    exit(498);
                }
            }
            else {
                std::cerr << "Error: No materials, material '" << token << "' not found. Exiting." << std::endl;
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
        if (xml_materials != nullptr) {
            bool missing = true;
            std::transform(string_material_list.begin(), string_material_list.end(), string_material_list.begin(), ::tolower);
            size_t index = 0;
            for (const tinyxml2::XMLElement* xml_material = xml_materials->FirstChildElement("material"); xml_material; xml_material = xml_material->NextSiblingElement("material")) {
                const char* material_char = xml_material->Attribute("name");
                if (material_char != nullptr) {
                    std::string name_material = material_char;
                    std::transform(name_material.begin(), name_material.end(), name_material.begin(), ::tolower);
                    if (name_material == string_material_list) {
                        material_list->push_back(index);
                        missing = false;
                        break;
                    }
                }                
                ++index;
            }
            if (missing) {
                std::cerr << "Error: Material '" << string_material_list << "' not found. Exiting." << std::endl;
                exit(498);
            }
        }
        else {
            std::cerr << "Error: No materials, material '" << string_material_list << "' not found. Exiting." << std::endl;
            exit(499);
        }
    }
    return material_list;
}

auto APTracer::Entities::SceneContext_t::get_medium_index_list(std::string string_medium_list, const tinyxml2::XMLElement* xml_mediums) -> std::list<size_t> {
    std::list<size_t> medium_list;
    std::string delimiter = ", ";
    size_t pos = 0;
    std::string token;

    while ((pos = string_medium_list.find(delimiter)) != std::string::npos) {
        token = string_medium_list.substr(0, pos);

        if (is_number(token)) {
            medium_list.push_back(std::stoi(token) - 1);
        }
        else {
            if (xml_mediums != nullptr) {
                bool missing = true;
                std::transform(token.begin(), token.end(), token.begin(), ::tolower);
                size_t index = 0;
                for (const tinyxml2::XMLElement* xml_medium = xml_mediums->FirstChildElement("medium"); xml_medium; xml_medium = xml_medium->NextSiblingElement("medium")) {
                    const char* medium_char = xml_medium->Attribute("name");
                    if (medium_char != nullptr) {
                        std::string name_medium = medium_char;
                        std::transform(name_medium.begin(), name_medium.end(), name_medium.begin(), ::tolower);
                        if (name_medium == token) {
                            medium_list.push_back(index);
                            missing = false;
                            break;
                        }
                    }                    
                    ++index;
                }
                if (missing) {
                    std::cerr << "Error: Medium '" << token << "' not found. Exiting." << std::endl;
                    exit(498);
                }
            }
            else {
                std::cerr << "Error: No mediums, medium '" << token << "' not found. Exiting." << std::endl;
                exit(499);
            }
        }
        // CHECK this should check for errors.

        string_medium_list.erase(0, pos + delimiter.length());
    }
    if (is_number(string_medium_list)) {
        medium_list.push_back(std::stoi(string_medium_list) - 1);
    }
    else {
        if (xml_mediums != nullptr) {
            bool missing = true;
            std::transform(string_medium_list.begin(), string_medium_list.end(), string_medium_list.begin(), ::tolower);
            size_t index = 0;
            for (const tinyxml2::XMLElement* xml_medium = xml_mediums->FirstChildElement("medium"); xml_medium; xml_medium = xml_medium->NextSiblingElement("medium")) {
                const char* medium_char = xml_medium->Attribute("name");
                if (medium_char != nullptr) {
                    std::string name_medium = medium_char;
                    std::transform(name_medium.begin(), name_medium.end(), name_medium.begin(), ::tolower);
                    if (name_medium == string_medium_list) {
                        medium_list.push_back(index);
                        missing = false;
                        break;
                    }
                }                
                ++index;
            }
            if (missing) {
                std::cerr << "Error: Medium '" << string_medium_list << "' not found. Exiting." << std::endl;
                exit(498);
            }
        }
        else {
            std::cerr << "Error: No mediums, medium '" << string_medium_list << "' not found. Exiting." << std::endl;
            exit(499);
        }
    }
    return medium_list;
}

auto APTracer::Entities::SceneContext_t::get_medium_list(std::string string_medium_list, const tinyxml2::XMLElement* xml_mediums) const -> std::list<Medium_t*> {
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
            if (xml_mediums != nullptr) {
                bool missing = true;
                std::transform(token.begin(), token.end(), token.begin(), ::tolower);
                size_t index = 0;
                for (const tinyxml2::XMLElement* xml_medium = xml_mediums->FirstChildElement("medium"); xml_medium; xml_medium = xml_medium->NextSiblingElement("medium")) {
                    const char* medium_char = xml_medium->Attribute("name");
                    if (medium_char != nullptr) {
                        std::string name_medium = medium_char;
                        std::transform(name_medium.begin(), name_medium.end(), name_medium.begin(), ::tolower);
                        if (name_medium == token) {
                            medium_list.push_back(mediums_[index].get());
                            missing = false;
                            break;
                        }
                    }
                    ++index;
                }
                if (missing) {
                    std::cerr << "Error: Medium '" << token << "' not found. Exiting." << std::endl;
                    exit(496);
                }
            }
            else {
                std::cerr << "Error: No mediums, medium '" << token << "' not found. Exiting." << std::endl;
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
        if (xml_mediums != nullptr) {
            bool missing = true;
            std::transform(string_medium_list.begin(), string_medium_list.end(), string_medium_list.begin(), ::tolower);
            size_t index = 0;
            for (const tinyxml2::XMLElement* xml_medium = xml_mediums->FirstChildElement("medium"); xml_medium; xml_medium = xml_medium->NextSiblingElement("medium")) {
                const char* medium_char = xml_medium->Attribute("name");
                if (medium_char != nullptr) {
                    std::string name_medium = medium_char;
                    std::transform(name_medium.begin(), name_medium.end(), name_medium.begin(), ::tolower);
                    if (name_medium == string_medium_list) {
                        medium_list.push_back(mediums_[index].get());
                        missing = false;
                        break;
                    }
                }
                ++index;
            }
            if (missing) {
                std::cerr << "Error: Medium '" << string_medium_list << "' not found. Exiting." << std::endl;
                exit(496);
            }
        }
        else {
            std::cerr << "Error: No mediums, medium '" << string_medium_list << "' not found. Exiting." << std::endl;
            exit(497);
        }
    }
    return medium_list;
}

auto APTracer::Entities::SceneContext_t::get_texture(std::string texture, const tinyxml2::XMLElement* xml_textures) const -> Texture_t* {
    if (is_number(texture)) {
        return textures_[std::stoi(texture) - 1].get();
    }
    
    if (xml_textures != nullptr) {
        std::transform(texture.begin(), texture.end(), texture.begin(), ::tolower);
        size_t index = 0;
        for (const tinyxml2::XMLElement* xml_texture = xml_textures->FirstChildElement("texture"); xml_texture; xml_texture = xml_texture->NextSiblingElement("texture")) {
            const char* name_char = xml_texture->Attribute("name");
            if (name_char != nullptr) {
                std::string name_texture = name_char;
                std::transform(name_texture.begin(), name_texture.end(), name_texture.begin(), ::tolower);
                if (name_texture == texture) {
                    return textures_[index].get();
                }
            }
            ++index;
        }
    }
    
    std::cerr << "Error. Texture '" << texture << "' not found. Exiting." << std::endl;
    exit(21); 
}

auto APTracer::Entities::SceneContext_t::get_material_mix(std::string material_refracted, std::string material_reflected, const tinyxml2::XMLElement* xml_materials) -> std::array<size_t, 2> {
    std::array<size_t, 2> output_materials;
    
    if (is_number(material_refracted)) {
        output_materials[0] = std::stoi(material_refracted) - 1;
    } 
    else {
        if (xml_materials != nullptr) {
            std::transform(material_refracted.begin(), material_refracted.end(), material_refracted.begin(), ::tolower);
            bool material_missing = true;
            size_t index = 0;
            for (const tinyxml2::XMLElement* xml_material = xml_materials->FirstChildElement("material"); xml_material; xml_material = xml_material->NextSiblingElement("material")) {
                const char* name_char = xml_material->Attribute("name");
                if (name_char != nullptr) {
                    std::string name_material = name_char;
                    std::transform(name_material.begin(), name_material.end(), name_material.begin(), ::tolower);
                    if (material_refracted == name_material) {
                        output_materials[0] = index;
                        material_missing = false;
                        break;
                    }
                }                
                ++index;
            }
            if (material_missing) {
                std::cerr << "Error: Material '" << material_refracted << "' not found. Exiting." << std::endl;
                exit(41); 
            }
        }
        else {
            std::cerr << "Error: No materials, '" << material_refracted << "' not found. Exiting." << std::endl;
            exit(47);
        }        
    }

    if (is_number(material_reflected)) {
        output_materials[1] = std::stoi(material_reflected) - 1;
    }
    else {
        if (xml_materials != nullptr) {
            std::transform(material_reflected.begin(), material_reflected.end(), material_reflected.begin(), ::tolower);
            bool material_missing = true;
            size_t index = 0;
            for (const tinyxml2::XMLElement* xml_material = xml_materials->FirstChildElement("material"); xml_material; xml_material = xml_material->NextSiblingElement("material")) {
                const char* name_char = xml_material->Attribute("name");
                if (name_char != nullptr) {
                    std::string name_material = name_char;
                    std::transform(name_material.begin(), name_material.end(), name_material.begin(), ::tolower);
                    if (material_reflected == name_material) {
                        output_materials[1] = index;
                        material_missing = false;
                        break;
                    }
                }
                ++index;
            }
            if (material_missing) {
                std::cerr << "Error: Material '" << material_reflected << "' not found. Exiting." << std::endl;
                exit(41); 
            }
        }
        else {
            std::cerr << "Error: No materials, '" << material_reflected << "' not found. Exiting." << std::endl;
            exit(47);
        }        
    }

    return output_materials;
}

auto APTracer::Entities::SceneContext_t::get_medium(std::string medium, const tinyxml2::XMLElement* xml_mediums) const -> Medium_t* {
    if (is_number(medium)) {
        return mediums_[std::stoi(medium) - 1].get();
    }
    
    if (xml_mediums != nullptr) {
        std::transform(medium.begin(), medium.end(), medium.begin(), ::tolower);
        size_t index = 0;
        for (const tinyxml2::XMLElement* xml_medium = xml_mediums->FirstChildElement("medium"); xml_medium; xml_medium = xml_medium->NextSiblingElement("medium")) {
            const char* name_char = xml_medium->Attribute("name");
            if (name_char != nullptr) {
                std::string name_medium = name_char;
                std::transform(name_medium.begin(), name_medium.end(), name_medium.begin(), ::tolower);
                if (name_medium == medium) {
                    return mediums_[index].get();
                }
            }                
            ++index;
        }
    }
    
    std::cerr << "Warning: Medium '" << medium << "' not found. Using 'nonabsorber'. This Causes a memory leak." << std::endl;
    return new APTracer::Materials::NonAbsorber_t(1.0, 0);
}

auto APTracer::Entities::SceneContext_t::get_mesh_geometry(std::string mesh_geometry, const tinyxml2::XMLElement* xml_mesh_geometries) const -> MeshGeometry_t* {
    if (is_number(mesh_geometry)) {
        return mesh_geometries_[std::stoi(mesh_geometry) - 1].get();
    }
    
    if (xml_mesh_geometries != nullptr) {
        std::transform(mesh_geometry.begin(), mesh_geometry.end(), mesh_geometry.begin(), ::tolower);
        size_t index = 0;
        for (const tinyxml2::XMLElement* xml_mesh_geometry = xml_mesh_geometries->FirstChildElement("mesh_geometry"); xml_mesh_geometry; xml_mesh_geometry = xml_mesh_geometry->NextSiblingElement("mesh_geometry")) {
            const char* name_char = xml_mesh_geometry->Attribute("name");
            if (name_char != nullptr) {
                std::string name_mesh_geometry = name_char;
                std::transform(name_mesh_geometry.begin(), name_mesh_geometry.end(), name_mesh_geometry.begin(), ::tolower);
                if (name_mesh_geometry == mesh_geometry) {
                    return mesh_geometries_[index].get();
                }
            }                
            ++index;
        }
    }
    
    std::cerr << "Error: Mesh geometry '" << mesh_geometry << "' not found. Exiting." << std::endl;
    exit(51);
}

auto APTracer::Entities::SceneContext_t::get_material_index(std::string material, const tinyxml2::XMLElement* xml_materials) -> size_t {
    if (is_number(material)) {
        return std::stoi(material) - 1;
    }
    
    if (xml_materials != nullptr) {
        std::transform(material.begin(), material.end(), material.begin(), ::tolower);
        size_t index = 0;
        for (const tinyxml2::XMLElement* xml_material = xml_materials->FirstChildElement("material"); xml_material; xml_material = xml_material->NextSiblingElement("material")) {
            const char* name_char = xml_material->Attribute("name");
            if (name_char != nullptr) {
                std::string name_material = name_char;
                std::transform(name_material.begin(), name_material.end(), name_material.begin(), ::tolower);
                if (name_material == material) {
                    return index;
                }
            }                
            ++index;
        }
    }

    std::cerr << "Error: Material '" << material << "' not found. Exiting." << std::endl;
    exit(41);
}

auto APTracer::Entities::SceneContext_t::get_material(std::string material, const tinyxml2::XMLElement* xml_materials) const -> Material_t* {
    if (is_number(material)) {
        return materials_[std::stoi(material) - 1].get();
    }

    if (xml_materials != nullptr) {
        std::transform(material.begin(), material.end(), material.begin(), ::tolower);
        size_t index = 0;
        for (const tinyxml2::XMLElement* xml_material = xml_materials->FirstChildElement("material"); xml_material; xml_material = xml_material->NextSiblingElement("material")) {
            const char* name_char = xml_material->Attribute("name");
            if (name_char != nullptr) {
                std::string name_material = name_char;
                std::transform(name_material.begin(), name_material.end(), name_material.begin(), ::tolower);
                if (name_material == material) {
                    return materials_[index].get();
                }
            }
            ++index;
        }
    }
    
    std::cerr << "Error: Material '" << material << "' not found. Exiting." << std::endl;
    exit(41);
}

auto APTracer::Entities::SceneContext_t::get_lights(std::string lights_string, const tinyxml2::XMLElement* xml_directional_lights) const -> std::vector<DirectionalLight_t*> {
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
            if (xml_directional_lights != nullptr) {
                bool missing = true;
                std::transform(token.begin(), token.end(), token.begin(), ::tolower);
                size_t index = 0;
                for (const tinyxml2::XMLElement* xml_directional_light = xml_directional_lights->FirstChildElement("directional_light"); xml_directional_light; xml_directional_light = xml_directional_light->NextSiblingElement("directional_light")) {
                    const char* name_char = xml_directional_light->Attribute("name");
                    if (name_char != nullptr) {
                        std::string name_light = name_char;
                        std::transform(name_light.begin(), name_light.end(), name_light.begin(), ::tolower);
                        if (name_light == token) {
                            lights_list.push_back(directional_lights_[index].get());
                            missing = false;
                            break;
                        }
                    }
                    ++index;
                }
                if (missing) {
                    std::cerr << "Warning: Light '" << token << "' not found. Ignoring." << std::endl;
                }
            }
            else {
                std::cerr << "Warning: No directional lights, light '" << token << "' not found. Ignoring." << std::endl;
            }            
        }
        // CHECK this should check for errors.

        lights_string.erase(0, pos + delimiter.length());
    }
    if (is_number(lights_string)) {
        lights_list.push_back(directional_lights_[std::stoi(lights_string) - 1].get());
    }
    else {
        if (xml_directional_lights != nullptr) {
            bool missing = true;
            std::transform(lights_string.begin(), lights_string.end(), lights_string.begin(), ::tolower);
            size_t index = 0;
            for (const tinyxml2::XMLElement* xml_directional_light = xml_directional_lights->FirstChildElement("directional_light"); xml_directional_light; xml_directional_light = xml_directional_light->NextSiblingElement("directional_light")) {
                const char* name_char = xml_directional_light->Attribute("name");
                if (name_char != nullptr) {
                    std::string name_light = name_char;
                    std::transform(name_light.begin(), name_light.end(), name_light.begin(), ::tolower);
                    if (name_light == lights_string) {
                        lights_list.push_back(directional_lights_[index].get());
                        missing = false;
                        break;
                    }
                }                
                ++index;
            }
            if (missing) {
                std::cerr << "Warning: Light '" << lights_string << "' not found. Ignoring." << std::endl;
            }
        }
        else {
            std::cerr << "Warning: No directional lights, light '" << lights_string << "' not found. Ignoring." << std::endl;
        }
    }

    std::vector<DirectionalLight_t*> lights(lights_list.size());
    size_t index = 0;

    for (auto* light: lights_list) {
        lights[index] = light;
        ++index;
    }

    return lights;
}

auto APTracer::Entities::SceneContext_t::get_imgbuffer(std::string imgbuffer, const tinyxml2::XMLElement* xml_imgbuffers) const -> ImgBuffer_t* {
    if (is_number(imgbuffer)) {
        return imgbuffers_[std::stoi(imgbuffer) - 1].get();
    }

    if (xml_imgbuffers != nullptr) {
        std::transform(imgbuffer.begin(), imgbuffer.end(), imgbuffer.begin(), ::tolower);
        size_t index = 0;
        for (const tinyxml2::XMLElement* xml_imgbuffer = xml_imgbuffers->FirstChildElement("imgbuffer"); xml_imgbuffer; xml_imgbuffer = xml_imgbuffer->NextSiblingElement("imgbuffer")) {
            const char* name_char = xml_imgbuffer->Attribute("name");
            if (name_char != nullptr) {
                std::string name_imgbuffer = name_char;
                std::transform(name_imgbuffer.begin(), name_imgbuffer.end(), name_imgbuffer.begin(), ::tolower);
                if (name_imgbuffer == imgbuffer) {
                    return imgbuffers_[index].get();
                }
            }
            ++index;
        }
    }

    std::cerr << "Error: Image buffer '" << imgbuffer << "' not found. Exiting." << std::endl;
    exit(91); 
}

auto APTracer::Entities::SceneContext_t::get_skybox(std::string skybox, const tinyxml2::XMLElement* xml_skyboxes) const -> Skybox_t* {
    if (is_number(skybox)) {
        return skyboxes_[std::stoi(skybox) - 1].get();
    }

    if (xml_skyboxes != nullptr) {
        std::transform(skybox.begin(), skybox.end(), skybox.begin(), ::tolower);
        size_t index = 0;
        for (const tinyxml2::XMLElement* xml_skybox = xml_skyboxes->FirstChildElement("skybox"); xml_skybox; xml_skybox = xml_skybox->NextSiblingElement("skybox")) {
            const char* name_char = xml_skybox->Attribute("name");
            if (name_char != nullptr) {
                std::string name_skybox = name_char;
                std::transform(name_skybox.begin(), name_skybox.end(), name_skybox.begin(), ::tolower);
                if (name_skybox == skybox) {
                    return skyboxes_[index].get();
                }
            }
            ++index;
        }
    }

    std::cerr << "Error: Skybox '" << skybox << "' not found. Exiting." << std::endl;
    exit(81); 
}

auto APTracer::Entities::SceneContext_t::get_objects(std::string objects_string, std::vector<Shape_t*> &shapes, std::vector<MeshTop_t*> &meshes, const tinyxml2::XMLElement* xml_objects) const -> void {
    std::list<size_t> objects_list = std::list<size_t>();
    std::string delimiter = ", ";
    size_t pos = 0;
    std::string token;
    size_t n_shapes = 0;
    size_t n_meshes = 0;

    while ((pos = objects_string.find(delimiter)) != std::string::npos) {
        token = objects_string.substr(0, pos);

        if (is_number(token)) {
            objects_list.push_back(std::stoi(token) - 1);
        }
        else {
            if (xml_objects != nullptr) {
                bool missing = true;
                std::transform(token.begin(), token.end(), token.begin(), ::tolower);
                size_t index = 0;
                for (const tinyxml2::XMLElement* xml_object = xml_objects->FirstChildElement("object"); xml_object; xml_object = xml_object->NextSiblingElement("object")) {
                    const char* name_char = xml_object->Attribute("name");
                    if (name_char != nullptr) {
                        std::string name_object = name_char;
                        std::transform(name_object.begin(), name_object.end(), name_object.begin(), ::tolower);
                        if (name_object == token) {
                            objects_list.push_back(index);
                            missing = false;
                            break;
                        }
                    }
                    ++index;
                }
                if (missing) {
                    std::cerr << "Warning: Object '" << token << "' not found. Ignoring." << std::endl;
                }
            }
            else {
                std::cerr << "Warning: No objects, object '" << token << "' not found. Ignoring." << std::endl;
            }
        }
        // CHECK this should check for errors.

        objects_string.erase(0, pos + delimiter.length());
    }
    if (is_number(objects_string)) {
        objects_list.push_back(std::stoi(objects_string) - 1);
    }
    else {
        if (xml_objects != nullptr) {
            bool missing = true;
            std::transform(objects_string.begin(), objects_string.end(), objects_string.begin(), ::tolower);
            size_t index = 0;
            for (const tinyxml2::XMLElement* xml_object = xml_objects->FirstChildElement("object"); xml_object; xml_object = xml_object->NextSiblingElement("object")) {
                const char* name_char = xml_object->Attribute("name");
                if (name_char != nullptr) {
                    std::string name_object = name_char;
                    std::transform(name_object.begin(), name_object.end(), name_object.begin(), ::tolower);
                    if (name_object == objects_string) {
                        objects_list.push_back(index);
                        missing = false;
                        break;
                    }
                }
                ++index;
            }
            if (missing) {
                std::cerr << "Warning: Object '" << objects_string << "' not found. Ignoring." << std::endl;
            }
        }
        else {
            std::cerr << "Warning: No objects, object '" << objects_string << "' not found. Ignoring." << std::endl;
        }
    }

    for (auto object_list_item : objects_list) {
        if (objects_[object_list_item]) {
            ++n_shapes;
        }
        else {
            ++n_meshes;
        }
    }

    shapes = std::vector<Shape_t*>(n_shapes);
    meshes = std::vector<MeshTop_t*>(n_meshes);
    size_t index_shapes = 0;
    size_t index_meshes = 0;

    for (auto objects_list_item : objects_list) {
        if (objects_[objects_list_item]) {
            shapes[index_shapes] = objects_[objects_list_item].get();
            ++index_shapes;
        }
        else {
            meshes[index_meshes] = meshes_[objects_list_item].get();
            ++index_meshes;
        }
    }
}

auto APTracer::Entities::SceneContext_t::get_objects(std::vector<Shape_t*> &shapes, std::vector<MeshTop_t*> &meshes) const -> void {
    size_t n_shapes = 0;
    size_t n_meshes = 0;

    for (auto const &object: objects_) {
        if (object) {
            ++n_shapes;
        }
        else {
            ++n_meshes;
        }
    }

    shapes = std::vector<Shape_t*>(n_shapes);
    meshes = std::vector<MeshTop_t*>(n_meshes);
    size_t index_shapes = 0;
    size_t index_meshes = 0;

    for (size_t i = 0; i < objects_.size(); ++i) {
        if (objects_[i]) {
            shapes[index_shapes] = objects_[i].get();
            ++index_shapes;
        }
        else {
            meshes[index_meshes] = meshes_[i].get();
            ++index_meshes;
        }
    }
}

auto APTracer::get_colour(std::string colour) -> Vec3f {
    std::transform(colour.begin(), colour.end(), colour.begin(), ::tolower);
    
    auto it = APTracer::Colours::colours.find(colour);
    if (it != APTracer::Colours::colours.end()) {
		return it->second;
	} 
    
    std::array<double, 3> values;
    unsigned int count = 0;
    std::stringstream ss(colour);

    for(std::string s; ss >> s; ) {
        if (count < 3) {
            values[count] = std::stod(s);
        }
        ++count;
    }

    if (count == 1) {
        return Vec3f(values[0]);
    }
    if (count != 3) {
        std::cerr << "Warning: Colour should be 1 or 3 values seperated by spaces, or a string. Current number of values is " << count << ", colour is '" << colour << "'. Using '0.5'." << std::endl;
        return Vec3f(0.5);
    }

    return {values};
}

auto APTracer::get_points(std::string points_string) -> std::vector<Vec3f> {
    std::vector<Vec3f> points;
    std::transform(points_string.begin(), points_string.end(), points_string.begin(), ::tolower);

    if (points_string != "nan") {
        points = std::vector<Vec3f>(3);
        std::array<double, 9> values;
        unsigned int count = 0;
        std::stringstream ss(points_string);
        
        for(std::string s; ss >> s; ) {
            if (count < 9) {
                values[count] = std::stod(s);
            }
            ++count;
        }
        if (count != 9) {
            std::cerr << "Error: Triangle points should be 9 values seperated by spaces, or nan. Current number of values is " << count << ", points are '" << points_string << "'. Exiting." << std::endl;
            exit(67);
        }
        else {
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

auto APTracer::get_texture_coordinates(std::string texture_coordinates_string) -> std::vector<double> {
    std::vector<double> texture_coordinates;

    std::transform(texture_coordinates_string.begin(), texture_coordinates_string.end(), texture_coordinates_string.begin(), ::tolower);

    if (texture_coordinates_string != "nan") {
        texture_coordinates = std::vector<double>(6);
        unsigned int count = 0;
        std::stringstream ss(texture_coordinates_string);
        
        for(std::string s; ss >> s; ) {
            if (count < 6) {
                texture_coordinates[count] = std::stod(s);
            }
            ++count;
        }
        if (count != 6) {
            std::cerr << "Error: Triangle texture coordinates should be 6 values seperated by spaces, or nan. Current number of values is " << count << ", texture coordinates are '" << texture_coordinates_string << "'­. Exiting." << std::endl;
            exit(68);
        }
    }

    return texture_coordinates;
}

auto APTracer::get_medium_names(std::string string_medium_names) -> std::unique_ptr<std::list<std::string>> {
    std::unique_ptr<std::list<std::string>> medium_names = std::make_unique<std::list<std::string>>();
    std::string delimiter = ", ";
    size_t pos = 0;

    while ((pos = string_medium_names.find(delimiter)) != std::string::npos) {
        medium_names->push_back(string_medium_names.substr(0, pos));
        string_medium_names.erase(0, pos + delimiter.length());
    }
    medium_names->push_back(string_medium_names);

    return medium_names;
}

auto APTracer::is_number(const std::string& s) -> bool {
    return !s.empty() && std::find_if(s.begin(), 
        s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

auto APTracer::apply_transformation(TransformMatrix_t* transform_matrix, const tinyxml2::XMLElement* transform) -> void {
    std::string type;
    const char* type_char = transform->Attribute("type");
    if (type_char == nullptr) {
        std::cerr << "Warning: XML transforms should have a 'type' attribute. Ignoring." << std::endl;
        return;
    }

    type = type_char;
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);

    if (type == "rotatexaxis") {
        const std::vector<const char*> attributes = {"value"};
        require_attributes(transform, attributes);
        transform_matrix->rotateXAxis(transform->DoubleAttribute("value"));
    }
    else if (type == "rotateyaxis") {
        const std::vector<const char*> attributes = {"value"};
        require_attributes(transform, attributes);
        transform_matrix->rotateYAxis(transform->DoubleAttribute("value"));
    }
    else if (type == "rotatezaxis") {
        const std::vector<const char*> attributes = {"value"};
        require_attributes(transform, attributes);
        transform_matrix->rotateZAxis(transform->DoubleAttribute("value"));
    }
    else if (type == "rotatex") {
        const std::vector<const char*> attributes = {"value"};
        require_attributes(transform, attributes);
        transform_matrix->rotateX(transform->DoubleAttribute("value"));
    }
    else if (type == "rotatey") {
        const std::vector<const char*> attributes = {"value"};
        require_attributes(transform, attributes);
        transform_matrix->rotateY(transform->DoubleAttribute("value"));
    }
    else if (type == "rotatez") {
        const std::vector<const char*> attributes = {"value"};
        require_attributes(transform, attributes);
        transform_matrix->rotateZ(transform->DoubleAttribute("value"));
    }
    else if (type == "rotateaxis") {
        const std::vector<const char*> attributes = {"value", "axis"};
        require_attributes(transform, attributes);
        transform_matrix->rotateAxis(APTracer::get_colour(transform->Attribute("axis")), transform->DoubleAttribute("value"));
    }
    else if (type == "rotate") {
        const std::vector<const char*> attributes = {"value", "axis"};
        require_attributes(transform, attributes);
        transform_matrix->rotate(APTracer::get_colour(transform->Attribute("axis")), transform->DoubleAttribute("value"));
    }
    else if (type == "translate") {
        const std::vector<const char*> attributes = {"value"};
        require_attributes(transform, attributes);
        transform_matrix->translate(APTracer::get_colour(transform->Attribute("value")));
    }
    else if (type == "scaleaxis") {
        const std::vector<const char*> attributes = {"value"};
        require_attributes(transform, attributes);
        transform_matrix->scaleAxis(APTracer::get_colour(transform->Attribute("value")));
    }
    else if (type == "uniformscaleaxis") {
        const std::vector<const char*> attributes = {"value"};
        require_attributes(transform, attributes);
        transform_matrix->scaleAxis(transform->DoubleAttribute("value"));
    }
    else if (type == "scale") {
        const std::vector<const char*> attributes = {"value"};
        require_attributes(transform, attributes);
        transform_matrix->scale(APTracer::get_colour(transform->Attribute("value")));
    }
    else if (type == "uniformscale") {
        const std::vector<const char*> attributes = {"value"};
        require_attributes(transform, attributes);
        transform_matrix->scale(transform->DoubleAttribute("value"));
    }
    else if (type == "reflect") {
        const std::vector<const char*> attributes = {"value"};
        require_attributes(transform, attributes);
        transform_matrix->reflect(APTracer::get_colour(transform->Attribute("value")));
    }
    else if (type == "shear") {
        const std::vector<const char*> attributes = {"value"};
        require_attributes(transform, attributes);
        transform_matrix->shear(APTracer::get_colour(transform->Attribute("value")));
    }
    else if (type == "transpose") {
        transform_matrix->transpose();
    }
    else if (type == "invert") {
        transform_matrix->invert();
    }
    else if (type == "neg") {
        transform_matrix->neg();
    }
    else {
        std::cerr << "Warning: Transform type '" << type << "' not implemented. Ignoring." << std::endl; 
    }
}

auto APTracer::require_attributes(const tinyxml2::XMLElement* element, const std::vector<const char*>& attributes) -> void {
    bool missing = false;

    for (const auto* attribute: attributes) {
        const char* value = element->Attribute(attribute);
        if (value == nullptr) {
            if (!missing) {
                missing = true;
                const char* name_char = element->Attribute("name");
                const char* type_type = element->Attribute("type");
                std::string name = (name_char == nullptr) ? "" : name_char;
                std::string type = (type_type == nullptr) ? "" : type_type;
                std::cerr << "Error: " << element->Name() << " XML element with name '" << name << "' and type '" << type << "' has the following attributes missing:" << std::endl;
            }
            std::cerr << '\t' << attribute << std::endl;
        }        
    }

    if (missing) {
        std::cerr << "Exiting." << std::endl;
        exit(2); 
    }
}