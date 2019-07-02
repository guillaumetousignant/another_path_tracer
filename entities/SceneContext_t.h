#ifndef SCENECONTEXT_T_H
#define SCENECONTEXT_T_H

#include <string>
#include <list>
#include "tinyxml2.h"

class TransformMatrix_t;
class Texture_t;
class ScatteringFunction_t;
class Material_t;
class MeshGeometry_t;
class Shape_t;
class DirectionalLight_t;
class Skybox_t;
class ImgBuffer_t;
class Camera_t;

class OpenGLRenderer_t;

class SceneContext_t{
    public:
        SceneContext_t();
        ~SceneContext_t();

        bool use_gl_;
        std::string scene_name_;
        OpenGLRenderer_t* opengl_renderer_;

        unsigned int n_transform_matrices_;
        unsigned int n_textures_;
        unsigned int n_scatterers_;
        unsigned int n_materials_;
        unsigned int n_mesh_geometries_;
        unsigned int n_objects_;
        unsigned int n_directional_lights_;
        unsigned int n_skyboxes_;
        unsigned int n_imgbuffers_;
        unsigned int n_cameras_;

        unsigned int index_transform_matrices_;
        unsigned int index_textures_;
        unsigned int index_scatterers_;
        unsigned int index_materials_;
        unsigned int index_mesh_geometries_;
        unsigned int index_objects_;
        unsigned int index_directional_lights_;
        unsigned int index_skyboxes_;
        unsigned int index_imgbuffers_;
        unsigned int index_cameras_;
        
        TransformMatrix_t** transform_matrices_;
        Texture_t** textures_;
        ScatteringFunction_t** scatterers_;
        Material_t** materials_;
        MeshGeometry_t** mesh_geometries_;
        Shape_t** objects_;
        DirectionalLight_t** directional_lights_;
        Skybox_t** skyboxes_;
        ImgBuffer_t** imgbuffers_;
        Camera_t** cameras_;

        void readXML(const std::string &filename);
        void render();
    
    private:
        void reset();
        TransformMatrix_t* create_transform_matrix(const tinyxml2::XMLElement* xml_transform_matrix) const;
        Texture_t* create_texture(const tinyxml2::XMLElement* xml_texture) const;
        ScatteringFunction_t* create_scatterer(const tinyxml2::XMLElement* xml_scatterer, std::list<unsigned int>* &scatterers_medium_list, const tinyxml2::XMLElement* xml_transform_matrices, const tinyxml2::XMLElement* xml_materials);
        Material_t* create_material(const tinyxml2::XMLElement* xml_material, std::list<unsigned int>* &materials_medium_list, unsigned int* &materials_mix_list, const tinyxml2::XMLElement* xml_textures, const tinyxml2::XMLElement* xml_transform_matrices, const tinyxml2::XMLElement* xml_materials, const tinyxml2::XMLElement* xml_scatterers);
        
        Vec3f get_colour(std::string colour) const; // copies string :(
        TransformMatrix_t* get_transform_matrix(std::string transform_matrix, const tinyxml2::XMLElement* xml_transform_matrices);
        std::list<unsigned int>* get_medium_list(std::string string_medium_list, const tinyxml2::XMLElement* xml_materials) const;
        Texture_t* get_texture(std::string texture, const tinyxml2::XMLElement* xml_textures) const;
        unsigned int* get_material_mix(std::string material_refracted, std::string material_reflected, const tinyxml2::XMLElement* xml_materials) const;
        ScatteringFunction_t* get_scatterer(std::string scatterer, const tinyxml2::XMLElement* xml_scatterers) const;
        bool is_number(const std::string& s) const;
};

#endif