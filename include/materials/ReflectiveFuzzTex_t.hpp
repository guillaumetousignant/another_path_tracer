#ifndef APTRACER_MATERIALS_REFLECTIVEFUZZTEX_T_HPP
#define APTRACER_MATERIALS_REFLECTIVEFUZZTEX_T_HPP

#include "entities/Material_t.hpp"
#include "entities/Ray_t.hpp"
#include "entities/Texture_t.hpp"
#include "entities/Vec3f.hpp"
#include <random>

namespace APTracer { namespace Entities {
    class Shape_t;
}}

namespace APTracer { namespace Materials {

    /**
     * @brief The reflective fuzz textured class describes a material that reflects rays at the same angle around the randomly disturbed surface normal, to model glossy reflection. Its reflective
     * colour is taken from a texture.
     *
     * This material has an emissive colour and a reflective texture, applied to rays on bounce.
     * The reflective colour is determined on bounce based on the object space coordinates of
     * the bounce, and fetched from the texture.
     * The rays are reflected with the same angle between the outgoing ray and the surface normal
     * as between the incident ray and the surface normal, but on he opposite side of the normal.
     * The normal has a random disruption applied to it to move it within its hemisphere.
     * This models glossy reflection, which reflect light in a cone around its normal.
     * This material represents non-uniformly coloured imperfect shiny surfaces, such as painted
     * brushed metals, frosted stained mirrors, and rough shiny coloured plastic.
     */
    class ReflectiveFuzzTex_t final : public Entities::Material_t {
        public:
            /**
             * @brief Construct a new ReflectiveFuzzTex_t object with an emissive colour, a reflective texture, the order of the curve describing the distribution of the surface normal, and a
             * diffusivity factor.
             *
             * @param emission Colour emitted by the material when a ray bounces on it.
             * @param texture Texture containing the colour reflected by the material when a ray bounces on it.
             * @param order Order of the curve describing the distribution of the surface normal, from 0 to inf. 0 is forced to the edge of the radius described by the diffusivity. Between 0 and 1 is
             * a meniscus shape, 1 is flat, 2 is parabole, higher looks like normal and inf is straight reflection.
             * @param diffusivity Portion of the hemisphere around the normal in which the normal can be randomly moved, from 0 to 1. 0 is straight reflection, 1 is all the hemisphere.
             */
            ReflectiveFuzzTex_t(const Entities::Vec3f& emission, Entities::Texture_t* texture, double order, double diffusivity);

            Entities::Texture_t* texture_; /**< @brief Texture containing the colour reflected by the material, mapped by object space coordinates.*/
            Entities::Vec3f emission_; /**< @brief Colour emitted by the material at each bounce.*/
            double order_; /**< @brief Order of the curve describing the distribution of the surface normal, from 0 to inf. 0 is forced to the edge of the radius described by the diffusivity. Between
                              0 and 1 is a meniscus shape, 1 is flat, 2 is parabole, higher looks like normal and inf is straight reflection.*/
            double diffusivity_; /**< @brief Portion of the hemisphere around the normal in which the normal can be randomly moved, from 0 to 1. 0 is straight reflection, 1 is all the hemisphere.*/
            std::uniform_real_distribution<double> unif_; /**< @brief Uniform random distribution used for generating random numbers.*/

            /**
             * @brief Bounces a ray of light on the material.
             *
             * The reflective colour of the material at the hit point is determined by reading the texture at the
             * hit point, in object coordinates.
             * The ray's mask is attenuated with the material's colour to model part of the light being absorbed.
             * Then, the material's emissive colour is multiplied with the ray's mask and added to the ray's colour
             * to model light being emitted by the material.
             * The ray's origin is set to the hit point, and its direction is set with the same angle as between the
             * incident ray and the surface normal, but on he opposite side of the normal.
             * The normal has a random disruption applied to it to move it within a portion of its hemisphere, with
             * the portion being represented by the diffusivity parameter, and the probability distribution within
             * the portion being governed by the order parameter.
             * This is a straight reflection, to model glossy reflection.
             *
             * @param uv Object space coordinates of the hit point. Used to query the shape for values at coordinates on it. Two components, u, and v, that can change meaning depending on the shape.
             * @param hit_obj Pointer to the shape that was hit by the ray.
             * @param ray Ray that has intersected the shape.
             */
            auto bounce(std::array<double, 2> uv, const Entities::Shape_t* hit_obj, Entities::Ray_t& ray) -> void final;
    };
}}
#endif
