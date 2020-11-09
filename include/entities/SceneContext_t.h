#ifndef SCENECONTEXT_T_H
#define SCENECONTEXT_T_H

#include <string>
#include <list>
#include <tuple>
#include <vector>
#include <memory>
#include "functions/tinyxml2.h"
#include "entities/Vec3f.h"

namespace APTracer { namespace Shapes{
    class MeshTop_t;
}}
namespace APTracer { namespace Entities{
    class TransformMatrix_t;
    class Texture_t;
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
}}

using namespace APTracer::Shapes;
using namespace APTracer::Entities;

namespace APTracer { namespace Entities {

    /**
     * @brief The scene context class contains all data required to draw a static scene. It can read xml scene files representing many objects.
     * 
     * The scene context holds arrays of every object needed to render a static scene. It uses smart pointers everywhere to simplify memory management. 
     * The scene context can be initialised by hand, or can read from xml scene files. Several of those files are included with the project, in the
     * scenes folder. Refer to them for format.
     */
    class SceneContext_t{
        public:
            /**
             * @brief Construct a new empty SceneContext_t object.
             */
            SceneContext_t();

            /**
             * @brief Destroy the SceneContext_t object. Calls the reset function.
             */
            ~SceneContext_t();

            bool use_gl_; /**< @brief True if the scenes will be shown on the screen with OpenGL.*/
            std::string scene_name_; /**< @brief Name of the scene. Default image filename.*/
            std::unique_ptr<OpenGLRenderer_t> opengl_renderer_; /**< @brief OpenGL renderer object that will render the scene, if enabled.*/
            ImgBufferOpenGL_t* opengl_imgbuffer_; /**< @brief Pointer to the imagebuffer to be used by OpenGL. Does not own the object.*/ // weak ptr
            Camera_t* opengl_camera_; /**< @brief Pointer to the camera to be used by OpenGL. Does not own the object.*/ // weak ptr
            std::unique_ptr<Scene_t> scene_; /**< @brief Scene to be rendered.*/

            std::vector<std::string> camera_rendermode_; /**< @brief Vector of strings representing the rendering mode of every camera.*/
            std::vector<unsigned int> camera_n_iter_; /**< @brief Vector of the number of samples to accumulate per camera.*/
            std::vector<unsigned int> camera_write_interval_; /**< @brief Vector of the write interval of cameras.*/

            size_t index_transform_matrices_; /**< @brief Index of the next transformation matrix in the vector.*/
            size_t index_textures_; /**< @brief Index of the next texture in the vector.*/
            size_t index_mediums_; /**< @brief Index of the next medium in the vector.*/
            size_t index_materials_; /**< @brief Index of the next material in the vector.*/
            size_t index_mesh_geometries_; /**< @brief Index of the next mesh geometry in the vector.*/
            size_t index_objects_; /**< @brief Index of the next shape in the vector.*/
            size_t index_directional_lights_; /**< @brief Index of the next directional light in the vector.*/
            size_t index_skyboxes_; /**< @brief Index of the next skybox in the vector.*/
            size_t index_imgbuffers_; /**< @brief Index of the next image buffer in the vector.*/
            size_t index_cameras_; /**< @brief Index of the next camera in the vector.*/
            
            std::vector<std::unique_ptr<TransformMatrix_t>> transform_matrices_; /**< @brief Vector containing the transformation matrices.*/
            std::vector<std::unique_ptr<Texture_t>> textures_; /**< @brief Vector containing the textures.*/
            std::vector<std::unique_ptr<Medium_t>> mediums_; /**< @brief Vector containing the mediums.*/
            std::vector<std::unique_ptr<Material_t>> materials_; /**< @brief Vector containing the materials.*/
            std::vector<std::unique_ptr<MeshGeometry_t>> mesh_geometries_; /**< @brief Vector containing the mesh geometries.*/
            std::vector<std::unique_ptr<Shape_t>> objects_; /**< @brief Vector containing the shapes.*/
            std::vector<std::unique_ptr<DirectionalLight_t>> directional_lights_; /**< @brief Vector containing the directional lights.*/
            std::vector<std::unique_ptr<Skybox_t>> skyboxes_; /**< @brief Vector containing the skyboxes.*/
            std::vector<std::unique_ptr<ImgBuffer_t>> imgbuffers_; /**< @brief Vector containing the image buffers.*/
            std::vector<std::unique_ptr<Camera_t>> cameras_; /**< @brief Vector containing the cameras.*/
            std::vector<std::unique_ptr<MaterialMap_t>> material_aggregates_; /**< @brief Vector containing the material aggregates. Elements are nullptr for regular materials.*/
            std::vector<std::unique_ptr<MeshTop_t>> meshes_; /**< @brief Vector containing the meshes.*/

