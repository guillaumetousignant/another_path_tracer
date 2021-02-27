#ifndef APTRACER_DISTANCEMATERIAL_T_H
#define APTRACER_DISTANCEMATERIAL_T_H

#include "entities/Material_t.h"
#include "entities/Ray_t.h"

namespace APTracer { namespace Entities {
    class Shape_t;
}}

using APTracer::Entities::Material_t;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;

namespace APTracer { namespace Materials {

    /**
     * @brief The distance material describes a material whose colour is proportional to the distance traveled by a ray.
     * 
     * This material emits light proportionally to the distance a ray has traveled when bouncing on it, and resets the
     * ray's mask to 0 in order to stop it from being cast again. 
     * This material is more useful for debugging than for display purposes.
     */
    class DistanceMaterial_t final : public Material_t{
        public:
            /**
             * @brief Construct a new DistanceMaterial_t object with a target distance and an exponent.
             * 
             * The colour of the material will be the inverse of the difference between the distance travelled 
             * by the ray and the target distance raised to an exponent. 
             * 
             * @param focal_length Distance at which the colour of the material will be white.
             * @param exponent Exponent on the difference between the target distance and the distance traveled. Higher numbers make the colour darker until is is very close to the target distance.
             */
            DistanceMaterial_t(double focal_length, double exponent);

            /**
             * @brief Destroy the DistanceMaterial_t object. Does nothing.
             */
            virtual ~DistanceMaterial_t() final;

            double focal_length_; /**< @brief Distance at which the colour of the material will be white.*/
            double exponent_; /**< @brief Exponent on the difference between the target distance and the distance traveled. Higher numbers make the colour darker until is is very close to the target distance.*/

            /**
             * @brief Bounces a ray of light on the material.
             * 
             * The ray is coloured proportionally to the distance a ray has traveled when bouncing on it, and resets the
             * ray's mask to 0 in order to stop it from being cast again.
             * 
             * @param uv Object space coordinates of the hit point. Used to query the shape for values at coordinates on it. Two components, u, and v, that can change meaning depending on the shape.
             * @param hit_obj Pointer to the shape that was hit by the ray.
             * @param ray Ray that has intersected the shape.
             */
            virtual void bounce(std::array<double, 2> uv, const Shape_t* hit_obj, Ray_t &ray) final;
    };
}}
#endif