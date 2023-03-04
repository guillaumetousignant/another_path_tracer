#ifndef APTRACER_SHAPES_MESHTOP_T_HPP
#define APTRACER_SHAPES_MESHTOP_T_HPP

#include "entities/Material_t.hpp"
#include "entities/MaterialMap_t.hpp"
#include "entities/MeshGeometry_t.hpp"
#include "entities/Ray_t.hpp"
#include "entities/Shape_t.hpp"
#include "entities/TransformMatrix_t.hpp"
#include "entities/Vec3f.hpp"
#include <array>
#include <memory>
#include <vector>

namespace APTracer { namespace Shapes {
    /**
     * @brief The mesh top class defines a container class for triangles.
     *
     * It is linked to a mesh geometry, containing the shapes used by the program representing the mesh. It has a transformation
     * matrix, used for all triangles. The triangles hold a reference to the geometry, and their index into the geometry.
     * Has a few methods in common with shapes, but should not be used as a shape as it reduces the speedup from the acceleration
     * structures. This is a virtual class, derived into regular and motionblur meshes.
     */
    class MeshTop_t {
        public:
            /**
             * @brief Construct a new MeshTop_t object with a specific transformation matrix, mesh geometry, and a dummy material.
             *
             * @param material Dummy material assigned to the mesh. First material in case of a material map.
             * @param transform_matrix Transformation matrix used for the mesh and all triangles inside it.
             * @param geom Mesh geometry used by the mesh and its triangles, which index into it.
             */
            MeshTop_t(Entities::Material_t* material, Entities::TransformMatrix_t* transform_matrix, Entities::MeshGeometry_t* geom);

            /**
             * @brief Destroy the MeshTop_t object.
             */
            virtual ~MeshTop_t() = default;

            /**
             * @brief Copy construct a new MeshTop_t object.
             *
             * @param other Object to copy.
             */
            MeshTop_t(const MeshTop_t& other) = default;

            /**
             * @brief Move construct a new MeshTop_t object.
             *
             * @param other Object to move.
             */
            MeshTop_t(MeshTop_t&& other) noexcept = default;

            /**
             * @brief Copy assignment.
             *
             * @param other Object to copy.
             * @return MeshTop_t& Reference to this object.
             */
            auto operator=(const MeshTop_t& other) -> MeshTop_t& = default;

            /**
             * @brief Move assignment.
             *
             * @param other Object to move.
             * @return MeshTop_t& Reference to this object.
             */
            auto operator=(MeshTop_t&& other) noexcept -> MeshTop_t& = default;

            Entities::Material_t* material_; /**< @brief Dummy material, in case it is used as a shape. Defaults to the first material of a material map if present.*/
            Entities::TransformMatrix_t* transformation_; /**< @brief Transformation matrix used for the mesh and all triangles inside it.*/
            Entities::MeshGeometry_t* geom_; /**< @brief Mesh geometry used by the mesh and its triangles, which index into it.*/
            std::vector<std::unique_ptr<Entities::Shape_t>> triangles_; /**< @brief Array of triangles held by the mesh.*/

            /**
             * @brief Updates all the triangles held by the mesh.
             */
            virtual auto update() -> void;

            /**
             * @brief Calculate the intersection between a ray and the mesh, and stores information about this intersection. Placeholder
             *
             * Do not use, does nothing.
             *
             * @param ray Ray to intersect with the mesh.
             * @param[out] intersected True if the ray intersects the mesh.
             * @param[out] t Distance from the ray origin to the intersection point.
             * @param[out] uv 2D coordinates of the intersection point in object space.
             */
            virtual auto intersection(const Entities::Ray_t& ray, bool& intersected, double& t, std::array<double, 2>& uv) const -> void; // Never use this

            /**
             * @brief Returns the surface normal and texture coordinates at a point in object coordinates. Placeholder
             *
             * Do not use, does nothing.
             *
             * @param ray Ray to calculate normal. Used for time.
             * @param uv 2D coordinates of the intersection point in object space.
             * @param[out] tuv Texture coordinates at the specified coordinates and time.
             * @param[out] normalvec Normal vector at the specified coordinates and time.
             */
            virtual auto normaluv(const Entities::Ray_t& ray, std::array<double, 2> uv, std::array<double, 2>& tuv, Entities::Vec3f& normalvec) const -> void;

            /**
             * @brief Returns the surface normal at a point in object coordinates. Placeholder
             *
             * Do not use, does nothing.
             *
             * @param ray Ray to calculate normal. Used for time.
             * @param uv Texture coordinates at the specified coordinates and time.
             * @param[out] normalvec Normal vector at the specified coordinates and time.
             */
            virtual auto normal(const Entities::Ray_t& ray, std::array<double, 2> uv, Entities::Vec3f& normalvec) const -> void;

            /**
             * @brief Minimum coordinates of an axis-aligned bounding box around the mesh.
             *
             * This is used by acceleration structures to spatially sort shapes. Equivalent to the minimum coordinate of the
             * mesh for each axis.
             *
             * @return Vec3f Minimum coordinates of an axis-aligned bounding box around the mesh.
             */
            virtual auto mincoord() const -> Entities::Vec3f;

            /**
             * @brief Maximum coordinates of an axis-aligned bounding box around the mesh.
             *
             * This is used by acceleration structures to spatially sort shapes. Equivalent to the maximum coordinate of the
             * mesh for each axis.
             *
             * @return Vec3f Maximum coordinates of an axis-aligned bounding box around the shape.
             */
            virtual auto maxcoord() const -> Entities::Vec3f;

            /**
             * @brief Virtual function in charge of creating the triangles for the different mesh types.
             *
             * This is so regular meshes can use regular triangles, and motionblur meshes can use motionblur triangles. Called after the constructor.
             * This function will assign the mesh material to all triangles.
             */
            virtual auto createTriangles() -> void = 0;

            /**
             * @brief Virtual function in charge of creating the triangles for the different mesh types, using a material map.
             *
             * This is so regular meshes can use regular triangles, and motionblur meshes can use motionblur triangles. Called after the constructor.
             * This function will assign the mesh material to all triangles. This version uses a material map to assign different materials to the
             * triangles according to their material name in the mesh geometry.
             *
             * @param materialmap Material map used to assign materials to the triangles according to their material name in the mesh geometry.
             */
            virtual auto createTriangles(Entities::MaterialMap_t* materialmap) -> void = 0;
    };
}}
#endif
