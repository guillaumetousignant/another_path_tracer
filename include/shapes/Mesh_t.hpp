#ifndef APTRACER_SHAPES_MESH_T_HPP
#define APTRACER_SHAPES_MESH_T_HPP

#include "shapes/MeshTop_t.hpp"

namespace APTracer { namespace Shapes {
    /**
     * @brief Regular mesh composed of regular triangles.
     *
     * The mesh contains a reference to a mesh geometry, and an array of triangles representing the mesh. The triangles
     * take their data from the mesh geometry and their index within it. The transformation matrix is used for all triangles,
     * so modifying the transformation matrix transforms all triangles.
     */
    class Mesh_t final : public MeshTop_t {
        public:
            /**
             * @brief Construct a new Mesh_t object with a single material.
             *
             * @param material Material used for all triangles.
             * @param transform_matrix Transformation matrix used for the mesh and all triangles inside it.
             * @param geom Mesh geometry used by the mesh and its triangles, which index into it.
             */
            Mesh_t(Entities::Material_t* material, Entities::TransformMatrix_t* transform_matrix, Entities::MeshGeometry_t* geom);

            /**
             * @brief Construct a new Mesh_t object with a material map.
             *
             * @param materialmap Material map used to assign materials to the triangles according to their material name in the mesh geometry.
             * @param transform_matrix Transformation matrix used for the mesh and all triangles inside it.
             * @param geom Mesh geometry used by the mesh and its triangles, which index into it.
             */
            Mesh_t(Entities::MaterialMap_t* materialmap, Entities::TransformMatrix_t* transform_matrix, Entities::MeshGeometry_t* geom);

            /**
             * @brief Creates the triangles of the mesh, with a reference to the mesh geometry and their index into it.
             *
             * This creates regular triangles, and assigns them the mesh's material.
             */
            auto createTriangles() -> void final;

            /**
             * @brief Creates the triangles of the mesh, with a reference to the mesh geometry and their index into it, using a material map.
             *
             * This creates regular triangles, and uses a material map to assign different materials to the
             * triangles according to their material name in the mesh geometry.
             *
             * @param materialmap Material map used to assign materials to the triangles according to their material name in the mesh geometry.
             */
            auto createTriangles(Entities::MaterialMap_t* materialmap) -> void final;
    };
}}
#endif