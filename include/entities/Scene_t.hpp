#ifndef APTRACER_ENTITIES_SCENE_T_HPP
#define APTRACER_ENTITIES_SCENE_T_HPP

#include "entities/AccelerationStructure_t.hpp"
#include "entities/Ray_t.hpp"
#include "entities/Shape_t.hpp"
#include "entities/Skybox_t.hpp"
#include "shapes/MeshTop_t.hpp"
#include <array>
#include <memory>
#include <vector>

namespace APTracer { namespace Entities {

    /**
     * @brief The scene class holds the shapes representing a scene to be rendered, and creates an acceleration structure containing them.
     *
     * Holds an array of shapes to be intersected by rays. Those shapes are sorted in an acceleration structure owned by the scene for
     * faster intersection. The acceleration has to be built using build_acc before intersecting the scene. It can also be intersected
     * without the acceleration structure with intersect_brute, at a much slower pace when there are many shapes. Shapes can be added and
     * removed from the scene, but these operations are costly so should be batched.
     */
    class Scene_t {
        public:
            /**
             * @brief Construct a new empty Scene_t object.
             */
            Scene_t() = default;

            /**
             * @brief Construct a new Scene_t object containing a single shape.
             *
             * @param shape Shape to add to the scene.
             */
            explicit Scene_t(Shape_t* shape);

            /**
             * @brief Construct a new Scene_t object from multiple shapes.
             *
             * @param shapes Shapes to be added to the scene.
             */
            explicit Scene_t(std::vector<Shape_t*> shapes);

            /**
             * @brief Construct a new Scene_t object containing a single mesh.
             *
             * @param mesh Mesh to add to the scene.
             */
            explicit Scene_t(Shapes::MeshTop_t* mesh);

            /**
             * @brief Construct a new Scene_t object from multiple meshes.
             *
             * @param meshes Meshes to be added to the scene.
             */
            explicit Scene_t(const std::vector<Shapes::MeshTop_t*>& meshes);

            /**
             * @brief Destroy the Scene_t object.
             */
            ~Scene_t() = default;

            /**
             * @brief Copy construct a new Scene_t object.
             *
             * @param other Object to copy.
             */
            Scene_t(const Scene_t& other);

            /**
             * @brief Move construct a new Scene_t object.
             *
             * @param other Object to move.
             */
            Scene_t(Scene_t&& other) noexcept = default;

            /**
             * @brief Copy assignment.
             *
             * @param other Object to copy.
             * @return Scene_t& Reference to this object.
             */
            auto operator=(const Scene_t& other) -> Scene_t&;

            /**
             * @brief Move assignment.
             *
             * @param other Object to move.
             * @return Scene_t& Reference to this object.
             */
            auto operator=(Scene_t&& other) noexcept -> Scene_t& = default;

            std::vector<Shape_t*> geometry_; /**< @brief Vector of shapes to be drawn.*/
            std::unique_ptr<AccelerationStructure_t> acc_; /**< @brief Acceleration structure containing the shapes, used to accelerate intersection.*/

            /**
             * @brief Adds a single shape to the scene.
             *
             * @param shape Shape to be added to the scene.
             */
            auto add(Shape_t* shape) -> void;

            /**
             * @brief Adds several shapes to the scene.
             *
             * @param shapes Array of shapes to be added to the scene.
             */
            auto add(const std::vector<Shape_t*>& shapes) -> void;

            /**
             * @brief Adds a single mesh to the scene.
             *
             * @param mesh Mesh to be added to the scene.
             */
            auto add(Shapes::MeshTop_t* mesh) -> void;

            /**
             * @brief Adds multiple meshes to the scene.
             *
             * @param meshes Array of meshes to be added to the scene.
             */
            auto add(const std::vector<Shapes::MeshTop_t*>& meshes) -> void;

            /**
             * @brief Removes a single shape from the scene.
             *
             * @param shape Shape to be removed from the scene.
             */
            auto remove(Shape_t* shape) -> void;

            /**
             * @brief Removes multiple shapes from a scene.
             *
             * @param shapes Array of shapes to be removed from the scene.
             */
            auto remove(const std::vector<Shape_t*>& shapes) -> void;

            /**
             * @brief Removes a mesh from the scene.
             *
             * This works by finding the first shape pointer of the mesh and deleting it and the n_tris next shapes.
             * This works fine if the mesh is added with add and if the triangles of the mesh are not added individually
             * out of order.
             *
             * @param mesh Mesh to be removed from the scene.
             */
            auto remove(Shapes::MeshTop_t* mesh) -> void;

            /**
             * @brief Removes multiple meshes from the scene.
             *
             * This works by finding the first shape pointer of the meshes and deleting it and the n_tris next shapes.
             * This works fine if the meshes are added with add and if the triangles of the meshes are not added individually
             * out of order.
             *
             * @param meshes Array of meshes to be removed from the scene.
             */
            auto remove(const std::vector<Shapes::MeshTop_t*>& meshes) -> void;

            /**
             * @brief Updates all the shapes in the scene.
             *
             * Called to update all the shapes in the structure if their transformation matrix
             * has changed.
             */
            auto update() -> void;

            /**
             * @brief Builds an acceleration structure with the scene's shapes.
             *
             * The type of the acceleration structure will be AccelerationMultiGridVector_t, the highest performing
             * one for most (static) scenes.
             */
            auto build_acc() -> void;

            /**
             * @brief Intersects the scene shapes directly one by one. Not to be used for general operation.
             *
             * @param[in] ray Ray to be intersected with the scene, using its current origin and direction.
             * @param[out] t Distance to intersection. It is stored in t if there is an intersection.
             * @param[out] uv 2D object-space coordinates of the intersection.
             * @return Shape_t* Pointer of the intersected shape. Returns nullptr if there is no intersection.
             */
            auto intersect_brute(const Ray_t& ray, double& t, std::array<double, 2>& uv) const -> Shape_t*;

            /**
             * @brief Intersects the scene using the acceleration structure. Main way to intersect shapes.
             *
             * @param[in] ray Ray to be intersected with the scene, using its current origin and direction.
             * @param[out] t Distance to intersection. It is stored in t if there is an intersection.
             * @param[out] uv 2D object-space coordinates of the intersection.
             * @return Shape_t* Pointer of the intersected shape. Returns nullptr if there is no intersection.
             */
            auto intersect(const Ray_t& ray, double& t, std::array<double, 2>& uv) const -> Shape_t*;
    
            /**
             * @brief Intersects the ray with objects in the scene and bounces it on their material.
             *
             * This is the main function to compute the colour of a ray. A ray is sent through the scene,
             * finding the closest object hit. Then, the ray is modified by this object's material.
             * This change can change ray direction, origin, colour and mask. This process is repeated
             * up to max_bounces times, or until no object is it, at which point the skybox is intersected.
             * The ray is also modified by its first medium using the scatter function. If it is scattered,
             * it won't be bounced on the hit object's material, as it intersects the medium instead of
             * the object.
             *
             * @param ray Ray to intersect with the scene.
             * @param[in] max_bounces Upper bound of number of bounces. Number of bounces may be less if no object is hit or ray can't be illuminated anymore.
             * @param[in] skybox Skybox that will be intersected if no object is hit.
             */
            auto raycast(Ray_t& ray, unsigned int max_bounces, const Skybox_t* skybox) const -> void;

    };
}}

#endif