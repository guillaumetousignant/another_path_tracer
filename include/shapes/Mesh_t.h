#ifndef MESH_T_H
#define MESH_T_H

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
             * @brief Destroy the Mesh_t object, deleting its triangles.
             */
            virtual ~Mesh_t() final;

            /**
             * @brief Create a Triangles object
             * 
             */
            virtual void createTriangles() final;
            virtual void createTriangles(MaterialMap_t *materialmap) final;
    };
}}
#endif