#ifndef APTRACER_TOON_T_H
#define APTRACER_TOON_T_H

#include "entities/Material_t.h"
#include "entities/Vec3f.h"
#include "entities/Ray_t.h"

namespace APTracer { namespace Entities {
    class Shape_t;
}}

using APTracer::Entities::Material_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;

namespace APTracer { namespace Materials {

    /**
     * @brief The toon class describes a material that has a solid colour, regardless of lighting.
     * 
     * This material sets rays' colour to its colour, and stops them bouncing.
     * This material isn't affected by lights and shadows, or any lighting.
     * This material is more useful for debugging, to force a specific colour, 
     * to create impossible lighting and for toon-like shading.
     */
    class Toon_t final : public Material_t{
        public:
            /**
             * @brief Construct a new Toon_t object with a colour.
             * 
             * @param colour Colour of the material, regardless of shading.
             */
            Toon_t(const Vec3f &colour);

            Vec3f colour_; /**< @brief Colour of the material, regardless of shading.*/

            /**
             * @brief Bounces a ray of light on the material.
             * 
             * This sets the ray's colour to the material's colour, and sets the ray's mask to 0 to prevent further bouncing.
             * 
             * @param uv Object space coordinates of the hit point. Used to query the shape for values at coordinates on it. Two components, u, and v, that can change meaning depending on the shape.
             * @param hit_obj Pointer to the shape that was hit by the ray.
             * @param ray Ray that has intersected the shape.
             */
            virtual auto bounce(std::array<double, 2> uv, const Shape_t* hit_obj, Ray_t &ray) -> void final;
    };
}}
#endif