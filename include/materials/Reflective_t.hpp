#ifndef APTRACER_MATERIALS_REFLECTIVE_T_HPP
#define APTRACER_MATERIALS_REFLECTIVE_T_HPP

#include "entities/Material_t.hpp"
#include "entities/Ray_t.hpp"
#include "entities/Vec3f.hpp"

namespace APTracer { namespace Entities {
    class Shape_t;
}}

namespace APTracer { namespace Materials {

    /**
     * @brief The reflective class describes a material that reflects rays at the same angle around the surface normal, to model specular reflection.
     *
     * This material has an emissive and reflective colour, applied to rays on bounce.
     * The rays are reflected with the same angle between the outgoing ray and the surface normal
     * as between the incident ray and the surface normal, but on he opposite side of the normal.
     * This models specular reflection, which reflect light in a single direction.
     * This material represents flat shiny surfaces, such as mirrors,
     * very polished metals, and shiny plastic.
     */
    class Reflective_t final : public Entities::Material_t {
        public:
            /**
             * @brief Construct a new Reflective_t object with an emissive and reflective colour.
             *
             * @param emission Colour emitted by the material when a ray bounces on it.
             * @param colour Colour reflected by the material when a ray bounces on it.
             */
            Reflective_t(const Entities::Vec3f& emission, const Entities::Vec3f& colour);

            Entities::Vec3f emission_; /**< @brief Colour emitted by the material at each bounce.*/
            Entities::Vec3f colour_; /**< @brief Colour reflected by the material at each bounce.*/

            /**
             * @brief Bounces a ray of light on the material.
             *
             * The ray's mask is attenuated with the material's colour to model part of the light being absorbed.
             * Then, the material's emissive colour is multiplied with the ray's mask and added to the ray's colour
             * to model light being emitted by the material.
             * The ray's origin is set to the hit point, and its direction is set with the same angle as between the
             * incident ray and the surface normal, but on he opposite side of the normal. This is a straight reflection,
             * to model specular reflection.
             *
             * @param uv Object space coordinates of the hit point. Used to query the shape for values at coordinates on it. Two components, u, and v, that can change meaning depending on the shape.
             * @param hit_obj Pointer to the shape that was hit by the ray.
             * @param ray Ray that has intersected the shape.
             */
            auto bounce(std::array<double, 2> uv, const Entities::Shape_t* hit_obj, Entities::Ray_t& ray) -> void final;
    };
}}
#endif
