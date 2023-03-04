#ifndef APTRACER_MATERIALS_TOON_T_HPP
#define APTRACER_MATERIALS_TOON_T_HPP

#include "entities/Material_t.hpp"
#include "entities/Ray_t.hpp"
#include "entities/Vec3f.hpp"

namespace APTracer { namespace Entities {
    class Shape_t;
}}

namespace APTracer { namespace Materials {

    /**
     * @brief The toon class describes a material that has a solid colour, regardless of lighting.
     *
     * This material sets rays' colour to its colour, and stops them bouncing.
     * This material isn't affected by lights and shadows, or any lighting.
     * This material is more useful for debugging, to force a specific colour,
     * to create impossible lighting and for toon-like shading.
     */
    class Toon_t final : public Entities::Material_t {
        public:
            /**
             * @brief Construct a new Toon_t object with a colour.
             *
             * @param colour Colour of the material, regardless of shading.
             */
            explicit Toon_t(const Entities::Vec3f& colour);

            Entities::Vec3f colour_; /**< @brief Colour of the material, regardless of shading.*/

            /**
             * @brief Bounces a ray of light on the material.
             *
             * This sets the ray's colour to the material's colour, and sets the ray's mask to 0 to prevent further bouncing.
             *
             * @param uv Object space coordinates of the hit point. Used to query the shape for values at coordinates on it. Two components, u, and v, that can change meaning depending on the shape.
             * @param hit_obj Pointer to the shape that was hit by the ray.
             * @param ray Ray that has intersected the shape.
             */
            auto bounce(std::array<double, 2> uv, const Entities::Shape_t* hit_obj, Entities::Ray_t& ray) -> void final;
    };
}}
#endif
