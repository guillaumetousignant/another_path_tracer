#ifndef APTRACER_DIFFUSE_T_H
#define APTRACER_DIFFUSE_T_H

#include "entities/Material_t.h"
#include "entities/Vec3f.h"
#include "entities/Ray_t.h"
#include <random>

namespace APTracer { namespace Entities {
    class Shape_t;
}}

using APTracer::Entities::Material_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;

namespace APTracer { namespace Materials {

    /**
     * @brief The diffuse class describes a material that reflects rays in random directions, to model diffuse reflection.
     * 
     * This material has an emissive and reflective colour, applied to rays on bounce.
     * The rays are reflected in a random direction in the hemisphere over the hit point.
     * This models diffuse reflection, which reflect light in all directions.
     * This material represents uniformly and finely rough surfaces, such as matte paint,
     * chalk, and non-shiny plastic.
     */
    class Diffuse_t final : public Material_t{
        public:
            /**
             * @brief Construct a new Diffuse_t object with an emissive and reflective colour, and a roughness.
             * 
             * @param emission Colour emitted by the material when a ray bounces on it.
             * @param colour Colour reflected by the material when a ray bounces on it.
             * @param roughness Attenuation of the colour as the angle between the incident ray and the surface normal increase. 0 to 1, 0 being very rough (no attenuation) and 1 being an ideal diffusely reflective surface, obeying Lambert's cosine law.
             */
            Diffuse_t(const Vec3f &emission, const Vec3f &colour, double roughness);

            /**
             * @brief Destroy the Diffuse_t object. Does nothing.
             */
            virtual ~Diffuse_t() final;

            Vec3f emission_; /**< @brief Colour emitted by the material at each bounce.*/
            Vec3f colour_; /**< @brief Colour reflected by the material at each bounce.*/
            double roughness_; /**< @brief Attenuation of the colour as the angle between the incident ray and the surface normal increase. 0 to 1, 0 being very rough (no attenuation) and 1 being an ideal diffusely reflective surface, obeying Lambert's cosine law.*/
            std::uniform_real_distribution<double> unif_; /**< @brief Uniform random distribution used for generating random numbers.*/

            /**
             * @brief Bounces a ray of light on the material.
             * 
             * The ray's mask is attenuated with the material's colour to model part of the light being absorbed.
             * Then, the material's emissive colour is multiplied with the ray's mask and added to the ray's colour
             * to model light being emitted bi the material.
             * The ray's origin is set to the hit point, and is normal is randomly selected within the hemisphere
             * above the hit point to model diffuse reflection.
             * 
             * @param uv Object space coordinates of the hit point. Used to query the shape for values at coordinates on it. Two components, u, and v, that can change meaning depending on the shape.
             * @param hit_obj Pointer to the shape that was hit by the ray.
             * @param ray Ray that has intersected the shape.
             */
            virtual void bounce(std::array<double, 2> uv, const Shape_t* hit_obj, Ray_t &ray) final;
    };
}}
#endif