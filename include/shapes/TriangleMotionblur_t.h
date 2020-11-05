#ifndef TRIANGLEMOTIONBLUR_T_H
#define TRIANGLEMOTIONBLUR_T_H

#include "entities/Ray_t.h"
#include "entities/Vec3f.h"
#include "entities/Shape_t.h"

namespace APTracer { namespace Entities {
    class TransformMatrix_t;
    class Material_t;
}}

using APTracer::Entities::Ray_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Material_t;
using APTracer::Entities::TransformMatrix_t;
using APTracer::Entities::Shape_t;

namespace APTracer { namespace Shapes {
    /**
     * @brief The triangle motionblur class defines a triangle shape that can be intersected by rays, with motion blur enabled.
     * 
     * A triangle is defined by three points, in counter-clockwise order. Its transformation matrix is used 
     * to modify those points. This shape remembers its last state to enable motion blur. The state is interpolated between current and previous
     * states according to a ray's time.
     */
    class TriangleMotionblur_t final : public Shape_t{
        public:
            /**
             * @brief Construct a new TriangleMotionblur_t object with a transformation matrix and material from three points.
             * 
             * @param material Material of the triangle. Material that will be bounced on at intersection.
             * @param transform_matrix Transformation used by the triangle to modify its points.
             * @param points Array of three points, in counter-clockwise order, defining the triangle.
             * @param normals Array of three normals, in counter-clockwise order, at the three points of the triangle.
             * @param texcoord Array of three texture coordinates with two components, in counter-clockwise order, at the three points of the triangle. [x0, y0, x1, y1, x2, y2]
             */
            TriangleMotionblur_t(Material_t *material, TransformMatrix_t *transform_matrix, const Vec3f* points, const Vec3f* normals, const double* texcoord);

            /**
             * @brief Destroy the TriangleMotionblur_t object. Does nothing.
             */
            virtual ~TriangleMotionblur_t() final;

            Vec3f points_orig_[3]; /**< @brief Array of the three un-transformed points of the triangle, in counter-clockwise order. Transformed by the transform matrix on update to give points.*/
            Vec3f normals_orig_[3]; /**< @brief Array of the three un-transformed normals of the triangle, in counter-clockwise order. Transformed by the transform matrix on update to give normals.*/
            double texture_coordinates_[6]; /**< @brief Array of the three texture coordinates with two components of the triangle, in counter-clockwise order. Transformed by the transform matrix on update to give texture coordinates. [x0, y0, x1, y1, x2, y2]*/
            double tuv_to_world_[2]; /**< @brief Matrix to change referential from texture coordinate space to world space. Used to compute tangent vector.*/
            Vec3f points_[3]; /**< @brief Array of the three points of the triangle, in counter-clockwise order.*/
            Vec3f normals_[3]; /**< @brief Array of the three  normals of the triangle, in counter-clockwise order.*/
            Vec3f v0v1_; /**< @brief Cached vector from point 0 to point 1. Used for intersection.*/
            Vec3f v0v2_; /**< @brief Cached vector from point 0 to point 2. Used for intersection.*/
            Vec3f tangent_vec_; /**< @brief Tangent vector of the triangle in world space. Points to positive u in texture coordinates. Used for normal mapping.*/
            Vec3f points_last_[3]; /**< @brief Array of the three points of the triangle before last update. Used for motion blur.*/
            Vec3f normals_last_[3]; /**< @brief Array of the three  normals of the triangle before last update. Used for motion blur.*/
            Vec3f v0v1_last_; /**< @brief Cached vector from point 0 to point 1 before last update. Used for motion blur.*/
            Vec3f v0v2_last_; /**< @brief Cached vector from point 0 to point 2 before last update. Used for motion blur.*/
            Vec3f tangent_vec_last_; /**< @brief Tangent vector of the triangle in world space before last update. Used for motion blur.*/

            /**
             * @brief Updates the triangle's points from its transformation matrix.
             * 
             * Stores the previous state in the _last variables, so that state can be interpolated according to time.
             * The points are created from the transformation matrix and the original points, stored in normals_orig_.
             */
            virtual void update() final;

            /**
             * @brief Intersects a ray with the triangle at a specific time, and stores information about the intersection.
             * 
             * This function returns wether a ray intersected the triangle or not, according to its direction, origin and time.
             * The intersection point, in object coordinates, is stored in uv, and the distance from the ray origin is stored
             * in t. uv is in barycentric coordinates, minus w [u, v]. Intersection is computed at ray.time, which 
             * interpolates between the previous and current state, with 0 being previous and 1 being current.
             * 
             * @param ray Ray to be tested for intersection with the triangle.
             * @param[out] t Distance at which the intersection ocurred, from ray origin. Undefined if not intersected.
             * @param[out] uv Coordinates in object space of the intersection. Undefined if not intersected. The coordinates are in barycentric coordinates, minus w [u, v].
             * @return true The ray intersected the triangle, t and uv are defined.
             * @return false The ray doesn't intersect the triangle, t and uv are undefined.
             */
            virtual bool intersection(const Ray_t &ray, double &t, double (&uv)[2]) const final; 

            /**
             * @brief 
             * 
             * @param time 
             * @param uv 
             * @param tuv 
             * @return Vec3f 
             */
            virtual Vec3f normaluv(double time, const double (&uv)[2], double (&tuv)[2]) const final;
            virtual Vec3f normal(double time, const double (&uv)[2]) const final;
            virtual Vec3f normal_uv_tangent(double time, const double (&uv)[2], double (&tuv)[2], Vec3f &tangentvec) const final;
            virtual Vec3f normal_face(double time) const final;
            virtual Vec3f mincoord() const final;
            virtual Vec3f maxcoord() const final;
    };
}}
#endif