            /**
             * @brief Reads a scene xml file, initialising the scene context from it.
             * 
             * Example files are available in the scenes folder.
             * 
             * @param filename Path to the scene file to be read, in xml format.
             */
            void readXML(const std::string &filename);

            /**
             * @brief Starts rendering of all the scene's cameras, according to their mode.
             */
            void render();
        
        private:
            /**
             * @brief Set the scene context to empty, so it can be filled again or deleted.
             */
            void reset();

            /**
             * @brief Create a transformation matrix object from an xml entry.
             * 
             * @param xml_transform_matrix XML entry containing the transformation matrix information.
             * @return std::unique_ptr<TransformMatrix_t> Unique pointer to the created transformation matrix.
             */
            std::unique_ptr<TransformMatrix_t> create_transform_matrix(const tinyxml2::XMLElement* xml_transform_matrix) const;

            /**
             * @brief Create a texture object from an xml entry.
             * 
             * @param xml_texture XML entry containing the texture information.
             * @return std::unique_ptr<Texture_t> Unique pointer to the created texture.
             */
            std::unique_ptr<Texture_t> create_texture(const tinyxml2::XMLElement* xml_texture) const;

            /**
             * @brief Create a medium object from an xml entry.
             * 
             * @param xml_medium XML entry containing the medium information.
             * @param[out] mediums_medium_list List of mediums in which the current medium is placed, if needed.
             * @param xml_transform_matrices XML entries of all the transformation matrices.
             * @param xml_mediums XML entries of all the mediums.
             * @return std::unique_ptr<Medium_t> Unique pointer to the created medium.
             */
            std::unique_ptr<Medium_t> create_medium(const tinyxml2::XMLElement* xml_medium, std::unique_ptr<std::list<size_t>> &mediums_medium_list, const tinyxml2::XMLElement* xml_transform_matrices, const tinyxml2::XMLElement* xml_mediums);
            
            /**
             * @brief Create a material object from an xml entry.
             * 
             * @param xml_material XML entry containing the material information.
             * @param[out] materials_medium_list List of mediums in which the current material is placed, if needed.
             * @param[out] materials_mix_list Indexes of the materials making up a material mix, if needed.
             * @param[out] materials_aggregate_list Tuple of a list of material indices and a list of material names making up a material aggregate, if needed.
             * @param xml_textures XML entries of all the textures.
             * @param xml_transform_matrices XML entries of all the transformation matrices.
             * @param xml_materials XML entries of all the materials.
             * @param xml_mediums XML entries of all the mediums.
             * @return std::unique_ptr<Material_t> Unique pointer to the created material.
             */
            std::unique_ptr<Material_t> create_material(const tinyxml2::XMLElement* xml_material, std::unique_ptr<std::list<size_t>> &materials_medium_list, std::vector<size_t> &materials_mix_list, std::unique_ptr<std::tuple<std::unique_ptr<std::list<size_t>>, std::unique_ptr<std::list<std::string>>>> &materials_aggregate_list, const tinyxml2::XMLElement* xml_textures, const tinyxml2::XMLElement* xml_transform_matrices, const tinyxml2::XMLElement* xml_materials, const tinyxml2::XMLElement* xml_mediums);
            
            /**
             * @brief Create a mesh geometry object from an xml entry.
             * 
             * @param xml_mesh_geometry XML entry containing the mesh geometry information.
             * @return std::unique_ptr<MeshGeometry_t> Unique pointer to the created mesh geometry.
             */
            std::unique_ptr<MeshGeometry_t> create_mesh_geometry(const tinyxml2::XMLElement* xml_mesh_geometry) const;

