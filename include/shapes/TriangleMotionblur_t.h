#ifndef APTRACER_TRIANGLEMOTIONBLUR_T_H
#define APTRACER_TRIANGLEMOTIONBLUR_T_H

#include "entities/Ray_t.h"
#include "entities/Vec3f.h"
#include "entities/Shape_t.h"
#include <array>

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
            TriangleMotionblur_t(Material_t *material, TransformMatrix_t *transform_matrix, std::array<Vec3f, 3> points, const Vec3f* normals, const double* texcoord);

            std::array<Vec3f, 3> points_orig_; /**< @brief Array of the three un-transformed points of the triangle, in counter-clockwise order. Transformed by the transform matrix on update to give points.*/
            std::array<Vec3f, 3> normals_orig_; /**< @brief Array of the three un-transformed normals of the triangle, in counter-clockwise order. Transformed by the transform matrix on update to give normals.*/
            std::array<double, 6> texture_coordinates_; /**< @brief Array of the three texture coordinates with two components of the triangle, in counter-clockwise order. Transformed by the transform matrix on update to give texture coordinates. [x0, y0, x1, y1, x2, y2]*/
            std::array<Vec3f, 3> points_; /**< @brief Array of the three points of the triangle, in counter-clockwise order.*/
            std::array<Vec3f, 3> normals_; /**< @brief Array of the three  normals of the triangle, in counter-clockwise order.*/
            Vec3f v0v1_; /**< @brief Cached vector from point 0 to point 1. Used for intersection.*/
            Vec3f v0v2_; /**< @brief Cached vector from point 0 to point 2. Used for intersection.*/
            std::array<double, 2> tuv_to_world_; /**< @brief Matrix to change referential from texture coordinate space to world space. Used to compute tangent vector.*/
            Vec3f tangent_vec_; /**< @brief Tangent vector of the triangle in world space. Points to positive u in texture coordinates. Used for normal mapping.*/
            std::array<Vec3f, 3> points_last_; /**< @brief Array of the three points of the triangle before last update. Used for motion blur.*/
            std::array<Vec3f, 3> normals_last_; /**< @brief Array of the three  normals of the triangle before last update. Used for motion blur.*/
            Vec3f v0v1_last_; /**< @brief Cached vector from point 0 to point 1 before last update. Used for motion blur.*/
            Vec3f v0v2_last_; /**< @brief Cached vector from point 0 to point 2 before last update. Used for motion blur.*/
            Vec3f tangent_vec_last_; /**< @brief Tangent vector of the triangle in world space before last update. Used for motion blur.*/

            /**
             * @brief Updates the triangle's points from its transformation matrix.
             * 
             * Stores the previous state in the _last variables, so that state can be interpolated according to time.
             * The points are created from the transformation matrix and the original points, stored in normals_orig_.
             */
            virtual auto update() -> void final;

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
            virtual auto intersection(const Ray_t &ray, double &t, std::array<double, 2> &uv) const -> bool final; 

            /**
             * @brief Returns the surface normal and texture coordinates at a point in object coordinates and a specific time.
             * 
             * This is used to find the surface normal on ray bounce. Used by materials to determine ray colour.
             * The time parameter is used to interpolate between previous and current states, 0 and 1 respectively.
             * The object coordinates are in barycentric cordinates (minus w) [u, v].
             * 
             * @param[in] time Time at which we want the normal and texture coordinates, from 0 to 1 for previous and current states.
             * @param[in] uv Object coordinates at which we want to find the normal and texture coordinates. The coordinates are in barycentric coordinates, minus w [u, v].
             * @param[out] tuv Texture coordinates at the specified coordinates and time.
             * @return Vec3f Normal vector at the specified coordinates and time.
             */
            virtual auto normaluv(double time, std::array<double, 2> uv, std::array<double, 2> &tuv) const -> Vec3f final;

            /**
             * @brief Returns the surface normal at a point in object coordinates and a specific time.
             * 
             * This is used to find the surface normal on ray bounce. Used by materials to determine ray colour.
             * The time parameter is used to interpolate between previous and current states, 0 and 1 respectively.
             * The object coordinates are in barycentric cordinates (minus w) [u, v].
             * 
             * @param[in] time Time at which we want the normal, from 0 to 1 for previous and current states.
             * @param[in] uv Object coordinates at which we want to find the normal. The coordinates are in barycentric coordinates, minus w [u, v].
             * @return Vec3f Normal vector at the specified coordinates and time.
             */
            virtual auto normal(double time, std::array<double, 2> uv) const -> Vec3f final;

            /**
             * @brief Returns the surface normal, texture coordinates and tangent vector at a point in object coordinates and a specific time.
             * 
             * This is used to find the surface normal on ray bounce. Used by materials to determine ray colour.
             * The texture coordinates is also returned, used by materials to fetch a colour in a texture. The tangent
             * vector is used by materials for normal mapping, as the normals returned by those textures are in object
             * coordinates. The time parameter is used to interpolate between previous and current states, 0 and 1 respectively.
             * The object coordinates are in barycentric cordinates (minus w) [u, v].
             * 
             * @param[in] time Time at which we want the normal and texture coordinates, from 0 to 1 for previous and current states.
             * @param[in] uv Object coordinates at which we want to find the normal, texture coordinates and tangent vector. The coordinates are in barycentric coordinates, minus w [u, v].
             * @param[out] tuv Texture coordinates at the specified coordinates and time.
             * @param[out] tangentvec Tangent vector at the specified coordinates and time.
             * @return Vec3f Normal vector at the specified coordinates and time.
             */
            virtual auto normal_uv_tangent(double time, std::array<double, 2> uv, std::array<double, 2> &tuv, Vec3f &tangentvec) const -> Vec3f final;

            /**
             * @brief Returns the geometric surface normal of the triangle at a specific time, not the interpolated one from vertex normals.
             * 
             * Not used anywhere usually, used to debug normal interpolation.
             * 
             * @param time Time at which we want the normal, from 0 to 1 for previous and current states.
             * @return Vec3f Normal vector of the triangle at the specified time.
             */
            virtual auto normal_face(double time) const -> Vec3f final;

            /**
             * @brief Minimum coordinates of an axis-aligned bounding box around the triangle.
             * 
             * This is used by acceleration structures to spatially sort shapes. Returns the minimum of all
             * three points and previous points for all axes. This ensures the minimum coordinate over the time range
             * is returned.
             * 
             * @return Vec3f Minimum coordinates of an axis-aligned bounding box around the triangle.
             */
            virtual auto mincoord() const -> Vec3f final;

            /**
             * @brief Maximum coordinates of an axis-aligned bounding box around the triangle.
             * 
             * This is used by acceleration structures to spatially sort shapes. Returns the minimum of all
             * three points and previous points for all axes. This ensures the maximum coordinate over the time range
             * is returned.
             * 
             * @return Vec3f Maximum coordinates of an axis-aligned bounding box around the triangle.
             */
            virtual auto maxcoord() const -> Vec3f final;
    };
}}
#endif