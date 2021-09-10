#ifndef APTRACER_MATERIALS_REFRACTIVE_T_H
#define APTRACER_MATERIALS_REFRACTIVE_T_H

#include "entities/Material_t.h"
#include "entities/Vec3f.h"
#include "entities/Ray_t.h"

namespace APTracer { namespace Entities {
    class Medium_t;
    class Shape_t;
}}

using APTracer::Entities::Material_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;
using APTracer::Entities::Medium_t;

namespace APTracer { namespace Materials {

    /**
     * @brief The refractive class describes a material that refracts rays around the surface normal, to model light refraction.
     * 
     * This material has an emissive and reflective colour, applied to rays on bounce.
     * The rays are refracted according to Snell's law.
     * This models light refraction, with going from one medium to the next.
     * This material represents transparents surfaces, such as water, glass, and ice.
     */
    class Refractive_t final : public Material_t {
        public:
            /**
             * @brief Construct a new Refractive_t object with an emissive and reflective colour, and a medium.
             * 
             * @param emission Colour emitted by the material when a ray bounces on it.
             * @param colour Colour reflected by the material when a ray bounces on it.
             * @param medium Medium representing the inside of the material.
             */
            Refractive_t(const Vec3f &emission, const Vec3f &colour, Medium_t* medium);

            Vec3f emission_; /**< @brief Colour emitted by the material at each bounce.*/
            Vec3f colour_; /**< @brief Colour reflected by the material at each bounce.*/
            Medium_t* medium_; /**< @brief Medium representing the inside of the material. It is added to rays' medium list when entering the material, and removed when exiting it.*/

            /**
             * @brief Bounces a ray of light on the material.
             * 
             * The ray's mask is attenuated with the material's colour to model part of the light being absorbed.
             * Then, the material's emissive colour is multiplied with the ray's mask and added to the ray's colour
             * to model light being emitted by the material.
             * The ray is then refracted around the normal by the material, according to Snell's law. Depending on 
             * its direction, the ray can enter, exit or stay inside the material. Its origin is set to the hit 
             * point, and its direction is adjusted accordingly. If the ray enters or exits the material, the 
             * material's medium is added or removed, respectively, from the ray's medium list. This sets in which 
             * medium the ray is travelling. This models light refraction.
             * 
             * @param uv Object space coordinates of the hit point. Used to query the shape for values at coordinates on it. Two components, u, and v, that can change meaning depending on the shape.
             * @param hit_obj Pointer to the shape that was hit by the ray.
             * @param ray Ray that has intersected the shape.
             */
            virtual auto bounce(std::array<double, 2> uv, const Shape_t* hit_obj, Ray_t &ray) -> void final;
    };
}}
#endif