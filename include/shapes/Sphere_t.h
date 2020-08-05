#ifndef SPHERE_T_H
#define SPHERE_T_H

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
     * @brief The sphere class defines a sphere shape that can be intersected by rays.
     * 
     * A sphere is defined by an origin and a radius. Its transformation matrix is used to modify those caracteristics.
     */
    class Sphere_t final : public Shape_t{
        public:
            /**
             * @brief Construct a new Sphere_t object with a transformation matrix and material.
             * 
             * @param material Material of the sphere. Material that will be bounced on at intersection.
             * @param transform_matrix Transformation used by the sphere to modify its size and position.
             */
            Sphere_t(Material_t *material, TransformMatrix_t *transform_matrix);

            /**
             * @brief Destroy the Sphere_t object. Does nothing.
             */
            virtual ~Sphere_t() final;

            double radius_; /**< @brief Radius of the sphere. Determined by the maximum scale of its transformation matrix.*/
            Vec3f origin_; /**< @brief Origin of the sphere. Position of its center.*/
            Vec3f direction_sph_; /**< @brief Rotation of the sphere, in spherical coordinates. Used for uv mapping.*/

            /**
             * @brief Updates the sphere's radius, origin and direction from its transformation matrix.
             * 
             * The scale is determined by the maximum scale of its transformation matrix.
             */
            virtual void update() final;

            /**
             * @brief Intersects a ray with the sphere, and stores information about the intersection.
             * 
             * This function returns wether a ray intersected the sphere or not, according to its direction and origin.
             * The intersection point, in object coordinates, is stored in uv, and the distance from the ray origin is stored
             * in t.
             * 
             * @param ray Ray to be tested for intersection with the sphere.
             * @param[out] t Distance at which the intersection ocurred, from ray origin. Undefined if not intersected.
             * @param[out] uv Coordinates in object space of the intersection. Undefined if not intersected. The coordinates are in spherical coordinates, minus r.
             * @return true The ray intersected the sphere, t and uv are defined.
             * @return false The ray doesn't intersect the sphere, t and uv are undefined.
             */
            virtual bool intersection(const Ray_t &ray, double &t, double (&uv)[2]) const final; 

            /**
             * @brief Returns the surface normal and texture coordinates at a point in object coordinates.
             * 
             * This is used to find the surface normal on ray bounce. Used by materials to determine ray colour.
             * The texture coordinates is also returned, used by materials to fetch a colour in a texture.
             * The time parameter is for motionblur shapes, where time is used to interpolate.
             * The object coordinates are in spherical cordinates (minus r).
             * 
             * @param[in] time Time at which we want the normal and texture coordinates. Not used here.
             * @param[in] uv Object coordinates at which we want to find the normal and texture coordinates. The coordinates are in spherical coordinates, minus r.
             * @param[out] tuv Texture coordinates at the specified coordinates and time.
             * @return Vec3f Normal vector at the specified coordinates and time.
             */
            virtual Vec3f normaluv(double time, const double (&uv)[2], double (&tuv)[2]) const final;
            virtual Vec3f normal(double time, const double (&uv)[2]) const final;
            virtual Vec3f normal_uv_tangent(double time, const double (&uv)[2], double (&tuv)[2], Vec3f &tangentvec) const final;
            virtual Vec3f mincoord() const final;
            virtual Vec3f maxcoord() const final;
    };
}}
#endif