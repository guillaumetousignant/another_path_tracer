#ifndef SCENECONTEXT_T_H
#define SCENECONTEXT_T_H

#include <string>
#include <list>
#include <tuple>
#include <vector>
#include <memory>
#include "tinyxml2.h"
#include "Vec3f.h"

namespace APTracer::Shapes{
    class MeshTop_t;
}
namespace APTracer::Entities{
    class TransformMatrix_t;
    class Texture_t;
    class ScatteringFunction_t;
    class Material_t;
    class Medium_t;
    class MeshGeometry_t;
    class Shape_t;
    class DirectionalLight_t;
    class Skybox_t;
    class ImgBuffer_t;
    class Camera_t;
    class MaterialMap_t;
    class ImgBufferOpenGL_t;
    class Scene_t;
    class OpenGLRenderer_t;
}

using namespace APTracer::Shapes;
using namespace APTracer::Entities;

namespace APTracer::Entities {
    class SceneContext_t{
        public:
            SceneContext_t();
            ~SceneContext_t();

            bool use_gl_;
            std::string scene_name_;
            std::unique_ptr<OpenGLRenderer_t> opengl_renderer_;
            ImgBufferOpenGL_t* opengl_imgbuffer_; // weak ptr
            Camera_t* opengl_camera_; // weak ptr
            std::unique_ptr<Scene_t> scene_;

            std::vector<std::string> camera_rendermode_;
            std::vector<unsigned int> camera_n_iter_;
            std::vector<unsigned int> camera_write_interval_;

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
            
            std::vector<std::unique_ptr<TransformMatrix_t>> transform_matrices_;
            std::vector<std::unique_ptr<Texture_t>> textures_;
            std::vector<std::unique_ptr<ScatteringFunction_t>> scatterers_;
            std::vector<std::unique_ptr<Material_t>> materials_;
            std::vector<std::unique_ptr<MeshGeometry_t>> mesh_geometries_;
            std::vector<std::unique_ptr<Shape_t>> objects_;
            std::vector<std::unique_ptr<DirectionalLight_t>> directional_lights_;
            std::vector<std::unique_ptr<Skybox_t>> skyboxes_;
            std::vector<std::unique_ptr<ImgBuffer_t>> imgbuffers_;
            std::vector<std::unique_ptr<Camera_t>> cameras_;
            std::vector<std::unique_ptr<MaterialMap_t>> material_aggregates_;
            std::vector<std::unique_ptr<MeshTop_t>> meshes_;

            void readXML(const std::string &filename);
            void render();
        
        private:
            void reset();

            std::unique_ptr<TransformMatrix_t> create_transform_matrix(const tinyxml2::XMLElement* xml_transform_matrix) const;
            std::unique_ptr<Texture_t> create_texture(const tinyxml2::XMLElement* xml_texture) const;
            std::unique_ptr<ScatteringFunction_t> create_scatterer(const tinyxml2::XMLElement* xml_scatterer, std::unique_ptr<std::list<unsigned int>> &scatterers_medium_list, const tinyxml2::XMLElement* xml_transform_matrices, const tinyxml2::XMLElement* xml_materials);
            std::unique_ptr<Material_t> create_material(const tinyxml2::XMLElement* xml_material, std::unique_ptr<std::list<unsigned int>> &materials_medium_list, std::vector<unsigned int> &materials_mix_list, std::unique_ptr<std::tuple<std::unique_ptr<std::list<unsigned int>>, std::unique_ptr<std::list<std::string>>>> &materials_aggregate_list, const tinyxml2::XMLElement* xml_textures, const tinyxml2::XMLElement* xml_transform_matrices, const tinyxml2::XMLElement* xml_materials, const tinyxml2::XMLElement* xml_scatterers);
            std::unique_ptr<MeshGeometry_t> create_mesh_geometry(const tinyxml2::XMLElement* xml_mesh_geometry) const;
            std::unique_ptr<Shape_t> create_object(const tinyxml2::XMLElement* xml_object, std::unique_ptr<MeshTop_t> &mesh, const tinyxml2::XMLElement* xml_transform_matrices, const tinyxml2::XMLElement* xml_materials, const tinyxml2::XMLElement* xml_mesh_geometries);
            std::unique_ptr<DirectionalLight_t> create_directional_light(const tinyxml2::XMLElement* xml_directional_light, const tinyxml2::XMLElement* xml_transform_matrices);
            std::unique_ptr<Skybox_t> create_skybox(const tinyxml2::XMLElement* xml_skybox, const tinyxml2::XMLElement* xml_textures, const tinyxml2::XMLElement* xml_transform_matrices, const tinyxml2::XMLElement* xml_directional_lights);
            std::unique_ptr<ImgBuffer_t> create_imgbuffer(const tinyxml2::XMLElement* xml_imgbuffer);
            std::unique_ptr<Camera_t> create_camera(const tinyxml2::XMLElement* xml_camera, const std::string &next_filename, const tinyxml2::XMLElement* xml_transform_matrices, const tinyxml2::XMLElement* xml_materials, const tinyxml2::XMLElement* xml_imgbuffers, const tinyxml2::XMLElement* xml_skyboxes);
            void create_acceleration_structure(const tinyxml2::XMLElement* xml_acceleration_structure);

