#ifndef APTRACER_MATERIALS_TRANSPARENT_T_HPP
#define APTRACER_MATERIALS_TRANSPARENT_T_HPP

#include "entities/Material_t.hpp"
#include "entities/Medium_t.hpp"
#include "entities/Ray_t.hpp"
#include "entities/Vec3f.hpp"

namespace APTracer { namespace Entities {
    class Shape_t;
}}

namespace APTracer { namespace Materials {

    /**
     * @brief The transparent class describes a material that is invisible.
     *
     * This material is invisible, and rays go straight through it. It still
     * adds its medium to rays' medium lists on entrance and removes is on exit.
     * This material can be used to represent volumetric materials, where the
     * medium is important and there is no surface interaction at its boundary.
     */
    class Transparent_t final : public Entities::Material_t {
        public:
            /**
             * @brief Construct a new Transparent_t object with a medium.
             *
             * @param medium Medium on the inside of the material.
             */
            explicit Transparent_t(Entities::Medium_t* medium);

            Entities::Medium_t* medium_; /**< @brief Medium on the inside of the material. It is added to a ray's medium list when entering the material and removed when exiting it.*/

            /**
             * @brief Bounces a ray of light on the material.
             *
             * This doesn't change the ray's colour, mask or direction. It moves its origin beyond the hit point so the ray goes through the material.
             * If the ray enters the material, the material's medium is added to the ray's medium list. If it exits the material, the medium is removed
             * from the ray's medium list. Wether the ray enters or exits the material is determined by the surface normal. If the ray's direction and
             * the surface normal are in opposite directions, the ray is entering.
             *
             * @param uv Object space coordinates of the hit point. Used to query the shape for values at coordinates on it. Two components, u, and v, that can change meaning depending on the shape.
             * @param hit_obj Pointer to the shape that was hit by the ray.
             * @param ray Ray that has intersected the shape.
             */
            auto bounce(std::array<double, 2> uv, const Entities::Shape_t* hit_obj, Entities::Ray_t& ray) -> void final;
    };
}}
#endif