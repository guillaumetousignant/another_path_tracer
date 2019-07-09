#ifndef SCENECONTEXT_T_H
#define SCENECONTEXT_T_H

#include <string>
#include <list>
#include <tuple>
#include "tinyxml2.h"
#include "Vec3f.h"

class TransformMatrix_t;
class Texture_t;
class ScatteringFunction_t;
class Material_t;
class Medium_t;
class MeshGeometry_t;
class MeshTop_t;
class Shape_t;
class DirectionalLight_t;
class Skybox_t;
class ImgBuffer_t;
class Camera_t;
class MaterialMap_t;
class ImgBufferOpenGL_t;
class Scene_t;

class OpenGLRenderer_t;

class SceneContext_t{
    public:
        SceneContext_t();
        ~SceneContext_t();

        bool use_gl_;
        std::string scene_name_;
        OpenGLRenderer_t* opengl_renderer_;
        ImgBufferOpenGL_t* opengl_imgbuffer_;
        Camera_t* opengl_camera_;
        Scene_t* scene_;

        std::string* camera_rendermode_;
        unsigned int* camera_n_iter_;
        unsigned int* camera_write_interval_;

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
        MaterialMap_t** material_aggregates_;

        void readXML(const std::string &filename);
        void render();
    
    private:
        void reset();

        TransformMatrix_t* create_transform_matrix(const tinyxml2::XMLElement* xml_transform_matrix) const;
        Texture_t* create_texture(const tinyxml2::XMLElement* xml_texture) const;
        ScatteringFunction_t* create_scatterer(const tinyxml2::XMLElement* xml_scatterer, std::list<unsigned int>* &scatterers_medium_list, const tinyxml2::XMLElement* xml_transform_matrices, const tinyxml2::XMLElement* xml_materials);
        Material_t* create_material(const tinyxml2::XMLElement* xml_material, std::list<unsigned int>* &materials_medium_list, unsigned int* &materials_mix_list, std::tuple<std::list<unsigned int>*, std::list<std::string>*>* &materials_aggregate_list, const tinyxml2::XMLElement* xml_textures, const tinyxml2::XMLElement* xml_transform_matrices, const tinyxml2::XMLElement* xml_materials, const tinyxml2::XMLElement* xml_scatterers);
        MeshGeometry_t* create_mesh_geometry(const tinyxml2::XMLElement* xml_mesh_geometry) const;
        Shape_t* create_object(const tinyxml2::XMLElement* xml_object, const tinyxml2::XMLElement* xml_transform_matrices, const tinyxml2::XMLElement* xml_materials, const tinyxml2::XMLElement* xml_mesh_geometries);
        DirectionalLight_t* create_directional_light(const tinyxml2::XMLElement* xml_directional_light, const tinyxml2::XMLElement* xml_transform_matrices);
        Skybox_t* create_skybox(const tinyxml2::XMLElement* xml_skybox, const tinyxml2::XMLElement* xml_textures, const tinyxml2::XMLElement* xml_transform_matrices, const tinyxml2::XMLElement* xml_directional_lights);
        ImgBuffer_t* create_imgbuffer(const tinyxml2::XMLElement* xml_imgbuffer);
        Camera_t* create_camera(const tinyxml2::XMLElement* xml_camera, const std::string &next_filename, const tinyxml2::XMLElement* xml_transform_matrices, const tinyxml2::XMLElement* xml_materials, const tinyxml2::XMLElement* xml_imgbuffers, const tinyxml2::XMLElement* xml_skyboxes);

        TransformMatrix_t* get_transform_matrix(std::string transform_matrix, const tinyxml2::XMLElement* xml_transform_matrices);
        std::list<unsigned int>* get_medium_index_list(std::string string_medium_list, const tinyxml2::XMLElement* xml_materials) const;
        std::list<Medium_t*> get_medium_list(std::string string_medium_list, const tinyxml2::XMLElement* xml_materials) const;
        Texture_t* get_texture(std::string texture, const tinyxml2::XMLElement* xml_textures) const;
        unsigned int* get_material_mix(std::string material_refracted, std::string material_reflected, const tinyxml2::XMLElement* xml_materials) const;
        ScatteringFunction_t* get_scatterer(std::string scatterer, const tinyxml2::XMLElement* xml_scatterers) const;
        MeshGeometry_t* get_mesh_geometry(std::string mesh_geometry, const tinyxml2::XMLElement* xml_mesh_geometries) const;
        unsigned int get_material_index(std::string material, const tinyxml2::XMLElement* xml_materials) const;
        Material_t* get_material(std::string material, const tinyxml2::XMLElement* xml_materials) const;
        void get_lights(std::string lights_string, DirectionalLight_t** &lights, unsigned int &n, const tinyxml2::XMLElement* xml_directional_lights) const;
        ImgBuffer_t* get_imgbuffer(std::string imgbuffer, const tinyxml2::XMLElement* xml_imgbuffers) const;
        Skybox_t* get_skybox(std::string skybox, const tinyxml2::XMLElement* xml_skyboxes) const;
        void get_shapes(std::string objects_string, Shape_t** &objects, unsigned int &n, const tinyxml2::XMLElement* xml_objects) const;
        void get_meshes(std::string meshes_string, MeshTop_t** &meshes, unsigned int &n, const tinyxml2::XMLElement* xml_objects) const;
};

Vec3f get_colour(std::string colour); // copies string :(
Vec3f* get_points(std::string points_string);
double** get_texture_coordinates(std::string texture_coordinates_string);    
std::list<std::string>* get_medium_names(std::string string_medium_names);
void get_xy(const std::string &string_value, double (&value)[2]);
void get_xy(const std::string &string_value, unsigned int (&value)[2]);

bool is_number(const std::string& s);
void apply_transformation(TransformMatrix_t* transform_matrix, const tinyxml2::XMLElement* transform);

#endif