#ifndef APTRACER_MATERIALS_RANDOMMIXIN_T_HPP
#define APTRACER_MATERIALS_RANDOMMIXIN_T_HPP

#include "entities/Material_t.hpp"
#include "entities/Ray_t.hpp"
#include <random>

namespace APTracer { namespace Entities {
    class Shape_t;
}}

namespace APTracer { namespace Materials {

    /**
     * @brief The random mix in material describes a mix of two materials, with the material being chosen randomly on bounce according to a ratio on entrance, and the refracted material on material
     * exit.
     *
     * This material will bounce rays on either of its material randomly according to a ratio. The
     * ratio is the proportion of light bounced on the refracted material.
     * When exiting the material, the refracted material is always used.
     * This material models surfaces that are a mix of two materials, like dusty surfaces or surfaces
     * that partly emit light.
     */
    class RandomMixIn_t final : public Entities::Material_t {
        public:
            /**
             * @brief Construct a new RandomMixIn_t object from two materials and a ratio.
             *
             * @param first_material First material, that will be randomly bounced according to the ratio, and always on exit.
             * @param second_material Second material, that will be randomly bounced according to (1 - ratio).
             * @param ratio Proportion of the light bounced by the first material, from 0 to 1.
             */
            RandomMixIn_t(Material_t* first_material, Material_t* second_material, double ratio);

            Material_t* first_material_; /**< @brief Material that will be bounced in proportion to the ratio, and always on exit.*/
            Material_t* second_material_; /**< @brief Material that will be bounced in proportion to (1 - ratio).*/
            double ratio_; /**< @brief Proportion of the light bounced by the first, "refracted", material when entering the material.*/
            std::uniform_real_distribution<double> unif_; /**< @brief Uniform random distribution used for generating random numbers.*/

            /**
             * @brief Bounces a ray of light on the material.
             *
             * The ray is bounced randomly on either the reflected or refracted material according
             * to a ratio, which if the proportion of light bounced by the refracted material.
             * When exiting the material, the refracted material is always used.
             *
             * @param uv Object space coordinates of the hit point. Used to query the shape for values at coordinates on it. Two components, u, and v, that can change meaning depending on the shape.
             * @param hit_obj Pointer to the shape that was hit by the ray.
             * @param ray Ray that has intersected the shape.
             */
            auto bounce(std::array<double, 2> uv, const Entities::Shape_t* hit_obj, Entities::Ray_t& ray) -> void final;
    };
}}
#endif