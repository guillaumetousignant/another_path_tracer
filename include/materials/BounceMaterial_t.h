#ifndef APTRACER_BOUNCEMATERIAL_T_H
#define APTRACER_BOUNCEMATERIAL_T_H

#include "entities/Material_t.h"
#include "entities/Vec3f.h"
#include "entities/Ray_t.h"
#include <random>

namespace APTracer { namespace Entities {
    class Shape_t;
}}

using APTracer::Entities::Material_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;

namespace APTracer { namespace Materials {

    /**
     * @brief The bounce material class describes a material whose colour is proportional to the number of bounces a ray has achieved.
     * 
     * This material emits light proportionally to the number of bounces a ray has achieved when bouncing on it, and resets the
     * ray's mask to 1 in order to continue this process on subsequent bounces. It then reflects the ray like a diffuse material,
     * in a random direction. 
     * This material is more usefull for debugging than for display purposes, as it works best when all objects of the scene are
     * made of this material.
     */
    class BounceMaterial_t final : public Material_t{
        public:
            /**
             * @brief Construct a new BounceMaterial_t object with a maximum number of bounces.
             * 
             * The maximum number of bounces is not a hard limit, the emission of the material is 1.0/max_bounces,
             * so after max_bounces a ray should have a white colour. It can still bounce further and accumulate
             * a colour higher than 1.0.
             * 
             * @param max_bounces Number of bounces at which the colour a ray would be white.
             */
            BounceMaterial_t(unsigned int max_bounces);

            /**
             * @brief Destroy the BounceMaterial_t object. Does nothing.
             */
            virtual ~BounceMaterial_t() final;

            Vec3f emission_; /**< @brief Colour emitted by the material at each bounce.*/  
            std::uniform_real_distribution<double> unif_; /**< @brief Uniform random distribution used for generating random numbers.*/

            /**
             * @brief Bounces a ray of light on the material.
             * 
             * The ray is coloured proportionally to the number of bounces a ray has achieved when bouncing on it, and resets the
             * ray's mask to 1 in order to continue this process on subsequent bounces.
             * The ray's origin and direction are modified like a diffuse material would, to the hitpoint and randomly in the 
             * hemisphere above the hit point, respectively.
             * 
             * @param uv Object space coordinates of the hit point. Used to query the shape for values at coordinates on it. Two components, u, and v, that can change meaning depending on the shape.
             * @param hit_obj Pointer to the shape that was hit by the ray.
             * @param ray Ray that has intersected the shape.
             */
            virtual void bounce(std::array<double, 2> uv, const Shape_t* hit_obj, Ray_t &ray) final;
    };
}}
#endif