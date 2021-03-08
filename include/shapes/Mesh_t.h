#ifndef APTRACER_MESH_T_H
#define APTRACER_MESH_T_H

#include "entities/Ray_t.h"
#include "entities/Vec3f.h"
#include "shapes/MeshTop_t.h"

namespace APTracer { namespace Entities {
    class Material_t;
    class TransformMatrix_t;
    class MeshGeometry_t;
    class MaterialMap_t;
}}

using APTracer::Entities::Material_t;
using APTracer::Entities::TransformMatrix_t;
using APTracer::Entities::MeshGeometry_t;
using APTracer::Entities::MaterialMap_t;
using APTracer::Shapes::MeshTop_t;

namespace APTracer { namespace Shapes {
    /**
     * @brief Regular mesh composed of regular triangles.
     * 
     * The mesh contains a reference to a mesh geometry, and an array of triangles representing the mesh. The triangles
     * take their data from the mesh geometry and their index within it. The transformation matrix is used for all triangles,
     * so modifying the transformation matrix transforms all triangles.
     */
    class Mesh_t final : public MeshTop_t{
        public:
            /**
             * @brief Construct a new Mesh_t object with a single material.
             * 
             * @param material Material used for all triangles.
             * @param transform_matrix Transformation matrix used for the mesh and all triangles inside it.
             * @param geom Mesh geometry used by the mesh and its triangles, which index into it.
             */
            Mesh_t(Material_t *material, TransformMatrix_t *transform_matrix, MeshGeometry_t* geom);

            /**
             * @brief Construct a new Mesh_t object with a material map.
             * 
             * @param materialmap Material map used to assign materials to the triangles according to their material name in the mesh geometry.
             * @param transform_matrix Transformation matrix used for the mesh and all triangles inside it.
             * @param geom Mesh geometry used by the mesh and its triangles, which index into it.
             */
            Mesh_t(MaterialMap_t *materialmap, TransformMatrix_t *transform_matrix, MeshGeometry_t* geom);

            /**
             * @brief Copy constructor.
             * 
             * @param other Mesh to be copied.
             */
            Mesh_t(const Mesh_t& other);
        
            /**
             * @brief Move constructor.
             * 
             * @param other Mesh to be moved.
             */
            Mesh_t(Mesh_t&& other) noexcept;
        
            /**
             * @brief Copy assignment.
             * 
             * @param other Mesh to be copied.
             * @return Mesh_t& Reference to this mesh.
             */
            auto operator=(const Mesh_t& other) -> Mesh_t&;
        
            /**
             * @brief Move assignment.
             * 
             * @param other Mesh to be moved.
             * @return Mesh_t& Reference to this mesh.
             */
            auto operator=(Mesh_t&& other) noexcept -> Mesh_t&;

            /**
             * @brief Creates the triangles of the mesh, with a reference to the mesh geometry and their index into it.
             * 
             * This creates regular triangles, and assigns them the mesh's material.
             */
            virtual auto createTriangles() -> void final;

            /**
             * @brief Creates the triangles of the mesh, with a reference to the mesh geometry and their index into it, using a material map.
             * 
             * This creates regular triangles, and uses a material map to assign different materials to the 
             * triangles according to their material name in the mesh geometry.
             * 
             * @param materialmap Material map used to assign materials to the triangles according to their material name in the mesh geometry.
             */
            virtual auto createTriangles(MaterialMap_t *materialmap) -> void final;
    };
}}
#endif