            /**
             * @brief Create a shape object from an xml entry.
             * 
             * @param xml_object XML entry containing the shape information.
             * @param[out] mesh Unique pointer to the created mesh, if the shape is a mesh. In this case, the return value is nullptr.
             * @param xml_transform_matrices XML entries of all the transformation matrices.
             * @param xml_materials XML entries of all the materials.
             * @param xml_mesh_geometries XML entries of all the mesh geometries.
             * @return std::unique_ptr<Shape_t> Unique pointer to the created shape. Nullptr if the shape is a mesh.
             */
            std::unique_ptr<Shape_t> create_object(const tinyxml2::XMLElement* xml_object, std::unique_ptr<MeshTop_t> &mesh, const tinyxml2::XMLElement* xml_transform_matrices, const tinyxml2::XMLElement* xml_materials, const tinyxml2::XMLElement* xml_mesh_geometries);
            
            /**
             * @brief Create a directional light object from an xml entry.
             * 
             * @param xml_directional_light XML entry containing the directional light information.
             * @param xml_transform_matrices XML entries of all the transformation matrices.
             * @return std::unique_ptr<DirectionalLight_t> Unique pointer to the created directional light.
             */
            std::unique_ptr<DirectionalLight_t> create_directional_light(const tinyxml2::XMLElement* xml_directional_light, const tinyxml2::XMLElement* xml_transform_matrices);
            
            /**
             * @brief Create a skybox object from an xml entry.
             * 
             * @param xml_skybox XML entry containing the skybox information.
             * @param xml_textures XML entries of all the textures.
             * @param xml_transform_matrices XML entries of all the transformation matrices.
             * @param xml_directional_lights XML entries of all the directional lights.
             * @return std::unique_ptr<Skybox_t> Unique pointer to the created skybox.
             */
            std::unique_ptr<Skybox_t> create_skybox(const tinyxml2::XMLElement* xml_skybox, const tinyxml2::XMLElement* xml_textures, const tinyxml2::XMLElement* xml_transform_matrices, const tinyxml2::XMLElement* xml_directional_lights);
            
            /**
             * @brief Create a image buffer object from an xml entry.
             * 
             * @param xml_imgbuffer XML entry containing the image buffer information.
             * @return std::unique_ptr<ImgBuffer_t> Unique pointer to the created image buffer.
             */
            std::unique_ptr<ImgBuffer_t> create_imgbuffer(const tinyxml2::XMLElement* xml_imgbuffer);

            /**
             * @brief Create a camera object from an xml entry.
             * 
             * @param xml_camera XML entry containing the camera information.
             * @param next_filename Path to the next available filename comprised of the scene name and a sequential number.
             * @param xml_transform_matrices XML entries of all the transformation matrices.
             * @param xml_mediums XML entries of all the mediums.
             * @param xml_imgbuffers XML entries of all the image buffers.
             * @param xml_skyboxes XML entries of all the skyboxes.
             * @return std::unique_ptr<Camera_t> Unique pointer to the created camera.
             */
            std::unique_ptr<Camera_t> create_camera(const tinyxml2::XMLElement* xml_camera, const std::string &next_filename, const tinyxml2::XMLElement* xml_transform_matrices, const tinyxml2::XMLElement* xml_mediums, const tinyxml2::XMLElement* xml_imgbuffers, const tinyxml2::XMLElement* xml_skyboxes);
            
            /**
             * @brief Create a acceleration structure object from an xml entry and bind it to the scene.
             * 
             * @param xml_acceleration_structure XML entry containing the acceleration structurecod information.
             */
            void create_acceleration_structure(const tinyxml2::XMLElement* xml_acceleration_structure);

            /**
             * @brief Get a transform matrix object from its name or index.
             * 
             * @param transform_matrix String containing a transformation matrix name or index.
             * @param xml_transform_matrices XML entries of all the transformation matrices.
             * @return TransformMatrix_t* Pointer to the requested transformation matrix.
             */
            TransformMatrix_t* get_transform_matrix(std::string transform_matrix, const tinyxml2::XMLElement* xml_transform_matrices);

            /**
             * @brief Get a list of material indices from a string of names of indices.
             * 
             * @param string_material_list String containing material names and/or indices.
             * @param xml_materials XML entries of all the materials.
             * @return std::unique_ptr<std::list<size_t>> List of material indices.
             */
            std::unique_ptr<std::list<size_t>> get_material_index_list(std::string string_material_list, const tinyxml2::XMLElement* xml_materials) const;

