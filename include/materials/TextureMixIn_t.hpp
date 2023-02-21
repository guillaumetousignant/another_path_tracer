#ifndef APTRACER_MATERIALS_TEXTUREMIXIN_T_HPP
#define APTRACER_MATERIALS_TEXTUREMIXIN_T_HPP

#include "entities/Material_t.hpp"
#include "entities/Ray_t.hpp"
#include <random>

namespace APTracer { namespace Entities {
    class Texture_t;
    class Shape_t;
}}

using APTracer::Entities::Material_t;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;
using APTracer::Entities::Texture_t;

namespace APTracer { namespace Materials {

    /**
     * @brief The texture mix in material describes a mix of two materials, with the material being chosen randomly on bounce according to a ratio taken from a texture on entrance, and the refracted
     * material on material exit.
     *
     * This material will bounce rays on either of its material randomly according to a ratio. The
     * ratio is the proportion of light bounced on the refracted material, it is read from a texture.
     * When exiting the material, the refracted material is always used.
     * This material models surfaces that are a mix of two materials, like dusty surfaces or surfaces
     * that partly emit light.
     */
    class TextureMixIn_t final : public Material_t {
        public:
            /**
             * @brief Construct a new TextureMixIn_t object from two materials and a ratio texture.
             *
             * @param first_material First material, that will be randomly bounced according to the ratio, and always on exit.
             * @param second_material Second material, that will be randomly bounced according to the (1 - ratio).
             * @param texture Texture containing the proportion of the light bounced by the first material, from 0 to 1 in the first channel.
             */
            TextureMixIn_t(Material_t* first_material, Material_t* second_material, const Texture_t* texture);

            Material_t* first_material_; /**< @brief Material that will be bounced in proportion to the ratio, and always on exit.*/
            Material_t* second_material_; /**< @brief Material that will be bounced in proportion to (1 - ratio).*/
            const Texture_t* texture_; /**< @brief Texture containing the proportion of the first material, 0 to 1 in the first channel.*/
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
            virtual auto bounce(std::array<double, 2> uv, const Shape_t* hit_obj, Ray_t& ray) -> void final;
    };
}}
#endif