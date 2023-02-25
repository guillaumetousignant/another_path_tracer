#ifndef APTRACER_SHAPES_TRIANGLEMESH_T_HPP
#define APTRACER_SHAPES_TRIANGLEMESH_T_HPP

#include "entities/MeshGeometry_t.hpp"
#include "entities/Shape_t.hpp"
#include <array>

namespace APTracer { namespace Shapes {
    /**
     * @brief The triangle mesh class defines a triangle shape that can be intersected by rays and is part of a mesh.
     *
     * A triangle is defined by three points, in counter-clockwise order. Its transformation matrix is used
     * to modify those points. These triangles are part of a mesh, and are defined by their index within this
     * mesh and the mesh's transformation matrix. They hold a reference to their mesh geometry and their index into it.
     * They use that data to populate their points.
     */
    class TriangleMesh_t final : public Entities::Shape_t {
        public:
            /**
             * @brief Construct a new TriangleMesh_t object with a material from a mesh geometry, an index within it, and the mesh's transformation matrix.
             *
             * @param material Material of the triangle. Material that will be bounced on at intersection.
             * @param transform_matrix Transformation used by the triangle to modify its points, shared by all triangles from a mesh.
             * @param geom Mesh geometry containing the triangle.
             * @param index Index of the triangle within the mesh geometry.
             */
            TriangleMesh_t(Entities::Material_t* material, Entities::TransformMatrix_t* transform_matrix, Entities::MeshGeometry_t* geom, size_t index);

            Entities::MeshGeometry_t* geom_; /**< @brief Mesh geometry containing the triangle.*/
            size_t index_; /**< @brief Index of the triangle within the mesh geometry.*/
            std::array<double, 6> texture_coordinates_; /**< @brief Array of the three texture coordinates with two components of the triangle, in counter-clockwise order. Transformed by the transform
                                                           matrix on update to give texture coordinates. [x0, y0, x1, y1, x2, y2]*/
            std::array<Entities::Vec3f, 3> points_; /**< @brief Array of the three points of the triangle, in counter-clockwise order.*/
            std::array<Entities::Vec3f, 3> normals_; /**< @brief Array of the three  normals of the triangle, in counter-clockwise order.*/
            Entities::Vec3f v0v1_; /**< @brief Cached vector from point 0 to point 1. Used for intersection.*/
            Entities::Vec3f v0v2_; /**< @brief Cached vector from point 0 to point 2. Used for intersection.*/
            std::array<double, 2> tuv_to_world_; /**< @brief Matrix to change referential from texture coordinate space to world space. Used to compute tangent vector.*/
            Entities::Vec3f tangent_vec_; /**< @brief Tangent vector of the triangle in world space. Points to positive u in texture coordinates. Used for normal mapping.*/

            /**
             * @brief Updates the triangle's points from its mesh geometry and the mesh's transformation matrix.
             *
             * The points are created from the mesh geometry using the triangle's index, and the transformation matrix.
             */
            auto update() -> void final;

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
            auto intersection(const Entities::Ray_t& ray, double& t, std::array<double, 2>& uv) const -> bool final;

            /**
             * @brief Returns the surface normal and texture coordinates at a point in object coordinates.
             *
             * This is used to find the surface normal on ray bounce. Used by materials to determine ray colour.
             * The texture coordinates is also returned, used by materials to fetch a colour in a texture.
             * The time parameter is for motionblur shapes, where time is used to interpolate. Not used here.
             * The object coordinates are in barycentric coordinates (minus w) [u, v].
             *
             * @param[in] time Time at which we want the normal and texture coordinates. Not used here.
             * @param[in] uv Object coordinates at which we want to find the normal and texture coordinates. The coordinates are in barycentric coordinates, minus w [u, v].
             * @param[out] tuv Texture coordinates at the specified coordinates.
             * @return Vec3f Normal vector at the specified coordinates.
             */
            auto normaluv(double time, std::array<double, 2> uv, std::array<double, 2>& tuv) const -> Entities::Vec3f final;

            /**
             * @brief Returns the surface normal at a point in object coordinates.
             *
             * This is used to find the surface normal on ray bounce. Used by materials to determine ray colour.
             * The time parameter is for motionblur shapes, where time is used to interpolate. Not used here.
             * The object coordinates are in barycentric coordinates (minus w) [u, v].
             *
             * @param[in] time Time at which we want the normal. Used when motion blur is used. Not used here.
             * @param[in] uv Object coordinates at which we want to find the normal. The coordinates are in barycentric coordinates, minus w [u, v].
             * @return Vec3f Normal vector at the specified coordinates.
             */
            auto normal(double time, std::array<double, 2> uv) const -> Entities::Vec3f final;

            /**
             * @brief Returns the surface normal, texture coordinates and tangent vector at a point in object coordinates.
             *
             * This is used to find the surface normal on ray bounce. Used by materials to determine ray colour.
             * The texture coordinates is also returned, used by materials to fetch a colour in a texture. The tangent
             * vector is used by materials for normal mapping, as the normals returned by those textures are in object
             * coordinates. The time parameter is for motionblur shapes, where time is used to interpolate. Not used here.
             * The object coordinates are in barycentric coordinates (minus w) [u, v].
             *
             * @param[in] time Time at which we want the normal, tangent and texture coordinates. Used when motion blur is used. Not used here.
             * @param[in] uv Object coordinates at which we want to find the normal, texture coordinates and tangent vector. The coordinates are in barycentric coordinates, minus w [u, v].
             * @param[out] tuv Texture coordinates at the specified coordinates.
             * @param[out] tangentvec Tangent vector at the specified coordinates.
             * @return Vec3f Normal vector at the specified coordinates.
             */
            auto normal_uv_tangent(double time, std::array<double, 2> uv, std::array<double, 2>& tuv, Entities::Vec3f& tangentvec) const -> Entities::Vec3f final;

            /**
             * @brief Returns the geometric surface normal of the triangle, not the interpolated one from vertex normals.
             *
             * Not used anywhere usually, used to debug normal interpolation.
             *
             * @param time Time at which we want the normal. Used when motion blur is used. Not used here.
             * @return Vec3f Normal vector of the triangle.
             */
            auto normal_face(double time) const -> Entities::Vec3f;

            /**
             * @brief Minimum coordinates of an axis-aligned bounding box around the triangle.
             *
             * This is used by acceleration structures to spatially sort shapes. Returns the minimum of all
             * three points for all axes.
             *
             * @return Vec3f Minimum coordinates of an axis-aligned bounding box around the triangle.
             */
            auto mincoord() const -> Entities::Vec3f final;

            /**
             * @brief Maximum coordinates of an axis-aligned bounding box around the triangle.
             *
             * This is used by acceleration structures to spatially sort shapes. Returns the minimum of all
             * three points for all axes.
             *
             * @return Vec3f Maximum coordinates of an axis-aligned bounding box around the triangle.
             */
            auto maxcoord() const -> Entities::Vec3f final;
    };
}}
#endif