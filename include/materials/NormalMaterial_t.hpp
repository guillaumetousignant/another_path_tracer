#ifndef APTRACER_MATERIALS_NORMALMATERIAL_T_HPP
#define APTRACER_MATERIALS_NORMALMATERIAL_T_HPP

#include "entities/Material_t.hpp"
#include "entities/Ray_t.hpp"

namespace APTracer { namespace Entities {
    class Shape_t;
}}

using APTracer::Entities::Material_t;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;

namespace APTracer { namespace Materials {

    /**
     * @brief The normal material class describes a material that emits colour relative to the surface normal at the hit point.
     *
     * This material emits light proportionally to the surface normal in world coordinates, x y and z being red green and blue, respectively.
     * It then resets the ray's mask to 0 in order to stop it from being cast again.
     * This material is more useful for debugging than for display purposes.
     */
    class NormalMaterial_t final : public Material_t {
        public:
            /**
             * @brief Construct a new NormalMaterial_t object.
             */
            NormalMaterial_t() = default;

            /**
             * @brief Bounces a ray of light on the material.
             *
             * The ray is coloured according to the surface normal at the hit point, x y and z being red green and blue, respectively.
             * It then resets the ray's mask to 0 in order to stop it from being cast again.
             *
             * @param uv Object space coordinates of the hit point. Used to query the shape for values at coordinates on it. Two components, u, and v, that can change meaning depending on the shape.
             * @param hit_obj Pointer to the shape that was hit by the ray.
             * @param ray Ray that has intersected the shape.
             */
            virtual auto bounce(std::array<double, 2> uv, const Shape_t* hit_obj, Ray_t& ray) -> void final;
    };
}}
#endif