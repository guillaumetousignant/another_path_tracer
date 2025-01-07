#ifndef APTRACER_ENTITIES_MATERIAL_T_HPP
#define APTRACER_ENTITIES_MATERIAL_T_HPP

#include "entities/Ray_t.hpp"
#include <array>

namespace APTracer { namespace Entities {
    class Shape_t;

    /**
     * @brief The material class describes how light interacts with a specific material.
     *
     * This is an interface class that define the core function a material must implement.
     * A material modified a ray of light when it bounces on it. It can change the ray's
     * direction and origin, to model the light path change from the intersection. It can
     * also change the ray's colour and mask to model light absorption and emission.
     */
    class Material_t {
        public:
            /**
             * @brief Destroy the Material_t object.
             */
            virtual ~Material_t() = default;

            /**
             * @brief Construct a new Material_t object.
             */
            Material_t() = default;

            /**
             * @brief Copy construct a new Material_t object.
             *
             * @param other Object to copy.
             */
            Material_t(const Material_t& other) = default;

            /**
             * @brief Move construct a new Material_t object.
             *
             * @param other Object to move.
             */
            Material_t(Material_t&& other) noexcept = default;

            /**
             * @brief Copy assignment.
             *
             * @param other Object to copy.
             * @return Material_t& Reference to this object.
             */
            auto operator=(const Material_t& other) -> Material_t& = default;

            /**
             * @brief Move assignment.
             *
             * @param other Object to move.
             * @return Material_t& Reference to this object.
             */
            auto operator=(Material_t&& other) noexcept -> Material_t& = default;

            /**
             * @brief Bounces a ray of light on the material.
             *
             * This is the main function to resolve intersection between a ray and a shape.
             * This is how rays get coloured and change direction after an intersection in the scene.
             * This sets the ray's new colour, mask, direction and origin.
             *
             * @param uv Shape space coordinates of the hit point. Used to query the shape for values at coordinates on it. Two components, u, and v, that can change meaning depending on the shape.
             * @param hit_obj Pointer to the shape that was hit by the ray.
             * @param ray Ray that has intersected the shape.
             */
            virtual auto bounce(std::array<double, 2> uv, const Shape_t* hit_obj, Ray_t& ray) -> void = 0;
    };
}}

#endif