            TransformMatrix_t* get_transform_matrix(std::string transform_matrix, const tinyxml2::XMLElement* xml_transform_matrices);
            std::unique_ptr<std::list<unsigned int>> get_medium_index_list(std::string string_medium_list, const tinyxml2::XMLElement* xml_materials) const;
            std::list<Medium_t*> get_medium_list(std::string string_medium_list, const tinyxml2::XMLElement* xml_materials) const;
            Texture_t* get_texture(std::string texture, const tinyxml2::XMLElement* xml_textures) const;
            std::vector<unsigned int> get_material_mix(std::string material_refracted, std::string material_reflected, const tinyxml2::XMLElement* xml_materials) const;
            ScatteringFunction_t* get_scatterer(std::string scatterer, const tinyxml2::XMLElement* xml_scatterers) const;
            MeshGeometry_t* get_mesh_geometry(std::string mesh_geometry, const tinyxml2::XMLElement* xml_mesh_geometries) const;
            unsigned int get_material_index(std::string material, const tinyxml2::XMLElement* xml_materials) const;
            Material_t* get_material(std::string material, const tinyxml2::XMLElement* xml_materials) const;
            void get_lights(std::string lights_string, std::vector<DirectionalLight_t*> &lights, const tinyxml2::XMLElement* xml_directional_lights) const;
            ImgBuffer_t* get_imgbuffer(std::string imgbuffer, const tinyxml2::XMLElement* xml_imgbuffers) const;
            Skybox_t* get_skybox(std::string skybox, const tinyxml2::XMLElement* xml_skyboxes) const;
            void get_objects(std::string objects_string, std::vector<Shape_t*> &shapes, std::vector<MeshTop_t*> &meshes, const tinyxml2::XMLElement* xml_objects) const;
            void get_objects(std::vector<Shape_t*> &shapes, std::vector<MeshTop_t*> &meshes) const;
    };
}

namespace APTracer {
    Vec3f get_colour(std::string colour); // copies string :(
    std::vector<Vec3f> get_points(std::string points_string);
    std::vector<std::vector<double>> get_texture_coordinates(std::string texture_coordinates_string);    
    std::unique_ptr<std::list<std::string>> get_medium_names(std::string string_medium_names);
    void get_xy(const std::string &string_value, double (&value)[2]);
    void get_xy(const std::string &string_value, unsigned int (&value)[2]);

    bool is_number(const std::string& s);
    void apply_transformation(TransformMatrix_t* transform_matrix, const tinyxml2::XMLElement* transform);
    void require_attributes(const tinyxml2::XMLElement* element, const char** attributes, unsigned int n);
}

#endif