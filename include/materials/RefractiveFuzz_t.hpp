#ifndef APTRACER_MATERIALS_REFRACTIVEFUZZ_T_HPP
#define APTRACER_MATERIALS_REFRACTIVEFUZZ_T_HPP

#include "entities/Material_t.hpp"
#include "entities/Ray_t.hpp"
#include "entities/Vec3f.hpp"
#include <random>

namespace APTracer { namespace Entities {
    class Medium_t;
    class Shape_t;
}}

using APTracer::Entities::Material_t;
using APTracer::Entities::Medium_t;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;
using APTracer::Entities::Vec3f;

namespace APTracer { namespace Materials {

    /**
     * @brief The refractive class describes a material that refracts rays around the randomly disturbed surface normal, to model light refraction.
     *
     * This material has an emissive and reflective colour, applied to rays on bounce.
     * The rays are refracted according to Snell's law.
     * The normal has a random disruption applied to it to move it within its hemisphere.
     * This models light refraction, with going from one medium to the next.
     * This material represents transparents rough surfaces, such as frosted glass and rough ice.
     */
    class RefractiveFuzz_t final : public Material_t {
        public:
            /**
             * @brief Construct a new RefractiveFuzz_t object with an emissive and reflective colour, a medium, the order of the curve describing the distribution of the surface normal, and a
             * diffusivity factor.
             *
             * @param emission Colour emitted by the material when a ray bounces on it.
             * @param colour Colour reflected by the material when a ray bounces on it.
             * @param order Order of the curve describing the distribution of the surface normal, from 0 to inf. 0 is forced to the edge of the radius described by the diffusivity. Between 0 and 1 is
             * a meniscus shape, 1 is flat, 2 is parabole, higher looks like normal and inf is straight reflection.
             * @param diffusivity Portion of the hemisphere around the normal in which the normal can be randomly moved, from 0 to 1. 0 is straight reflection, 1 is all the hemisphere.
             * @param medium Medium representing the inside of the material.
             */
            RefractiveFuzz_t(const Vec3f& emission, const Vec3f& colour, double order, double diffusivity, Medium_t* medium);

            Vec3f emission_; /**< @brief Colour emitted by the material at each bounce.*/
            Vec3f colour_; /**< @brief Colour reflected by the material at each bounce.*/
            double order_; /**< @brief Order of the curve describing the distribution of the surface normal, from 0 to inf. 0 is forced to the edge of the radius described by the diffusivity. Between
                              0 and 1 is a meniscus shape, 1 is flat, 2 is parabole, higher looks like normal and inf is straight reflection.*/
            double diffusivity_; /**< @brief Portion of the hemisphere around the normal in which the normal can be randomly moved, from 0 to 1. 0 is straight reflection, 1 is all the hemisphere.*/
            std::uniform_real_distribution<double> unif_; /**< @brief Uniform random distribution used for generating random numbers.*/
            Medium_t* medium_; /**< @brief Medium representing the inside of the material. It is added to rays' medium list when entering the material, and removed when exiting it.*/

            /**
             * @brief Bounces a ray of light on the material.
             *
             * The ray's mask is attenuated with the material's colour to model part of the light being absorbed.
             * Then, the material's emissive colour is multiplied with the ray's mask and added to the ray's colour
             * to model light being emitted by the material.
             * The normal has a random disruption applied to it to move it within a portion of its hemisphere, with
             * the portion being represented by the diffusivity parameter, and the probability distribution within
             * the portion being governed by the order parameter.
             * The ray is then refracted around the normal by the material, according to Snell's law. Depending on
             * its direction, the ray can enter, exit or stay inside the material. Its origin is set to the hit
             * point, and its direction is adjusted accordingly. If the ray enters or exits the material, the
             * material's medium is added or removed, respectively, from the ray's medium list. This sets in which
             * medium the ray is ravelling. This models light refraction.
             *
             * @param uv Object space coordinates of the hit point. Used to query the shape for values at coordinates on it. Two components, u, and v, that can change meaning depending on the shape.
             * @param hit_obj Pointer to the shape that was hit by the ray.
             * @param ray Ray that has intersected the shape.
             */
            virtual auto bounce(std::array<double, 2> uv, const Shape_t* hit_obj, Ray_t& ray) -> void final;
    };
}}
#endif