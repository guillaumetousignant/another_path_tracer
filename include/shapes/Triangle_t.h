#ifndef APTRACER_TRIANGLE_T_H
#define APTRACER_TRIANGLE_T_H

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
     * @brief The triangle class defines a triangle shape that can be intersected by rays.
     * 
     * A triangle is defined by three points, in counter-clockwise order. Its transformation matrix is used 
     * to modify those points.
     */
    class Triangle_t final : public Shape_t{
        public:
            /**
             * @brief Construct a new Triangle_t object with a transformation matrix and material from three points.
             * 
             * @param material Material of the triangle. Material that will be bounced on at intersection.
             * @param transform_matrix Transformation used by the triangle to modify its points.
             * @param points Array of three points, in counter-clockwise order, defining the triangle.
             * @param normals Array of three normals, in counter-clockwise order, at the three points of the triangle.
             * @param texcoord Array of three texture coordinates with two components, in counter-clockwise order, at the three points of the triangle. [x0, y0, x1, y1, x2, y2]
             */
            Triangle_t(Material_t *material, TransformMatrix_t *transform_matrix, const Vec3f* points, const Vec3f* normals, const double* texcoord);

            /**
             * @brief Destroy the Triangle_t object. Does nothing.
             */
            virtual ~Triangle_t() final;

            Vec3f points_orig_[3]; /**< @brief Array of the three un-transformed points of the triangle, in counter-clockwise order. Transformed by the transform matrix on update to give points.*/
            Vec3f normals_orig_[3]; /**< @brief Array of the three un-transformed normals of the triangle, in counter-clockwise order. Transformed by the transform matrix on update to give normals.*/
            double texture_coordinates_[6]; /**< @brief Array of the three texture coordinates with two components of the triangle, in counter-clockwise order. Transformed by the transform matrix on update to give texture coordinates. [x0, y0, x1, y1, x2, y2]*/
            Vec3f points_[3]; /**< @brief Array of the three points of the triangle, in counter-clockwise order.*/
            Vec3f normals_[3]; /**< @brief Array of the three  normals of the triangle, in counter-clockwise order.*/
            Vec3f v0v1_; /**< @brief Cached vector from point 0 to point 1. Used for intersection.*/
            Vec3f v0v2_; /**< @brief Cached vector from point 0 to point 2. Used for intersection.*/
            double tuv_to_world_[2]; /**< @brief Matrix to change referential from texture coordinate space to world space. Used to compute tangent vector.*/
            Vec3f tangent_vec_; /**< @brief Tangent vector of the triangle in world space. Points to positive u in texture coordinates. Used for normal mapping.*/

            /**
             * @brief Updates the triangle's points from its transformation matrix.
             * 
             * The points are created from the transformation matrix and the original points, stored in normals_orig_.
             */
            virtual void update() final;

            /**
             * @brief Intersects a ray with the triangle, and stores information about the intersection.
             * 
             * This function returns wether a ray intersected the triangle or not, according to its direction and origin.
             * The intersection point, in object coordinates, is stored in uv, and the distance from the ray origin is stored
             * in t. uv is in barycentric coordinates, minus w [u, v].
             * 
             * @param ray Ray to be tested for intersection with the triangle.
             * @param[out] t Distance at which the intersection ocurred, from ray origin. Undefined if not intersected.
             * @param[out] uv Coordinates in object space of the intersection. Undefined if not intersected. The coordinates are in barycentric coordinates, minus w [u, v].
             * @return true The ray intersected the triangle, t and uv are defined.
             * @return false The ray doesn't intersect the triangle, t and uv are undefined.
             */
            virtual bool intersection(const Ray_t &ray, double &t, std::array<double, 2> &uv) const final; 

            /**
             * @brief Returns the surface normal and texture coordinates at a point in object coordinates.
             * 
             * This is used to find the surface normal on ray bounce. Used by materials to determine ray colour.
             * The texture coordinates is also returned, used by materials to fetch a colour in a texture.
             * The time parameter is for motionblur shapes, where time is used to interpolate. Not used here.
             * The object coordinates are in barycentric cordinates (minus w) [u, v].
             * 
             * @param[in] time Time at which we want the normal and texture coordinates. Not used here.
             * @param[in] uv Object coordinates at which we want to find the normal and texture coordinates. The coordinates are in barycentric coordinates, minus w [u, v].
             * @param[out] tuv Texture coordinates at the specified coordinates.
             * @return Vec3f Normal vector at the specified coordinates.
             */
            virtual Vec3f normaluv(double time, std::array<double, 2> uv, std::array<double, 2> &tuv) const final;

            /**
             * @brief Returns the surface normal at a point in object coordinates.
             * 
             * This is used to find the surface normal on ray bounce. Used by materials to determine ray colour.
             * The time parameter is for motionblur shapes, where time is used to interpolate. Not used here.
             * The object coordinates are in barycentric cordinates (minus w) [u, v].
             * 
             * @param[in] time Time at which we want the normal. Used when motion blur is used. Not used here.
             * @param[in] uv Object coordinates at which we want to find the normal. The coordinates are in barycentric coordinates, minus w [u, v].
             * @return Vec3f Normal vector at the specified coordinates. 
             */
            virtual Vec3f normal(double time, std::array<double, 2> uv) const final;

            /**
             * @brief Returns the surface normal, texture coordinates and tangent vector at a point in object coordinates.
             * 
             * This is used to find the surface normal on ray bounce. Used by materials to determine ray colour.
             * The texture coordinates is also returned, used by materials to fetch a colour in a texture. The tangent
             * vector is used by materials for normal mapping, as the normals returned by those textures are in object
             * coordinates. The time parameter is for motionblur shapes, where time is used to interpolate. Not used here.
             * The object coordinates are in barycentric cordinates (minus w) [u, v].
             * 
             * @param[in] time Time at which we want the normal, tangent and texture coordinates. Used when motion blur is used. Not used here.
             * @param[in] uv Object coordinates at which we want to find the normal, texture coordinates and tangent vector. The coordinates are in barycentric coordinates, minus w [u, v].
             * @param[out] tuv Texture coordinates at the specified coordinates.
             * @param[out] tangentvec Tangent vector at the specified coordinates.
             * @return Vec3f Normal vector at the specified coordinates. 
             */
            virtual Vec3f normal_uv_tangent(double time, std::array<double, 2> uv, std::array<double, 2> &tuv, Vec3f &tangentvec) const final;

            /**
             * @brief Returns the geometric surface normal of the triangle, not the interpolated one from vertex normals.
             * 
             * Not used anywhere usually, used to debug normal interpolation.
             * 
             * @param time Time at which we want the normal. Used when motion blur is used. Not used here.
             * @return Vec3f Normal vector of the triangle.
             */
            virtual Vec3f normal_face(double time) const final;

            /**
             * @brief Minimum coordinates of an axis-aligned bounding box around the triangle.
             * 
             * This is used by acceleration structures to spatially sort shapes. Returns the minimum of all
             * three points for all axes.
             * 
             * @return Vec3f Minimum coordinates of an axis-aligned bounding box around the triangle.
             */
            virtual Vec3f mincoord() const final;

            /**
             * @brief Maximum coordinates of an axis-aligned bounding box around the triangle.
             * 
             * This is used by acceleration structures to spatially sort shapes. Returns the minimum of all
             * three points for all axes.
             * 
             * @return Vec3f Maximum coordinates of an axis-aligned bounding box around the triangle.
             */
            virtual Vec3f maxcoord() const final;
    };
}}
#endif