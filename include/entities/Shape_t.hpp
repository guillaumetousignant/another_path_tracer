#ifndef APTRACER_ENTITIES_SHAPE_T_HPP
#define APTRACER_ENTITIES_SHAPE_T_HPP

#include "entities/Material_t.hpp"
#include "entities/Ray_t.hpp"
#include "entities/TransformMatrix_t.hpp"
#include "entities/Vec3f.hpp"
#include <array>

namespace APTracer { namespace Entities {

    /**
     * @brief The shape interface class describes surfaces which can be intersected by a ray.
     *
     * Shapes are surfaces that can be intersected by rays and shaded. Rays bounce on them according to their material, and they can provide
     * information about an intersection, like the hit point, object coordinates hit point, surface normal, etc. Shapes are stored in a scene and
     * in an acceleration structure, and as such must provide the mincoord and maxcoord functions to be spatially sorted. The shapes can be modified
     * by applying transformations to their transformation matrix. These changes are propagated on update.
     */
    class Shape_t {
        public:
            /**
             * @brief Construct a new Shape_t object with a material and a transformation matrix.
             *
             * @param material Material of the shape.
             * @param transform_matrix Transformation matrix used to transform and move the shape.
             */
            Shape_t(Material_t* material, TransformMatrix_t* transform_matrix) : material_(material), transformation_(transform_matrix){};

            /**
             * @brief Destroy the Shape_t object.
             */
            virtual ~Shape_t() = default;

            /**
             * @brief Copy construct a new Shape_t object.
             *
             * @param other Object to copy.
             */
            Shape_t(const Shape_t& other) = default;

            /**
             * @brief Move construct a new Shape_t object.
             *
             * @param other Object to move.
             */
            Shape_t(Shape_t&& other) noexcept = default;

            /**
             * @brief Copy assignment.
             *
             * @param other Object to copy.
             * @return Camera_t& Reference to this object.
             */
            auto operator=(const Shape_t& other) -> Shape_t& = default;

            /**
             * @brief Move assignment.
             *
             * @param other Object to move.
             * @return Camera_t& Reference to this object.
             */
            auto operator=(Shape_t&& other) noexcept -> Shape_t& = default;

            Material_t* material_; /**< @brief Material of which the shape is made of.*/
            TransformMatrix_t* transformation_; /**< @brief Transformation matrix used to modify the position and other transformations of the shape.*/

            /**
             * @brief Propagates changes from the shape's transformation matrix to the shape.
             *
             * Usually called once per frame, will modify the shape once with all changes that have been made to the transformation
             * matrix since last update.
             */
            virtual auto update() -> void = 0;

            /**
             * @brief Calculate the intersection between a ray and a shape, and stores information about this intersection.
             *
             * This function must return wether a ray intersected the shape or not, according to its direction and origin.
             * The intersection point, in object coordinates, is stored in uv, and the distance from the ray origin is stored
             * in t.
             *
             * @param ray Ray to be tested for intersection with the shape.
             * @param[out] t Distance at which the intersection ocurred, from ray origin. Undefined if not intersected.
             * @param[out] uv Coordinates in object space of the intersection. Undefined if not intersected.
             * @return true The ray intersected the shape, t and uv are defined.
             * @return false The ray doesn't intersect the shape, t and uv are undefined.
             */
            virtual auto intersection(const Ray_t& ray, double& t, std::array<double, 2>& uv) const -> bool = 0;

            /**
             * @brief Returns the surface normal at a point in object coordinates.
             *
             * This is used to find the surface normal on ray bounce. Used by materials to determine ray colour.
             * The time parameter is for motionblur shapes, where time is used to interpolate.
             *
             * @param time Time at which we want the normal. Used when motion blur is used.
             * @param uv Object coordinates at which we want to find the normal.
             * @return Vec3f Normal vector at the specified coordinates and time.
             */
            virtual auto normal(double time, std::array<double, 2> uv) const -> Vec3f = 0;

            /**
             * @brief Returns the surface normal and texture coordinates at a point in object coordinates.
             *
             * This is used to find the surface normal on ray bounce. Used by materials to determine ray colour.
             * The texture coordinates is also returned, used by materials to fetch a colour in a texture.
             * The time parameter is for motionblur shapes, where time is used to interpolate.
             *
             * @param[in] time Time at which we want the normal and texture coordinates. Used when motion blur is used.
             * @param[in] uv Object coordinates at which we want to find the normal and texture coordinates.
             * @param[out] tuv Texture coordinates at the specified coordinates and time.
             * @return Vec3f Normal vector at the specified coordinates and time.
             */
            virtual auto normaluv(double time, std::array<double, 2> uv, std::array<double, 2>& tuv) const -> Vec3f = 0;

            /**
             * @brief Returns the surface normal, texture coordinates and tangent vector at a point in object coordinates.
             *
             * This is used to find the surface normal on ray bounce. Used by materials to determine ray colour.
             * The texture coordinates is also returned, used by materials to fetch a colour in a texture. The tangent
             * vector is used by materials for normal mapping, as the normals returned by those textures are in object
             * coordinates. The time parameter is for motionblur shapes, where time is used to interpolate.
             *
             * @param[in] time Time at which we want the normal and texture coordinates. Used when motion blur is used.
             * @param[in] uv Object coordinates at which we want to find the normal, texture coordinates and tangent vector.
             * @param[out] tuv Texture coordinates at the specified coordinates and time.
             * @param[out] tangentvec Tangent vector at the specified coordinates and time.
             * @return Vec3f Normal vector at the specified coordinates and time.
             */
            virtual auto normal_uv_tangent(double time, std::array<double, 2> uv, std::array<double, 2>& tuv, Vec3f& tangentvec) const -> Vec3f = 0;

            /**
             * @brief Minimum coordinates of an axis-aligned bounding box around the shape.
             *
             * This is used by acceleration structures to spatially sort shapes. Equivalent to the minimum coordinate of the
             * shape for each axis.
             *
             * @return Vec3f Minimum coordinates of an axis-aligned bounding box around the shape.
             */
            virtual auto mincoord() const -> Vec3f = 0;

            /**
             * @brief Maximum coordinates of an axis-aligned bounding box around the shape.
             *
             * This is used by acceleration structures to spatially sort shapes. Equivalent to the maximum coordinate of the
             * shape for each axis.
             *
             * @return Vec3f Maximum coordinates of an axis-aligned bounding box around the shape.
             */
            virtual auto maxcoord() const -> Vec3f = 0;
    };
}}
#endif