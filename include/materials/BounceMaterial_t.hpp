#ifndef APTRACER_MATERIALS_BOUNCEMATERIAL_T_HPP
#define APTRACER_MATERIALS_BOUNCEMATERIAL_T_HPP

#include "entities/Material_t.hpp"
#include "entities/Ray_t.hpp"
#include "entities/Vec3f.hpp"
#include <random>

namespace APTracer { namespace Entities {
    class Shape_t;
}}

namespace APTracer { namespace Materials {

    /**
     * @brief The bounce material class describes a material whose colour is proportional to the number of bounces a ray has achieved.
     *
     * This material emits light proportionally to the number of bounces a ray has achieved when bouncing on it, and resets the
     * ray's mask to 1 in order to continue this process on subsequent bounces. It then reflects the ray like a diffuse material,
     * in a random direction.
     * This material is more useful for debugging than for display purposes, as it works best when all objects of the scene are
     * made of this material.
     */
    class BounceMaterial_t final : public Entities::Material_t {
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
            explicit BounceMaterial_t(unsigned int max_bounces);

            Entities::Vec3f emission_; /**< @brief Colour emitted by the material at each bounce.*/
            std::uniform_real_distribution<double> unif_; /**< @brief Uniform random distribution used for generating random numbers.*/

            /**
             * @brief Bounces a ray of light on the material.
             *
             * The ray is coloured proportionally to the number of bounces a ray has achieved when bouncing on it, and resets the
             * ray's mask to 1 in order to continue this process on subsequent bounces.
             * The ray's origin and direction are modified like a diffuse material would, to the hit point and randomly in the
             * hemisphere above the hit point, respectively.
             *
             * @param uv Object space coordinates of the hit point. Used to query the shape for values at coordinates on it. Two components, u, and v, that can change meaning depending on the shape.
             * @param hit_obj Pointer to the shape that was hit by the ray.
             * @param ray Ray that has intersected the shape.
             */
            auto bounce(std::array<double, 2> uv, const Entities::Shape_t* hit_obj, Entities::Ray_t& ray) -> void final;
    };
}}
#endif
