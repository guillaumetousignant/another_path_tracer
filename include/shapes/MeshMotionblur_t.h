#ifndef APTRACER_MESHMOTIONBLUR_T_H
#define APTRACER_MESHMOTIONBLUR_T_H

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
     * @brief Motionblur mesh composed of motionblur triangles.
     * 
     * The mesh contains a reference to a mesh geometry, and an array of triangles representing the mesh. The triangles
     * take their data from the mesh geometry and their index within it. The transformation matrix is used for all triangles,
     * so modifying the transformation matrix transforms all triangles. This is the motionblur version.
     */
    class MeshMotionblur_t final : public MeshTop_t{
        public:
            /**
             * @brief Construct a new MeshMotionblur_t object with a single material.
             * 
             * @param material Material used for all triangles.
             * @param transform_matrix Transformation matrix used for the mesh and all triangles inside it.
             * @param geom Mesh geometry used by the mesh and its triangles, which index into it.
             */
            MeshMotionblur_t(Material_t *material, TransformMatrix_t *transform_matrix, MeshGeometry_t* geom);

            /**
             * @brief Construct a new MeshMotionblur_t object with a material map.
             * 
             * @param materialmap Material map used to assign materials to the triangles according to their material name in the mesh geometry.
             * @param transform_matrix Transformation matrix used for the mesh and all triangles inside it.
             * @param geom Mesh geometry used by the mesh and its triangles, which index into it.
             */
            MeshMotionblur_t(MaterialMap_t *materialmap, TransformMatrix_t *transform_matrix, MeshGeometry_t* geom);

            /**
             * @brief Destroy the MeshMotionblur_t object, deleting its triangles.
             */
            virtual ~MeshMotionblur_t() final;

            /**
             * @brief Copy constructor.
             * 
             * @param other Mesh to be copied.
             */
            MeshMotionblur_t(const MeshMotionblur_t& other);
        
            /**
             * @brief Move constructor.
             * 
             * @param other Mesh to be moved.
             */
            MeshMotionblur_t(MeshMotionblur_t&& other);
        
            /**
             * @brief Copy assignment.
             * 
             * @param other Mesh to be copied.
             * @return MeshMotionblur_t& Reference to this mesh.
             */
            MeshMotionblur_t& operator=(const MeshMotionblur_t& other);
        
            /**
             * @brief Move assignment.
             * 
             * @param other Mesh to be moved.
             * @return MeshMotionblur_t& Reference to this mesh.
             */
            MeshMotionblur_t& operator=(MeshMotionblur_t&& other);

            /**
             * @brief Creates the triangles of the mesh, with a reference to the mesh geometry and their index into it.
             * 
             * his creates motionblur triangles, and assigns them the mesh's material.
             */
            virtual void createTriangles() final;

            /**
             * @brief Creates the triangles of the mesh, with a reference to the mesh geometry and their index into it, using a material map.
             * 
             * This creates motionblur triangles, and uses a material map to assign different materials to the 
             * triangles according to their material name in the mesh geometry.
             * 
             * @param materialmap Material map used to assign materials to the triangles according to their material name in the mesh geometry.
             */
            virtual void createTriangles(MaterialMap_t *materialmap) final;
    };
}}
#endif