#ifndef APTRACER_SHAPES_SPHERE_T_HPP
#define APTRACER_SHAPES_SPHERE_T_HPP

#include "entities/Shape_t.hpp"

namespace APTracer { namespace Shapes {
    /**
     * @brief The sphere class defines a sphere shape that can be intersected by rays.
     *
     * A sphere is defined by an origin and a radius. Its transformation matrix is used to modify those characteristics.
     */
    class Sphere_t final : public Entities::Shape_t {
        public:
            /**
             * @brief Construct a new Sphere_t object with a transformation matrix and material.
             *
             * @param material Material of the sphere. Material that will be bounced on at intersection.
             * @param transform_matrix Transformation used by the sphere to modify its size and position.
             */
            Sphere_t(Entities::Material_t* material, Entities::TransformMatrix_t* transform_matrix);

            double radius_; /**< @brief Radius of the sphere. Determined by the maximum scale of its transformation matrix.*/
            Entities::Vec3f origin_; /**< @brief Origin of the sphere. Position of its center.*/
            Entities::Vec3f direction_sph_; /**< @brief Rotation of the sphere, in spherical coordinates. Used for uv mapping.*/

            /**
             * @brief Updates the sphere's radius, origin and direction from its transformation matrix.
             *
             * The scale is determined by the maximum scale of its transformation matrix.
             */
            auto update() -> void final;

            /**
             * @brief Intersects a ray with the sphere, and stores information about the intersection.
             *
             * This function returns wether a ray intersected the sphere or not, according to its direction and origin.
             * The intersection point, in object coordinates, is stored in uv, and the distance from the ray origin is stored
             * in t. uv is in spherical coordinates, minus r [theta, phi].
             *
             * @param ray Ray to be tested for intersection with the sphere.
             * @param[out] t Distance at which the intersection ocurred, from ray origin. Undefined if not intersected.
             * @param[out] uv Coordinates in object space of the intersection. Undefined if not intersected. The coordinates are in spherical coordinates, minus r [theta, phi].
             * @return true The ray intersected the sphere, t and uv are defined.
             * @return false The ray doesn't intersect the sphere, t and uv are undefined.
             */
            auto intersection(const Entities::Ray_t& ray, double& t, std::array<double, 2>& uv) const -> bool final;

            /**
             * @brief Returns the surface normal and texture coordinates at a point in object coordinates.
             *
             * This is used to find the surface normal on ray bounce. Used by materials to determine ray colour.
             * The texture coordinates is also returned, used by materials to fetch a colour in a texture.
             * The time parameter is for motionblur shapes, where time is used to interpolate. Not used here.
             * The object coordinates are in spherical coordinates (minus r) [theta, phi].
             *
             * @param[in] time Time at which we want the normal and texture coordinates. Not used here.
             * @param[in] uv Object coordinates at which we want to find the normal and texture coordinates. The coordinates are in spherical coordinates, minus r [theta, phi].
             * @param[out] tuv Texture coordinates at the specified coordinates.
             * @return Vec3f Normal vector at the specified coordinates.
             */
            auto normaluv(double time, std::array<double, 2> uv, std::array<double, 2>& tuv) const -> Entities::Vec3f final;

            /**
             * @brief Returns the surface normal at a point in object coordinates.
             *
             * This is used to find the surface normal on ray bounce. Used by materials to determine ray colour.
             * The time parameter is for motionblur shapes, where time is used to interpolate. Not used here.
             * The object coordinates are in spherical coordinates (minus r) [theta, phi].
             *
             * @param[in] time Time at which we want the normal. Used when motion blur is used. Not used here.
             * @param[in] uv Object coordinates at which we want to find the normal. The coordinates are in spherical coordinates, minus r [theta, phi].
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
             * The object coordinates are in spherical coordinates (minus r) [theta, phi].
             *
             * @param[in] time Time at which we want the normal, tangent and texture coordinates. Used when motion blur is used. Not used here.
             * @param[in] uv Object coordinates at which we want to find the normal, texture coordinates and tangent vector. The coordinates are in spherical coordinates, minus r [theta, phi].
             * @param[out] tuv Texture coordinates at the specified coordinates.
             * @param[out] tangentvec Tangent vector at the specified coordinates.
             * @return Vec3f Normal vector at the specified coordinates.
             */
            auto normal_uv_tangent(double time, std::array<double, 2> uv, std::array<double, 2>& tuv, Entities::Vec3f& tangentvec) const -> Entities::Vec3f final;

            /**
             * @brief Minimum coordinates of an axis-aligned bounding box around the sphere.
             *
             * This is used by acceleration structures to spatially sort shapes. Returns the center position minus
             * radius for all axes.
             *
             * @return Vec3f Minimum coordinates of an axis-aligned bounding box around the sphere.
             */
            auto mincoord() const -> Entities::Vec3f final;

            /**
             * @brief Maximum coordinates of an axis-aligned bounding box around the sphere.
             *
             * This is used by acceleration structures to spatially sort shapes. Returns the center position plus
             * radius for all axes.
             *
             * @return Vec3f Maximum coordinates of an axis-aligned bounding box around the sphere.
             */
            auto maxcoord() const -> Entities::Vec3f final;
    };
}}
#endif
