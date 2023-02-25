#ifndef APTRACER_ENTITIES_SCENECONTEXT_T_HPP
#define APTRACER_ENTITIES_SCENECONTEXT_T_HPP

#include "entities/Camera_t.hpp"
#include "entities/DirectionalLight_t.hpp"
#include "entities/ImgBuffer_t.hpp"
#include "entities/ImgBufferOpenGL_t.hpp"
#include "entities/Material_t.hpp"
#include "entities/MaterialMap_t.hpp"
#include "entities/Medium_t.hpp"
#include "entities/MeshGeometry_t.hpp"
#include "entities/OpenGLRenderer_t.hpp"
#include "entities/Scene_t.hpp"
#include "entities/Shape_t.hpp"
#include "entities/Skybox_t.hpp"
#include "entities/Texture_t.hpp"
#include "entities/TransformMatrix_t.hpp"
#include "entities/Vec3f.hpp"
#include "functions/tinyxml2.h"
#include "materials/FresnelMix_t.hpp"
#include "materials/FresnelMixIn_t.hpp"
#include "materials/FresnelMixNormal_t.hpp"
#include "materials/Portal_t.hpp"
#include "materials/PortalScatterer_t.hpp"
#include "materials/RandomMix_t.hpp"
#include "materials/RandomMixIn_t.hpp"
#include "materials/TextureMix_t.hpp"
#include "materials/TextureMixIn_t.hpp"
#include "shapes/MeshTop_t.hpp"
#include <array>
#include <iostream>
#include <list>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace APTracer { namespace Entities {

    /**
     * @brief The scene context class contains all data required to draw a static scene. It can read xml scene files representing many objects.
     *
     * The scene context holds arrays of every object needed to render a static scene. It uses smart pointers everywhere to simplify memory management.
     * The scene context can be initialised by hand, or can read from xml scene files. Several of those files are included with the project, in the
     * scenes folder. Refer to them for format.
     */
    class SceneContext_t {
        public:
            /**
             * @brief Construct a new empty SceneContext_t object.
             */
            SceneContext_t();

            /**
             * @brief Stores the lists of materials mixes and the indexes of their materials.
             *
             * This is needed because two passes have to be made for those materials, as the
             * materials they reference are not necessarily created yet, so their pointers
             * can't be stored in the material mix.
             */
            struct MaterialMixLists {
                    /** @brief List containing fresnel mixes and their materials.*/
                    std::list<std::pair<APTracer::Materials::FresnelMix_t*, std::array<size_t, 2>>> fresnel_mix;
                    /** @brief List containing fresnel mixes in and their materials.*/
                    std::list<std::pair<APTracer::Materials::FresnelMixIn_t*, std::array<size_t, 2>>> fresnel_mix_in;
                    /** @brief List containing normal-mapped fresnel mixes and their materials.*/
                    std::list<std::pair<APTracer::Materials::FresnelMixNormal_t*, std::array<size_t, 2>>> fresnel_mix_normal;
                    /** @brief List containing random mixes and their materials.*/
                    std::list<std::pair<APTracer::Materials::RandomMix_t*, std::array<size_t, 2>>> random_mix;
                    /** @brief List containing random mixes in and their materials.*/
                    std::list<std::pair<APTracer::Materials::RandomMixIn_t*, std::array<size_t, 2>>> random_mix_in;
                    /** @brief List containing texture mixes and their materials.*/
                    std::list<std::pair<APTracer::Materials::TextureMix_t*, std::array<size_t, 2>>> texture_mix;
                    /** @brief List containing texture mixes in and their materials.*/
                    std::list<std::pair<APTracer::Materials::TextureMixIn_t*, std::array<size_t, 2>>> texture_mix_in;
            };

            /**
             * @brief Stores the lists of transformable materials and their transformations.
             *
             * This is needed because the materials have to be transformed after their creation.
             */
            struct MaterialTransformations {
                    /** @brief List containing portal materials and their pre-update transformations.*/
                    std::list<std::pair<APTracer::Materials::Portal_t*, const tinyxml2::XMLElement*>> transformations_pre;
                    /** @brief List containing portal materials and their post-update transformations.*/
                    std::list<std::pair<APTracer::Materials::Portal_t*, const tinyxml2::XMLElement*>> transformations_post;
            };

            /**
             * @brief Stores the lists of transformable mediums and their transformations.
             *
             * This is needed because the mediums have to be transformed after their creation.
             */
            struct MediumTransformations {
                    /** @brief List containing portal scatterers and their pre-update transformations.*/
                    std::list<std::pair<APTracer::Materials::PortalScatterer_t*, const tinyxml2::XMLElement*>> transformations_pre;
                    /** @brief List containing portal scatterers and their post-update transformations.*/
                    std::list<std::pair<APTracer::Materials::PortalScatterer_t*, const tinyxml2::XMLElement*>> transformations_post;
            };

            bool use_gl_; /**< @brief True if the scenes will be shown on the screen with OpenGL.*/
            std::string scene_name_; /**< @brief Name of the scene. Default image filename.*/
            std::unique_ptr<OpenGLRenderer_t> opengl_renderer_; /**< @brief OpenGL renderer object that will render the scene, if enabled.*/
            ImgBufferOpenGL_t* opengl_imgbuffer_; /**< @brief Pointer to the image buffer to be used by OpenGL. Does not own the object.*/ // weak ptr
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
            std::vector<std::unique_ptr<Shapes::MeshTop_t>> meshes_; /**< @brief Vector containing the meshes.*/

            std::unique_ptr<TransformMatrix_t> invalid_transform_matrix_; /**< @brief Transformation matrix used when a reference is not found.*/
            std::unique_ptr<Medium_t> invalid_medium_; /**< @brief Medium used when a reference is not found.*/

            /**
             * @brief Reads a scene xml file, initialising the scene context from it.
             *
             * Example files are available in the scenes folder.
             *
             * @param filename Path to the scene file to be read, in xml format.
             */
            auto readXML(const std::string& filename) -> void;

            /**
             * @brief Starts rendering of all the scene's cameras, according to their mode.
             */
            auto render() -> void;

        private:
            /**
             * @brief Set the scene context to empty, so it can be filled again or deleted.
             */
            auto reset() -> void;

            /**
             * @brief Create a transformation matrix object from an xml entry.
             *
             * @param xml_transform_matrix XML entry containing the transformation matrix information.
             * @return std::unique_ptr<TransformMatrix_t> Unique pointer to the created transformation matrix.
             */
            static auto create_transform_matrix(const tinyxml2::XMLElement* xml_transform_matrix) -> std::unique_ptr<TransformMatrix_t>;

            /**
             * @brief Create a texture object from an xml entry.
             *
             * @param xml_texture XML entry containing the texture information.
             * @return std::unique_ptr<Texture_t> Unique pointer to the created texture.
             */
            static auto create_texture(const tinyxml2::XMLElement* xml_texture) -> std::unique_ptr<Texture_t>;

            /**
             * @brief Create a medium object from an xml entry.
             *
             * @param xml_medium XML entry containing the medium information.
             * @param[out] mediums_medium_list A list of mediums in which the current medium is placed and a pointer to the medium is appended to this if needed.
             * @param[out] medium_transformations Structure containing lists of transformable medium pointers and their transformations.
             * @param xml_transform_matrices XML entries of all the transformation matrices.
             * @param xml_mediums XML entries of all the mediums.
             * @return std::unique_ptr<Medium_t> Unique pointer to the created medium.
             */
            auto create_medium(const tinyxml2::XMLElement* xml_medium,
                               std::list<std::pair<APTracer::Materials::PortalScatterer_t*, std::list<size_t>>>& mediums_medium_list,
                               MediumTransformations& medium_transformations,
                               const tinyxml2::XMLElement* xml_transform_matrices,
                               const tinyxml2::XMLElement* xml_mediums) -> std::unique_ptr<Medium_t>;

            /**
             * @brief Create a material object from an xml entry.
             *
             * @param xml_material XML entry containing the material information.
             * @param[out] materials_medium_list A list of mediums in which the current material is placed and a pointer to the material is appended to this­ if needed.
             * @param[out] material_mix_lists Structure containing lists of material mix pointers and the index oh their materials.
             * @param[out] materials_aggregate_list Tuple of a list of material indices and a list of material names making up a material aggregate, if needed.
             * @param[out] material_transformations Structure containing lists of transformable material pointers and their transformations.
             * @param xml_textures XML entries of all the textures.
             * @param xml_transform_matrices XML entries of all the transformation matrices.
             * @param xml_materials XML entries of all the materials.
             * @param xml_mediums XML entries of all the mediums.
             * @return std::unique_ptr<Material_t> Unique pointer to the created material.
             */
            auto create_material(const tinyxml2::XMLElement* xml_material,
                                 std::list<std::pair<APTracer::Materials::Portal_t*, std::list<size_t>>>& materials_medium_list,
                                 MaterialMixLists& material_mix_lists,
                                 std::unique_ptr<std::tuple<std::unique_ptr<std::list<size_t>>, std::unique_ptr<std::list<std::string>>>>& materials_aggregate_list,
                                 MaterialTransformations& material_transformations,
                                 const tinyxml2::XMLElement* xml_textures,
                                 const tinyxml2::XMLElement* xml_transform_matrices,
                                 const tinyxml2::XMLElement* xml_materials,
                                 const tinyxml2::XMLElement* xml_mediums) -> std::unique_ptr<Material_t>;

            /**
             * @brief Create a mesh geometry object from an xml entry.
             *
             * @param xml_mesh_geometry XML entry containing the mesh geometry information.
             * @return std::unique_ptr<MeshGeometry_t> Unique pointer to the created mesh geometry.
             */
            static auto create_mesh_geometry(const tinyxml2::XMLElement* xml_mesh_geometry) -> std::unique_ptr<MeshGeometry_t>;

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
            auto create_object(const tinyxml2::XMLElement* xml_object,
                               std::unique_ptr<Shapes::MeshTop_t>& mesh,
                               const tinyxml2::XMLElement* xml_transform_matrices,
                               const tinyxml2::XMLElement* xml_materials,
                               const tinyxml2::XMLElement* xml_mesh_geometries) -> std::unique_ptr<Shape_t>;

            /**
             * @brief Create a directional light object from an xml entry.
             *
             * @param xml_directional_light XML entry containing the directional light information.
             * @param xml_transform_matrices XML entries of all the transformation matrices.
             * @return std::unique_ptr<DirectionalLight_t> Unique pointer to the created directional light.
             */
            auto create_directional_light(const tinyxml2::XMLElement* xml_directional_light, const tinyxml2::XMLElement* xml_transform_matrices) -> std::unique_ptr<DirectionalLight_t>;

            /**
             * @brief Create a skybox object from an xml entry.
             *
             * @param xml_skybox XML entry containing the skybox information.
             * @param xml_textures XML entries of all the textures.
             * @param xml_transform_matrices XML entries of all the transformation matrices.
             * @param xml_directional_lights XML entries of all the directional lights.
             * @return std::unique_ptr<Skybox_t> Unique pointer to the created skybox.
             */
            auto create_skybox(const tinyxml2::XMLElement* xml_skybox,
                               const tinyxml2::XMLElement* xml_textures,
                               const tinyxml2::XMLElement* xml_transform_matrices,
                               const tinyxml2::XMLElement* xml_directional_lights) -> std::unique_ptr<Skybox_t>;

            /**
             * @brief Create a image buffer object from an xml entry.
             *
             * @param xml_imgbuffer XML entry containing the image buffer information.
             * @return std::unique_ptr<ImgBuffer_t> Unique pointer to the created image buffer.
             */
            auto create_imgbuffer(const tinyxml2::XMLElement* xml_imgbuffer) -> std::unique_ptr<ImgBuffer_t>;

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
            auto create_camera(const tinyxml2::XMLElement* xml_camera,
                               const std::string& next_filename,
                               const tinyxml2::XMLElement* xml_transform_matrices,
                               const tinyxml2::XMLElement* xml_mediums,
                               const tinyxml2::XMLElement* xml_imgbuffers,
                               const tinyxml2::XMLElement* xml_skyboxes) -> std::unique_ptr<Camera_t>;

            /**
             * @brief Create a acceleration structure object from an xml entry and bind it to the scene.
             *
             * @param xml_acceleration_structure XML entry containing the acceleration structure information.
             */
            auto create_acceleration_structure(const tinyxml2::XMLElement* xml_acceleration_structure) const -> void;

            /**
             * @brief Get a transform matrix object from its name or index.
             *
             * @param transform_matrix String containing a transformation matrix name or index.
             * @param xml_transform_matrices XML entries of all the transformation matrices.
             * @return TransformMatrix_t* Pointer to the requested transformation matrix.
             */
            auto get_transform_matrix(std::string transform_matrix, const tinyxml2::XMLElement* xml_transform_matrices) -> TransformMatrix_t*;

            /**
             * @brief Get a list of material indices from a string of names of indices.
             *
             * @param string_material_list String containing material names and/or indices.
             * @param xml_materials XML entries of all the materials.
             * @return std::unique_ptr<std::list<size_t>> List of material indices.
             */
            static auto get_material_index_list(std::string string_material_list, const tinyxml2::XMLElement* xml_materials) -> std::unique_ptr<std::list<size_t>>;

            /**
             * @brief Get a list of medium indices from a string of names of indices.
             *
             * @param string_medium_list String containing medium names and/or indices.
             * @param xml_mediums XML entries of all the mediums.
             * @return std::unique_ptr<std::list<size_t>> List of medium indices.
             */
            static auto get_medium_index_list(std::string string_medium_list, const tinyxml2::XMLElement* xml_mediums) -> std::list<size_t>;

            /**
             * @brief Get a list of mediums from a string of names of indices.
             *
             * @param string_medium_list String containing medium names and/or indices.
             * @param xml_mediums XML entries of all the mediums.
             * @return std::list<Medium_t*> List of mediums.
             */
            auto get_medium_list(std::string string_medium_list, const tinyxml2::XMLElement* xml_mediums) const -> std::list<Medium_t*>;

            /**
             * @brief Get a texture from its name or index.
             *
             * @param texture String containing a texture's name or index.
             * @param xml_textures XML entries of all the textures.
             * @return Texture_t* Pointer to the requested texture.
             */
            auto get_texture(std::string texture, const tinyxml2::XMLElement* xml_textures) const -> Texture_t*;

            /**
             * @brief Get the two indices of the materials making up a material mix from their names or indices.
             *
             * @param material_refracted String containing the first material's name or index.
             * @param material_reflected String containing the second material's name or index.
             * @param xml_materials XML entries of all the materials.
             * @return std::array<size_t, 2> Array of the indices ot the two materials making up a material mix.
             */
            static auto get_material_mix(std::string material_refracted, std::string material_reflected, const tinyxml2::XMLElement* xml_materials) -> std::array<size_t, 2>;

            /**
             * @brief Get a medium from its name or index.
             *
             * @param medium String containing a medium's name or index.
             * @param xml_mediums XML entries of all the mediums.
             * @return Medium_t* Pointer to the requested medium.
             */
            auto get_medium(std::string medium, const tinyxml2::XMLElement* xml_mediums) -> Medium_t*;

            /**
             * @brief Get a mesh geometry from its name or index.
             *
             * @param mesh_geometry String containing a mesh geometry's name or index.
             * @param xml_mesh_geometries XML entries of all the mesh geometries.
             * @return MeshGeometry_t* Pointer to the requested mesh geometry.
             */
            auto get_mesh_geometry(std::string mesh_geometry, const tinyxml2::XMLElement* xml_mesh_geometries) const -> MeshGeometry_t*;

            /**
             * @brief Get a material's index from its name or index.
             *
             * @param material String containing a material's name or index.
             * @param xml_materials XML entries of all the materials.
             * @return size_t Index of the requested material.
             */
            static auto get_material_index(std::string material, const tinyxml2::XMLElement* xml_materials) -> size_t;

            /**
             * @brief Get a material from its name or index.
             *
             * @param material String containing a material's name or index.
             * @param xml_materials XML entries of all the materials.
             * @return Material_t* Pointer to the requested material.
             */
            auto get_material(std::string material, const tinyxml2::XMLElement* xml_materials) const -> Material_t*;

            /**
             * @brief Get a vector of directional lights from their name or index.
             *
             * @param lights_string String containing light names and/or indices.
             * @param xml_directional_lights XML entries of all the directional lights.
             * @return std::vector<DirectionalLight_t*> Vector of the requested directional lights.
             */
            auto get_lights(std::string lights_string, const tinyxml2::XMLElement* xml_directional_lights) const -> std::vector<DirectionalLight_t*>;

            /**
             * @brief Get an image buffer from its name or index.
             *
             * @param imgbuffer String containing an image buffer's name or index.
             * @param xml_imgbuffers XML entries of all the image buffers.
             * @return ImgBuffer_t* Pointer to the requested image buffer.
             */
            auto get_imgbuffer(std::string imgbuffer, const tinyxml2::XMLElement* xml_imgbuffers) const -> ImgBuffer_t*;

            /**
             * @brief Get a skybox from its name or index.
             *
             * @param skybox String containing a skybox's name or index.
             * @param xml_skyboxes XML entries of all the skyboxes.
             * @return Skybox_t* Pointer to the requested skybox.
             */
            auto get_skybox(std::string skybox, const tinyxml2::XMLElement* xml_skyboxes) const -> Skybox_t*;

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
            auto get_objects(std::string objects_string, std::vector<Shape_t*>& shapes, std::vector<Shapes::MeshTop_t*>& meshes, const tinyxml2::XMLElement* xml_objects) const -> void;

            /**
             * @brief Get the shapes and meshes making up the scene from all the created shapes and meshes.
             *
             * This function adds all the created shapes and meshes to the scene.
             *
             * @param[out] shapes Vector containing the shapes making up the scene.
             * @param[out] meshes Vector containing the meshes making up the scene.
             */
            auto get_objects(std::vector<Shape_t*>& shapes, std::vector<Shapes::MeshTop_t*>& meshes) const -> void;
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
auto get_colour(std::string colour) -> Entities::Vec3f; // copies string :(

/**
 * @brief Get a vector of three points from a string of numerical values.
 *
 * @param points_string String containing the three components of the three points.
 * @return std::array<Vec3f, 3> Array containing the components of three points.
 */
auto get_points(std::string points_string) -> std::array<Entities::Vec3f, 3>;

/**
 * @brief Get a vector of three normals from a string of numerical values, or nan to generate them.
 *
 * @param normals_string String containing the three components of the three normals, or 'nan'.
 * @param points Coordinates of the three points making up the triangle, to generate normals.
 * @return std::array<Vec3f, 3> Array containing the components of three normals.
 */
auto get_normals(std::string normals_string, const std::array<Entities::Vec3f, 3>& points) -> std::array<Entities::Vec3f, 3>;

/**
 * @brief Get a vector of three 2D coordinates from a string of numerical values, or nan to generate them.
 *
 * @param texture_coordinates_string String containing the two components of the three coordinates, or 'nan'.
 * @return std::array<double, 6> Array containing the components of three coordinates. [x0, y0, x1, y1, x2, y2]
 */
auto get_texture_coordinates(std::string texture_coordinates_string) -> std::array<double, 6>;

/**
 * @brief Get a list of strings representing material names or indices from a string of material names or indices.
 *
 * @param string_medium_names String of material names or indices.
 * @return std::unique_ptr<std::list<std::string>> List of strings representing material names or indices
 */
auto get_medium_names(std::string string_medium_names) -> std::unique_ptr<std::list<std::string>>;

/**
 * @brief Get an array of two values from a string.
 *
 * @param string_value String containing two values.
 * @return std::array<T, 2> Array of two values of type T.
 */
template<typename T>
auto get_xy(const std::string& string_value) -> std::array<T, 2> {
    std::array<T, 2> value;
    std::stringstream ss(string_value);

    for (unsigned int i = 0; i < 2; ++i) {
        if (ss.rdbuf()->in_avail() == 0) {
            std::cerr << "Error: xy should be 2 values separated by spaces. Current number of values is " << i << ", string is '" << string_value << "'. Exiting." << std::endl;
            exit(3);
        }
        ss >> value[i];
    }

    if (ss.rdbuf()->in_avail() != 0) {
        std::cerr << "Warning: xy should be 2 values separated by spaces. String not empty after two values, string is '" << string_value << "'. Ignoring." << std::endl;
    }

    return value;
};

/**
 * @brief Returns true if the string represents an integer.
 *
 * @param str String containing or not an integer.
 * @return true The string represents an integer.
 * @return false The string doesn't represent an integer.
 */
auto is_number(const std::string& str) -> bool;

/**
 * @brief Applies a transformation represented by an XML element to a transformation matrix.
 *
 * @param[in, out] transform_matrix Transformation matrix to which we apply the transformation.
 * @param transform XML element representing the transformation.
 */
auto apply_transformation(Entities::TransformMatrix_t* transform_matrix, const tinyxml2::XMLElement* transform) -> void;

/**
 * @brief This function defines the parameters that have to be present in an XML element and throws an error if a parameter is missing.
 *
 * @param element XML element to probe for parameters.
 * @param attributes Array of char pointers representing parameter names.
 */
auto require_attributes(const tinyxml2::XMLElement* element, const std::vector<const char*>& attributes) -> void;
}

#endif