            /**
             * @brief Get a list of medium indices from a string of names of indices.
             * 
             * @param string_medium_list String containing medium names and/or indices.
             * @param xml_mediums XML entries of all the mediums.
             * @return std::unique_ptr<std::list<size_t>> List of medium indices.
             */
            std::unique_ptr<std::list<size_t>> get_medium_index_list(std::string string_medium_list, const tinyxml2::XMLElement* xml_mediums) const;

            /**
             * @brief Get a list of mediums from a string of names of indices.
             * 
             * @param string_medium_list String containing medium names and/or indices.
             * @param xml_mediums XML entries of all the mediums.
             * @return std::list<Medium_t*> List of mediums.
             */
            std::list<Medium_t*> get_medium_list(std::string string_medium_list, const tinyxml2::XMLElement* xml_mediums) const;

            /**
             * @brief Get a texture from its name or index.
             * 
             * @param texture String containing a texture's name or index.
             * @param xml_textures XML entries of all the textures.
             * @return Texture_t* Pointer to the requested texture.
             */
            Texture_t* get_texture(std::string texture, const tinyxml2::XMLElement* xml_textures) const;

            /**
             * @brief Get the two indices of the materials making up a material mix from their names or indices.
             * 
             * @param material_refracted String containing the first material's name or index.
             * @param material_reflected String containing the second material's name or index.
             * @param xml_materials XML entries of all the materials.
             * @return std::vector<size_t> Vector of the indices ot the two materials making up a material mix.
             */
            std::vector<size_t> get_material_mix(std::string material_refracted, std::string material_reflected, const tinyxml2::XMLElement* xml_materials) const;

            /**
             * @brief Get a medium from its name or index.
             * 
             * @param medium String containing a medium's name or index.
             * @param xml_mediums XML entries of all the mediums.
             * @return Medium_t* Pointer to the requested medium.
             */
            Medium_t* get_medium(std::string medium, const tinyxml2::XMLElement* xml_mediums) const;

            /**
             * @brief Get a mesh geometry from its name or index.
             * 
             * @param mesh_geometry String containing a mesh geometry's name or index.
             * @param xml_mesh_geometries XML entries of all the mesh geometries.
             * @return MeshGeometry_t* Pointer to the requested mesh geometry.
             */
            MeshGeometry_t* get_mesh_geometry(std::string mesh_geometry, const tinyxml2::XMLElement* xml_mesh_geometries) const;

            /**
             * @brief Get a material's index from its name or index.
             * 
             * @param material String containing a material's name or index.
             * @param xml_materials XML entries of all the materials.
             * @return size_t Index of the requested material.
             */
            size_t get_material_index(std::string material, const tinyxml2::XMLElement* xml_materials) const;

            /**
             * @brief Get a material from its name or index.
             * 
             * @param material String containing a material's name or index.
             * @param xml_materials XML entries of all the materials.
             * @return Material_t* Pointer to the requested material.
             */
            Material_t* get_material(std::string material, const tinyxml2::XMLElement* xml_materials) const;

            /**
             * @brief Get a vector of directional lights from their name or index.
             * 
             * @param lights_string String containing light names and/or indices.
             * @param xml_directional_lights XML entries of all the directional lights.
             * @return std::vector<DirectionalLight_t*> Vector of the requested directional lights.
             */
            std::vector<DirectionalLight_t*> get_lights(std::string lights_string, const tinyxml2::XMLElement* xml_directional_lights) const;

            /**
             * @brief Get an image buffer from its name or index.
             * 
             * @param imgbuffer String containing an image buffer's name or index.
             * @param xml_imgbuffers XML entries of all the image buffers.
             * @return ImgBuffer_t* Pointer to the requested image buffer.
             */
            ImgBuffer_t* get_imgbuffer(std::string imgbuffer, const tinyxml2::XMLElement* xml_imgbuffers) const;

