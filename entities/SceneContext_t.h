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
        void reset();
        TransformMatrix_t* create_transform_matrix(const tinyxml2::XMLElement* xml_transform_matrix) const;
        Texture_t* create_texture(const tinyxml2::XMLElement* xml_texture) const;
        ScatteringFunction_t* create_scatterer(const tinyxml2::XMLElement* xml_scatterer, std::list<unsigned int>** scatterers_medium_list);
        Vec3f get_colour(std::string &colour); // not const string
        void render();
};

#endif