#ifndef APTRACER_ENTITIES_SKYBOX_T_H
#define APTRACER_ENTITIES_SKYBOX_T_H

#include "entities/Vec3f.h"

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
             * @brief Returns the colour of the skybox for a given direction.
             *
             * @param xyz Direction in cartesian coordinates for which we want the colour.
             * @return Vec3f Colour of the skybox for the given direction.
             */
            virtual auto get(const Vec3f& xyz) const -> Vec3f = 0;
    };
}}

#endif