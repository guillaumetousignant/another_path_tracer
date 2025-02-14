#ifndef APTRACER_MATERIALS_DIFFUSENORMAL_T_HPP
#define APTRACER_MATERIALS_DIFFUSENORMAL_T_HPP

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
     * @brief The diffuse normal class describes a material that reflects rays in random directions around the surface normal taken from a normal map, to model diffuse reflection.
     *
     * This material has an emissive and reflective colour, applied to rays on bounce.
     * The rays are reflected in a random direction in the hemisphere over the hit point.
     * That hemisphere is centered on the surface normal, which is fetched from a normal map
     * based on the object space coordinates of the bounce.
     * This models diffuse reflection, which reflect light in all directions.
     * This material represents uniformly and finely rough surfaces that are not flat,
     * such as brick, textured clay, and wood.
     */
    class DiffuseNormal_t final : public Entities::Material_t {
        public:
            /**
             * @brief Construct a new DiffuseNormal_t object with an emissive and reflective colour, a roughness and a normal map.
             *
             * @param emission Colour emitted by the material when a ray bounces on it.
             * @param colour Colour reflected by the material when a ray bounces on it.
             * @param normal_map Texture containing the surface normal in object coordinates.
             * @param roughness Attenuation of the colour as the angle between the incident ray and the surface normal increase. 0 to 1, 0 being very rough (no attenuation) and 1 being an ideal
             * diffusely reflective surface, obeying Lambert's cosine law.
             */
            DiffuseNormal_t(const Entities::Vec3f& emission, const Entities::Vec3f& colour, const Entities::Texture_t* normal_map, double roughness);

            Entities::Vec3f emission_; /**< @brief Colour emitted by the material at each bounce.*/
            Entities::Vec3f colour_; /**< @brief Colour reflected by the material at each bounce.*/
            const Entities::Texture_t* normal_map_; /**< @brief Texture containing the surface normal of the material in object coordinates, mapped by object space coordinates.*/
            double roughness_; /**< @brief Attenuation of the colour as the angle between the incident ray and the surface normal increase. 0 to 1, 0 being very rough (no attenuation) and 1 being an
                                  ideal diffusely reflective surface, obeying Lambert's cosine law.*/
            std::uniform_real_distribution<double> unif_; /**< @brief Uniform random distribution used for generating random numbers.*/

            /**
             * @brief Bounces a ray of light on the material.
             *
             * The ray's mask is attenuated with the material's colour to model part of the light being absorbed.
             * Then, the material's emissive colour is multiplied with the ray's mask and added to the ray's colour
             * to model light being emitted by the material.
             * The ray's origin is set to the hit point, and is normal is randomly selected within the hemisphere
             * around the normal to model diffuse reflection. The normal of the material at the hit point is
             * determined by reading the texture at the hit point, in object coordinates.
             *
             * @param uv Object space coordinates of the hit point. Used to query the shape for values at coordinates on it. Two components, u, and v, that can change meaning depending on the shape.
             * @param hit_obj Pointer to the shape that was hit by the ray.
             * @param ray Ray that has intersected the shape.
             */
            auto bounce(std::array<double, 2> uv, const Entities::Shape_t* hit_obj, Entities::Ray_t& ray) -> void final;
    };
}}
#endif
