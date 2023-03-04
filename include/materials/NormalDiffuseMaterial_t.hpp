#ifndef APTRACER_MATERIALS_NORMALDIFFUSEMATERIAL_T_HPP
#define APTRACER_MATERIALS_NORMALDIFFUSEMATERIAL_T_HPP

#include "entities/Material_t.hpp"
#include "entities/Ray_t.hpp"
#include <random>

namespace APTracer { namespace Entities {
    class Shape_t;
}}

namespace APTracer { namespace Materials {

    /**
     * @brief The normal diffuse material class describes a material whose colour is relative to the surface normal at the hit point.
     *
     * This material's reflective colour is proportional to the surface normal in world coordinates, x y and z being red green and blue, respectively.
     * The rays are reflected in a random direction in the hemisphere over the hit point.
     * This models diffuse reflection, which reflect light in all directions.
     * This material is more useful for debugging than for display purposes.
     */
    class NormalDiffuseMaterial_t final : public Entities::Material_t {
        public:
            /**
             * @brief Construct a new NormalDiffuseMaterial_t object with a roughness.
             *
             * @param roughness Attenuation of the colour as the angle between the incident ray and the surface normal increase. 0 to 1, 0 being very rough (no attenuation) and 1 being an ideal
             * diffusely reflective surface, obeying Lambert's cosine law.
             */
            explicit NormalDiffuseMaterial_t(double roughness);

            double roughness_; /**< @brief Attenuation of the colour as the angle between the incident ray and the surface normal increase. 0 to 1, 0 being very rough (no attenuation) and 1 being an
                                  ideal diffusely reflective surface, obeying Lambert's cosine law.*/
            std::uniform_real_distribution<double> unif_; /**< @brief Uniform random distribution used for generating random numbers.*/

            /**
             * @brief Bounces a ray of light on the material.
             *
             * The reflective colour is calculating according to the surface normal at the hit point, x y and z being red green and blue, respectively.
             * The ray's mask is attenuated with the material's colour to model part of the light being absorbed.
             * The ray's origin is set to the hit point, and is normal is randomly selected within the hemisphere
             * above the hit point to model diffuse reflection.
             *
             * @param uv Object space coordinates of the hit point. Used to query the shape for values at coordinates on it. Two components, u, and v, that can change meaning depending on the shape.
             * @param hit_obj Pointer to the shape that was hit by the ray.
             * @param ray Ray that has intersected the shape.
             */
            auto bounce(std::array<double, 2> uv, const Entities::Shape_t* hit_obj, Entities::Ray_t& ray) -> void final;
    };
}}
#endif
