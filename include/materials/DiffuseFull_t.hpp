#ifndef APTRACER_MATERIALS_DIFFUSEFULL_T_HPP
#define APTRACER_MATERIALS_DIFFUSEFULL_T_HPP

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
     * @brief The diffuse class describes a material that reflects rays in random directions, to model diffuse reflection. Its reflective and emissive colours are taken from textures.
     *
     * This material has an emissive and reflective texture. The colours are determined on bounce
     * based on the object space coordinates of the bounce, and fetched from the textures.
     * The rays are reflected in a random direction in the hemisphere over the hit point.
     * This models diffuse reflection, which reflect light in all directions.
     * This material represents uniformly and finely rough surfaces, such as matte paint,
     * chalk, and non-shiny plastic.
     */
    class DiffuseFull_t final : public Entities::Material_t {
        public:
            /**
             * @brief Construct a new DiffuseFull_t object with an emissive and reflective texture, and a roughness.
             *
             * @param emission_map Texture containing the colour emitted by the material when a ray bounces on it.
             * @param texture Texture containing the colour reflected by the material when a ray bounces on it.
             * @param roughness Attenuation of the colour as the angle between the incident ray and the surface normal increase. 0 to 1, 0 being very rough (no attenuation) and 1 being an ideal
             * diffusely reflective surface, obeying Lambert's cosine law.
             */
            DiffuseFull_t(const Entities::Texture_t* emission_map, const Entities::Texture_t* texture, double roughness);

            const Entities::Texture_t* emission_map_; /**< @brief Texture containing the colour emitted by the material, mapped by object space coordinates.*/
            const Entities::Texture_t* texture_; /**< @brief Texture containing the colour reflected by the material, mapped by object space coordinates.*/
            double roughness_; /**< @brief Attenuation of the colour as the angle between the incident ray and the surface normal increase. 0 to 1, 0 being very rough (no attenuation) and 1 being an
                                  ideal diffusely reflective surface, obeying Lambert's cosine law.*/
            std::uniform_real_distribution<double> unif_; /**< @brief Uniform random distribution used for generating random numbers.*/

            /**
             * @brief Bounces a ray of light on the material.
             *
             * The colour and emissive colour of the material at the hit point are determined by reading the
             * textures at the hit point, in object coordinates.
             * The ray's mask is attenuated with the material's colour to model part of the light being absorbed.
             * Then, the material's emissive colour is multiplied with the ray's mask and added to the ray's colour
             * to model light being emitted bi the material.
             * The ray's origin is set to the hit point, and its direction is randomly selected within the hemisphere
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
