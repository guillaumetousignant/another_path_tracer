#ifndef APTRACER_MESHTOP_T_H
#define APTRACER_MESHTOP_T_H

#include "entities/Ray_t.h"
#include "entities/Vec3f.h"
#include <array>
#include <vector>

namespace APTracer { namespace Entities {
    class Material_t;
    class TransformMatrix_t;
    class MeshGeometry_t;
    class MaterialMap_t;
    class Shape_t;
}}

using APTracer::Entities::Ray_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Material_t;
using APTracer::Entities::TransformMatrix_t;
using APTracer::Entities::MeshGeometry_t;
using APTracer::Entities::MaterialMap_t;
using APTracer::Entities::Shape_t;

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
             * @param geom c
             */
            MeshTop_t(Material_t *material, TransformMatrix_t *transform_matrix, MeshGeometry_t* geom);

            /**
             * @brief Destroy the MeshTop_t object, deleting its triangles.
             */
            virtual ~MeshTop_t();

            Material_t *material_; /**< @brief Dummy material, in case it is used as a shape. Defaults to the first material of a material map if present.*/
            TransformMatrix_t *transformation_; /**< @brief Transformation matrix used for the mesh and all triangles inside it.*/
            MeshGeometry_t* geom_; /**< @brief Mesh geometry used by the mesh and its triangles, which index into it.*/
            size_t n_tris_; /**< @brief Number of triangles held by the mesh.*/
            std::vector<Shape_t*> triangles_; /**< @brief Array of triangles held by the mesh.*/

            /**
             * @brief Updates all the triangles held by the mesh.
             */
            virtual void update();

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
            virtual void intersection(const Ray_t &ray, bool &intersected, double &t, std::array<double, 2> &uv) const; // Never use this

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
            virtual void normaluv(const Ray_t &ray, std::array<double, 2> uv, std::array<double, 2> &tuv, Vec3f &normalvec) const;

            /**
             * @brief Returns the surface normal at a point in object coordinates. Placeholder
             * 
             * Do not use, does nothing.
             * 
             * @param ray Ray to calculate normal. Used for time.
             * @param uv Texture coordinates at the specified coordinates and time.
             * @param[out] normalvec Normal vector at the specified coordinates and time.
             */
            virtual void normal(const Ray_t &ray, std::array<double, 2> uv, Vec3f &normalvec) const;

            /**
             * @brief Minimum coordinates of an axis-aligned bounding box around the mesh.
             * 
             * This is used by acceleration structures to spatially sort shapes. Equivalent to the minimum coordinate of the 
             * mesh for each axis.
             * 
             * @return Vec3f Minimum coordinates of an axis-aligned bounding box around the mesh.
             */
            virtual Vec3f mincoord() const;

            /**
             * @brief Maximum coordinates of an axis-aligned bounding box around the mesh.
             * 
             * This is used by acceleration structures to spatially sort shapes. Equivalent to the maximum coordinate of the 
             * mesh for each axis.
             * 
             * @return Vec3f Maximum coordinates of an axis-aligned bounding box around the shape.
             */
            virtual Vec3f maxcoord() const;

            /**
             * @brief Virtual function in charge of creating the triangles for the different mesh types.
             * 
             * This is so regular meshes can use regular triangles, and motionblur meshes can use motionblur triangles. Called after the constructor.
             * This function will assign the mesh material to all triangles.
             */
            virtual void createTriangles() = 0;

            /**
             * @brief Virtual function in charge of creating the triangles for the different mesh types, using a material map.
             * 
             * This is so regular meshes can use regular triangles, and motionblur meshes can use motionblur triangles. Called after the constructor.
             * This function will assign the mesh material to all triangles. This version uses a material map to assign different materials to the 
             * triangles according to their material name in the mesh geometry.
             * 
             * @param materialmap Material map used to assign materials to the triangles according to their material name in the mesh geometry.
             */
            virtual void createTriangles(MaterialMap_t *materialmap) = 0;
    };
}}
#endif