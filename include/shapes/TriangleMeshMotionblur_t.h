#ifndef APTRACER_TRIANGLEMESHMOTIONBLUR_T_H
#define APTRACER_TRIANGLEMESHMOTIONBLUR_T_H

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

            /**
             * @brief Updates the triangle's points from its mesh geometry and the mesh's transformation matrix.
             * 
             * Stores the previous state in the _last variables, so that state can be interpolated according to time.
             * The points are created from the mesh geometry using the triangle's index, and the transformation matrix.
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
            virtual Vec3f normaluv(double time, const double (&uv)[2], double (&tuv)[2]) const final;

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
            virtual Vec3f normal(double time, const double (&uv)[2]) const final;

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
            virtual Vec3f normal_uv_tangent(double time, const double (&uv)[2], double (&tuv)[2], Vec3f &tangentvec) const final;

            /**
             * @brief Returns the geometric surface normal of the triangle at a specific time, not the interpolated one from vertex normals.
             * 
             * Not used anywhere usually, used to debug normal interpolation.
             * 
             * @param time Time at which we want the normal, from 0 to 1 for previous and current states.
             * @return Vec3f Normal vector of the triangle at the specified time.
             */
            virtual Vec3f normal_face(double time) const final;

            /**
             * @brief Minimum coordinates of an axis-aligned bounding box around the triangle.
             * 
             * This is used by acceleration structures to spatially sort shapes. Returns the minimum of all
             * three points and previous points for all axes. This ensures the minimum coordinate over the time range
             * is returned.
             * 
             * @return Vec3f Minimum coordinates of an axis-aligned bounding box around the triangle.
             */
            virtual Vec3f mincoord() const final;

            /**
             * @brief Maximum coordinates of an axis-aligned bounding box around the triangle.
             * 
             * This is used by acceleration structures to spatially sort shapes. Returns the minimum of all
             * three points and previous points for all axes. This ensures the maximum coordinate over the time range
             * is returned.
             * 
             * @return Vec3f Maximum coordinates of an axis-aligned bounding box around the triangle.
             */
            virtual Vec3f maxcoord() const final;
    };
}}
#endif