            /**
             * @brief Get a skybox from its name or index.
             * 
             * @param skybox String containing a skybox's name or index.
             * @param xml_skyboxes XML entries of all the skyboxes.
             * @return Skybox_t* Pointer to the requested skybox.
             */
            Skybox_t* get_skybox(std::string skybox, const tinyxml2::XMLElement* xml_skyboxes) const;

            /**
             * @brief Get the shapes and meshes making up the scene from their name or index.
             * 
             * This function selectively adds the created shapes and meshes to the scene, depending on if they are listed or not.
             * 
             * @param objects_string String containing the shapes and/or meshes name or index.
             * @param[out] shapes Vector containing the shapes making up the scene.
             * @param[out] meshes Vector containing the meshes making up the scene.
             * @param xml_objects XML entries of all the shapes and/or meshes.
             */
            void get_objects(std::string objects_string, std::vector<Shape_t*> &shapes, std::vector<MeshTop_t*> &meshes, const tinyxml2::XMLElement* xml_objects) const;

            /**
             * @brief Get the shapes and meshes making up the scene from all the created shapes and meshes.
             * 
             * This function adds all the created shapes and meshes to the scene.
             * 
             * @param[out] shapes Vector containing the shapes making up the scene.
             * @param[out] meshes Vector containing the meshes making up the scene.
             */
            void get_objects(std::vector<Shape_t*> &shapes, std::vector<MeshTop_t*> &meshes) const;
    };
}}

namespace APTracer {
    /**
     * @brief Get the colour encoded in a string. Can be one numerical value, three numerical values, or a colour name.
     * 
     * The colour can be a colour name, provided it is defined in the APTracer::Colours::colours map. It can also be
     * a single numerical value, in which case all three components will have the same value, or three values, one for
     * each component.
     * 
     * @param colour String containing a colour value(s) or name.
     * @return Vec3f Three component colour.
     */
    Vec3f get_colour(std::string colour); // copies string :(

    /**
     * @brief Get a vector of three points from a string of numerical values, or nan for nullptr.
     * 
     * @param points_string String containing the three components of the three points, or 'nan'.
     * @return std::vector<Vec3f> Vector containing the components of three points, or empty vector.
     */
    std::vector<Vec3f> get_points(std::string points_string);

    /**
     * @brief Get a vector of three 2D coordinates from a string of numerical values, or nan for nullptr.
     * 
     * @param texture_coordinates_string String containing the two components of the three coordinates, or 'nan'.
     * @return std::vector<double> Vector containing the components of three coordinates, or empty vector. [x0, y0, x1, y1, x2, y2]
     */
    std::vector<double> get_texture_coordinates(std::string texture_coordinates_string);    

    /**
     * @brief Get a list of strings representing material names or indices from a string of material names or indices.
     * 
     * @param string_medium_names String of material names or indices.
     * @return std::unique_ptr<std::list<std::string>> List of strings representing material names or indices
     */
    std::unique_ptr<std::list<std::string>> get_medium_names(std::string string_medium_names);

    /**
     * @brief Get an array of two double values from a string.
     * 
     * @param string_value String containing two values.
     * @param value Array of two double values
     */
    void get_xy(const std::string &string_value, double (&value)[2]);

    /**
     * @brief Get an array of two unsigned integer values from a string.
     * 
     * @param string_value String containing two values.
     * @param value Array of two unsigned integer values.
     */
    void get_xy(const std::string &string_value, unsigned int (&value)[2]);

    /**
     * @brief Returns true if the string represents an integer.
     * 
     * @param s String containint or not an integer.
     * @return true The string represents an integer.
     * @return false The string doesn't represent an integer.
     */
    bool is_number(const std::string& s);

    /**
     * @brief Applies a transformation represented by an XML element to a transformation matrix.
     * 
     * @param[in, out] transform_matrix Transformation matrix to which we apply the transformation.
     * @param transform XML element representing the transformation.
     */
    void apply_transformation(TransformMatrix_t* transform_matrix, const tinyxml2::XMLElement* transform);

    /**
     * @brief This function defines the parameters that have to be present in an XML element and throws an error if a parameter is missing.
     * 
     * @param element XML element to probe for parameters.
     * @param attributes Array of chars representing parameter names.
     * @param n Number of parameters to test.
     */
    void require_attributes(const tinyxml2::XMLElement* element, const char** attributes, unsigned int n);
}

#endif