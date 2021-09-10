#ifndef APTRACER_MATERIALS_REFLECTIVENORMAL_T_H
#define APTRACER_MATERIALS_REFLECTIVENORMAL_T_H

#include "entities/Material_t.h"
#include "entities/Vec3f.h"
#include "entities/Ray_t.h"

namespace APTracer { namespace Entities {
    class Texture_t;
    class Shape_t;
}}

using APTracer::Entities::Material_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;
using APTracer::Entities::Texture_t;

namespace APTracer { namespace Materials {

    /**
     * @brief The reflective normal class describes a material that reflects rays at the same angle around the surface normal taken from a normal map, to model specular reflection.
     * 
     * This material has an emissive and reflective colour, applied to rays on bounce.
     * The rays are reflected with the same angle between the outgoing ray and the surface normal
     * as between the incident ray and the surface normal, but on he opposite side of the normal.
     * The normal is fetched from a normal map based on the object space coordinates of the bounce.
     * This models specular reflection, which reflect light in a single direction.
     * This material represents textured shiny surfaces, such as sculpted mirrors,
     * very polished patterned metals, and shiny shaped plastic.
     */
    class ReflectiveNormal_t final : public Material_t {
        public:
            /**
             * @brief Construct a new ReflectiveNormal_t object with an emissive and reflective colour, and a normal map.
             * 
             * @param emission Colour emitted by the material when a ray bounces on it.
             * @param colour Colour reflected by the material when a ray bounces on it.
             * @param normal_map Texture containing the surface normal in object coordinates.
             */
            ReflectiveNormal_t(const Vec3f &emission, const Vec3f &colour, const Texture_t* normal_map);

            Vec3f emission_; /**< @brief Colour emitted by the material at each bounce.*/
            Vec3f colour_; /**< @brief Colour reflected by the material at each bounce.*/
            const Texture_t* normal_map_; /**< @brief Texture containing the surface normal of the material in object coordinates, mapped by object space coordinates.*/

            /**
             * @brief Bounces a ray of light on the material.
             * 
             * The ray's mask is attenuated with the material's colour to model part of the light being absorbed.
             * Then, the material's emissive colour is multiplied with the ray's mask and added to the ray's colour
             * to model light being emitted by the material.
             * The ray's origin is set to the hit point, and its direction is set with the same angle as between the 
             * incident ray and the surface normal, but on he opposite side of the normal. The normal of the material 
             * at the hit point is determined by reading the texture at the hit point, in object coordinates.
             * This is a straight reflection, to model specular reflection.
             * 
             * @param uv Object space coordinates of the hit point. Used to query the shape for values at coordinates on it. Two components, u, and v, that can change meaning depending on the shape.
             * @param hit_obj Pointer to the shape that was hit by the ray.
             * @param ray Ray that has intersected the shape.
             */
            virtual auto bounce(std::array<double, 2> uv, const Shape_t* hit_obj, Ray_t &ray) -> void final;
    };
}}
#endif