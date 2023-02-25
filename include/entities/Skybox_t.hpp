#ifndef APTRACER_ENTITIES_SKYBOX_T_HPP
#define APTRACER_ENTITIES_SKYBOX_T_HPP

#include "entities/Vec3f.hpp"

namespace APTracer { namespace Entities {

    /**
     * @brief Skyboxes represent the infinitely far background of a scene. Define how the ray gets coloured when it doesn't intersect geometry.
     *
     * Skyboxes return the colour of the background based on ray direction only. Using only the direction places the skybox infinitely far.
     * The colour returned is used as an emissive colour, and thus lights up the scene as a colour source.
     */
    class Skybox_t {
        public:
            /**
             * @brief Destroy the Skybox_t object.
             */
            virtual ~Skybox_t() = default;

            /**
             * @brief Construct a new Skybox_t object.
             */
            Skybox_t() = default;

            /**
             * @brief Copy construct a new Skybox_t object.
             *
             * @param other Object to copy.
             */
            Skybox_t(const Skybox_t& other) = default;

            /**
             * @brief Move construct a new Skybox_t object.
             *
             * @param other Object to move.
             */
            Skybox_t(Skybox_t&& other) noexcept = default;

            /**
             * @brief Copy assignment.
             *
             * @param other Object to copy.
             * @return Skybox_t& Reference to this object.
             */
            auto operator=(const Skybox_t& other) -> Skybox_t& = default;

            /**
             * @brief Move assignment.
             *
             * @param other Object to move.
             * @return Skybox_t& Reference to this object.
             */
            auto operator=(Skybox_t&& other) noexcept -> Skybox_t& = default;

            /**
             * @brief Returns the colour of the skybox for a given direction.
             *
             * @param xyz Direction in cartesian coordinates for which we want the colour.
             * @return Vec3f Colour of the skybox for the given direction.
             */
            virtual auto get(const Vec3f& xyz) const -> Vec3f = 0;
    };
}}

#endif