#ifndef APTRACER_MATERIALS_REFLECTIVEREFRACTIVE_T_H
#define APTRACER_MATERIALS_REFLECTIVEREFRACTIVE_T_H

#include "entities/Material_t.h"
#include "entities/Ray_t.h"
#include "entities/Vec3f.h"
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
     * @brief The reflective refractive class describes a material that reflects and refracts rays around the surface normal, to model refractive surfaces.
     *
     * This material has an emissive and reflective colour, applied to rays on bounce.
     * Part of the rays are reflected and the rest are refracted, according to the Fresnel equations.
     * The rays are refracted according to Snell's law.
     * The rays are reflected with the same angle between the outgoing ray and the surface normal
     * as between the incident ray and the surface normal, but on he opposite side of the normal.
     * This models real refractive surfaces refraction, only a portion of the light entering the surface.
     * This material represents transparents surfaces, such as water, glass, and ice.
     */
    class ReflectiveRefractive_t final : public Material_t {
        public:
            /**
             * @brief Construct a new ReflectiveRefractive_t object with an emissive and reflective colour, and a medium.
             *
             * @param emission Colour emitted by the material when a ray bounces on it.
             * @param colour Colour reflected by the material when a ray bounces on it.
             * @param medium Medium representing the inside of the material.
             */
            ReflectiveRefractive_t(const Vec3f& emission, const Vec3f& colour, Medium_t* medium);

            Vec3f emission_; /**< @brief Colour emitted by the material at each bounce.*/
            Vec3f colour_; /**< @brief Colour reflected by the material at each bounce.*/
            std::uniform_real_distribution<double> unif_; /**< @brief Uniform random distribution used for generating random numbers.*/
            Medium_t* medium_; /**< @brief Medium representing the inside of the material. It is added to rays' medium list when entering the material, and removed when exiting it.*/

            /**
             * @brief Bounces a ray of light on the material.
             *
             * The ray's mask is attenuated with the material's colour to model part of the light being absorbed.
             * Then, the material's emissive colour is multiplied with the ray's mask and added to the ray's colour
             * to model light being emitted by the material.
             * Part of the rays are reflected and the rest are refracted, according to the Fresnel equations.
             * Refracted rays are refracted around the normal by the material, according to Snell's law. Depending on
             * its direction, the ray can enter, exit or stay inside the material. Its origin is set to the hit
             * point, and its direction is adjusted accordingly. If the ray enters or exits the material, the
             * material's medium is added or removed, respectively, from the ray's medium list. This sets in which
             * medium the ray is travelling. This models light refraction.
             * Reflected rays' direction is set with the same angle as between the
             * incident ray and the surface normal, but on he opposite side of the normal. This is a straight reflection,
             * to model specular reflection.
             *
             * @param uv Object space coordinates of the hit point. Used to query the shape for values at coordinates on it. Two components, u, and v, that can change meaning depending on the shape.
             * @param hit_obj Pointer to the shape that was hit by the ray.
             * @param ray Ray that has intersected the shape.
             */
            virtual auto bounce(std::array<double, 2> uv, const Shape_t* hit_obj, Ray_t& ray) -> void final;
    };
}}
#endif