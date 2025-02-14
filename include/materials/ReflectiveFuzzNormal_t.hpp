#ifndef APTRACER_MATERIALS_REFLECTIVEFUZZNORMAL_T_HPP
#define APTRACER_MATERIALS_REFLECTIVEFUZZNORMAL_T_HPP

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
     * @brief The reflective fuzz normal class describes a material that reflects rays at the same angle around the randomly disturbed surface normal taken from a normal map, to model glossy
     * reflection.
     *
     * This material has an emissive and reflective colour, applied to rays on bounce.
     * The rays are reflected with the same angle between the outgoing ray and the surface normal
     * as between the incident ray and the surface normal, but on he opposite side of the normal.
     * The normal is fetched from a normal map based on the object space coordinates of the bounce.
     * The normal has a random disruption applied to it to move it within its hemisphere.
     * This models glossy reflection, which reflect light in a cone around its normal.
     * This material represents textured imperfect shiny surfaces, such as patterned brushed metals,
     * sculpted frosted mirrors, and rough shiny shaped plastic.
     */
    class ReflectiveFuzzNormal_t final : public Entities::Material_t {
        public:
            /**
             * @brief Construct a new ReflectiveFuzzNormal_t object with an emissive and reflective colour, a normal map, the order of the curve describing the distribution of the surface normal, and
             * a diffusivity factor.
             *
             * @param emission Colour emitted by the material when a ray bounces on it.
             * @param colour Colour reflected by the material when a ray bounces on it.
             * @param normal_map Texture containing the surface normal in object coordinates.
             * @param order Order of the curve describing the distribution of the surface normal, from 0 to inf. 0 is forced to the edge of the radius described by the diffusivity. Between 0 and 1 is
             * a meniscus shape, 1 is flat, 2 is parabole, higher looks like normal and inf is straight reflection.
             * @param diffusivity Portion of the hemisphere around the normal in which the normal can be randomly moved, from 0 to 1. 0 is straight reflection, 1 is all the hemisphere.
             */
            ReflectiveFuzzNormal_t(const Entities::Vec3f& emission, const Entities::Vec3f& colour, const Entities::Texture_t* normal_map, double order, double diffusivity);

            Entities::Vec3f emission_; /**< @brief Colour emitted by the material at each bounce.*/
            Entities::Vec3f colour_; /**< @brief Colour reflected by the material at each bounce.*/
            const Entities::Texture_t* normal_map_; /**< @brief Texture containing the surface normal of the material in object coordinates, mapped by object space coordinates.*/
            double order_; /**< @brief Order of the curve describing the distribution of the surface normal, from 0 to inf. 0 is forced to the edge of the radius described by the diffusivity. Between
                              0 and 1 is a meniscus shape, 1 is flat, 2 is parabole, higher looks like normal and inf is straight reflection.*/
            double diffusivity_; /**< @brief Portion of the hemisphere around the normal in which the normal can be randomly moved, from 0 to 1. 0 is straight reflection, 1 is all the hemisphere.*/
            std::uniform_real_distribution<double> unif_; /**< @brief Uniform random distribution used for generating random numbers.*/

            /**
             * @brief Bounces a ray of light on the material.
             *
             * The ray's mask is attenuated with the material's colour to model part of the light being absorbed.
             * Then, the material's emissive colour is multiplied with the ray's mask and added to the ray's colour
             * to model light being emitted by the material.
             * The ray's origin is set to the hit point, and its direction is set with the same angle as between the
             * incident ray and the surface normal, but on he opposite side of the normal.
             * The normal has a random disruption applied to it to move it within a portion of its hemisphere, with
             * the portion being represented by the diffusivity parameter, and the probability distribution within
             * the portion being governed by the order parameter.
             * The normal of the material at the hit point is determined by reading the texture at the hit point,
             * in object coordinates.
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
