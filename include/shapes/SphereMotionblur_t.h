#ifndef APTRACER_SPHEREMOTIONBLUR_T_H
#define APTRACER_SPHEREMOTIONBLUR_T_H

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
     * @brief The sphere motionblur class defines a sphere shape that can be intersected by rays, with motion blur enabled.
     * 
     * A sphere is defined by an origin and a radius. Its transformation matrix is used to modify those caracteristics.
     * This shape remembers its last state to enable motion blur. The state is interpolated between current and previous
     * states according to a ray's time.
     */
    class SphereMotionblur_t final : public Shape_t {
        public:
            /**
             * @brief Construct a new SphereMotionblur_t object with a transformation matrix and material.
             * 
             * @param material Material of the sphere. Material that will be bounced on at intersection.
             * @param transform_matrix Transformation used by the sphere to modify its size and position.
             */
            SphereMotionblur_t(Material_t *material, TransformMatrix_t *transform_matrix);

            double radius_; /**< @brief Radius of the sphere. Determined by the maximum scale of its transformation matrix.*/
            Vec3f origin_; /**< @brief Origin of the sphere. Position of its center.*/
            Vec3f direction_sph_; /**< @brief Rotation of the sphere, in spherical coordinates. Used for uv mapping.*/
            double radius_last_; /**< @brief Radius of the sphere before last update. Used for motion blur.*/
            Vec3f origin_last_; /**< @brief Origin of the sphere before last update. Used for motion blur.*/
            Vec3f direction_sph_last_; /**< @brief Rotation of the sphere before last update. Used for motion blur.*/

            /**
             * @brief Updates the sphere's radius, origin and direction from its transformation matrix.
             * 
             * Stores the previous state in the _last variables, so that state can be interpolated according to time.
             * The scale is determined by the maximum scale of its transformation matrix.
             */
            virtual auto update() -> void final;

            /**
             * @brief Intersects a ray with the sphere at a specific time, and stores information about the intersection.
             * 
             * This function returns wether a ray intersected the sphere or not, according to its direction, origin and time.
             * The intersection point, in object coordinates, is stored in uv, and the distance from the ray origin is stored
             * in t. uv is in spherical coordinates, minus r [theta, phi]. Intersection is computed at ray.time, which 
             * interpolates between the previous and current state, with 0 being previous and 1 being current.
             * 
             * @param ray Ray to be tested for intersection with the sphere.
             * @param[out] t Distance at which the intersection ocurred, from ray origin. Undefined if not intersected.
             * @param[out] uv Coordinates in object space of the intersection. Undefined if not intersected. The coordinates are in spherical coordinates, minus r [theta, phi].
             * @return true The ray intersected the sphere, t and uv are defined.
             * @return false The ray doesn't intersect the sphere, t and uv are undefined.
             */
            virtual auto intersection(const Ray_t &ray, double &t, std::array<double, 2> &uv) const -> bool final; 

            /**
             * @brief Returns the surface normal and texture coordinates at a point in object coordinates and a specific time.
             * 
             * This is used to find the surface normal on ray bounce. Used by materials to determine ray colour.
             * The texture coordinates is also returned, used by materials to fetch a colour in a texture.
             * The time parameter is used to interpolate between previous and current states, 0 and 1 respectively.
             * The object coordinates are in spherical cordinates (minus r) [theta, phi].
             * 
             * @param[in] time Time at which we want the normal and texture coordinates, from 0 to 1 for previous and current states.
             * @param[in] uv Object coordinates at which we want to find the normal and texture coordinates. The coordinates are in spherical coordinates, minus r [theta, phi].
             * @param[out] tuv Texture coordinates at the specified coordinates and time.
             * @return Vec3f Normal vector at the specified coordinates and time.
             */
            virtual auto normaluv(double time, std::array<double, 2> uv, std::array<double, 2> &tuv) const -> Vec3f final;

            /**
             * @brief Returns the surface normal at a point in object coordinates and a specific time.
             * 
             * This is used to find the surface normal on ray bounce. Used by materials to determine ray colour.
             * The time parameter is used to interpolate between previous and current states, 0 and 1 respectively.
             * The object coordinates are in spherical cordinates (minus r) [theta, phi].
             * 
             * @param[in] time Time at which we want the normal, from 0 to 1 for previous and current states.
             * @param[in] uv Object coordinates at which we want to find the normal. The coordinates are in spherical coordinates, minus r [theta, phi].
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
             * The object coordinates are in spherical cordinates (minus r) [theta, phi].
             * 
             * @param[in] time Time at which we want the normal, tangent and texture coordinates, from 0 to 1 for previous and current states.
             * @param[in] uv Object coordinates at which we want to find the normal, texture coordinates and tangent vector. The coordinates are in spherical coordinates, minus r [theta, phi].
             * @param[out] tuv Texture coordinates at the specified coordinates and time.
             * @param[out] tangentvec Tangent vector at the specified coordinates and time.
             * @return Vec3f Normal vector at the specified coordinates and time.
             */
            virtual auto normal_uv_tangent(double time, std::array<double, 2> uv, std::array<double, 2> &tuv, Vec3f &tangentvec) const -> Vec3f final;

            /**
             * @brief Minimum coordinates of an axis-aligned bounding box around the sphere.
             * 
             * This is used by acceleration structures to spatially sort shapes. Returns the lowest of the center position minus 
             * radius for all axes between the previous and current time. This ensures the minimum coordinate over the time range
             * is returned.
             * 
             * @return Vec3f Minimum coordinates of an axis-aligned bounding box around the sphere.
             */
            virtual auto mincoord() const -> Vec3f final;

            /**
             * @brief Maximum coordinates of an axis-aligned bounding box around the sphere.
             * 
             * This is used by acceleration structures to spatially sort shapes. Returns the highest of the center position plus 
             * radius for all axes between the previous and current time. This ensures the maximum coordinate over the time range
             * is returned.
             * 
             * @return Vec3f Maximum coordinates of an axis-aligned bounding box around the sphere.
             */
            virtual auto maxcoord() const -> Vec3f final;
    };
}}
#endif