#ifndef TRIANGLEMESHMOTIONBLUR_T_H
#define TRIANGLEMESHMOTIONBLUR_T_H

#include "entities/Ray_t.h"
#include "entities/Vec3f.h"
#include "entities/Shape_t.h"

namespace APTracer { namespace Entities {
    class TransformMatrix_t;
    class Material_t;
    class MeshGeometry_t;
}}

using APTracer::Entities::Ray_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Material_t;
using APTracer::Entities::TransformMatrix_t;
using APTracer::Entities::Shape_t;
using APTracer::Entities::MeshGeometry_t;

namespace APTracer { namespace Shapes {

    /**
     * @brief The triangle mesh motionblur class defines a triangle shape that can be intersected by rays with motion blur enabled and is part of a mesh.
     * 
     * A triangle is defined by three points, in counter-clockwise order. Its transformation matrix is used 
     * to modify those points. These triangles are part of a mesh, and are defined by their index within this 
     * mesh and the mesh's transformation matrix. They hold a reference to their mesh geometry and their index into it.
     * They use that data to populate their points. This shape remembers its last state to enable motion blur. The state 
     * is interpolated between current and previous states according to a ray's time.
     */
    class TriangleMeshMotionblur_t final : public Shape_t{
        public:
            /**
             * @brief Construct a new TriangleMeshMotionblur_t object with a material from a mesh geometry, an index within it, and the mesh's transformation matrix.
             * 
             * @param material Material of the triangle. Material that will be bounced on at intersection.
             * @param transform_matrix 
             * @param geom 
             * @param index 
             */
            TriangleMeshMotionblur_t(Material_t *material, TransformMatrix_t *transform_matrix, MeshGeometry_t* geom, size_t index);

            /**
             * @brief Destroy the TriangleMeshMotionblur_t object. Does nothing.
             */
            virtual ~TriangleMeshMotionblur_t() final;

            MeshGeometry_t* geom_; /**< @brief Mesh geometry containing the triangle.*/
            size_t index_; /**< @brief Index of the triangle within the mesh geometry.*/
            double texture_coordinates_[6]; /**< @brief Array of the three texture coordinates with two components of the triangle, in counter-clockwise order. Transformed by the transform matrix on update to give texture coordinates. [x0, y0, x1, y1, x2, y2]*/
            Vec3f points_[3]; /**< @brief Array of the three points of the triangle, in counter-clockwise order.*/
            Vec3f normals_[3]; /**< @brief Array of the three  normals of the triangle, in counter-clockwise order.*/
            Vec3f v0v1_; /**< @brief Cached vector from point 0 to point 1. Used for intersection.*/
            Vec3f v0v2_; /**< @brief Cached vector from point 0 to point 2. Used for intersection.*/
            double tuv_to_world_[2]; /**< @brief Matrix to change referential from texture coordinate space to world space. Used to compute tangent vector.*/
            Vec3f tangent_vec_; /**< @brief Tangent vector of the triangle in world space. Points to positive u in texture coordinates. Used for normal mapping.*/
            double texture_coordinates_last_[6]; /**< @brief Array of the three texture coordinates with two components of the triangle before last update. Used for motion blur.*/
            Vec3f points_last_[3]; /**< @brief Array of the three points of the triangle before last update. Used for motion blur.*/
            Vec3f normals_last_[3]; /**< @brief Array of the three  normals of the triangle before last update. Used for motion blur.*/
            Vec3f v0v1_last_; /**< @brief Cached vector from point 0 to point 1 before last update. Used for motion blur.*/
            Vec3f v0v2_last_; /**< @brief Cached vector from point 0 to point 2 before last update. Used for motion blur.*/
            Vec3f tangent_vec_last_; /**< @brief Tangent vector of the triangle in world space before last update. Used for motion blur.*/

            virtual void update() final;
            virtual bool intersection(const Ray_t &rays, double &t, double (&uv)[2]) const final; 
            virtual Vec3f normaluv(double time, const double (&uv)[2], double (&tuv)[2]) const final;
            virtual Vec3f normal(double time, const double (&uv)[2]) const final;
            virtual Vec3f normal_uv_tangent(double time, const double (&uv)[2], double (&tuv)[2], Vec3f &tangentvec) const final;
            virtual Vec3f normal_face(double time) const final;
            virtual Vec3f mincoord() const final;
            virtual Vec3f maxcoord() const final;
    };
}}
#endif