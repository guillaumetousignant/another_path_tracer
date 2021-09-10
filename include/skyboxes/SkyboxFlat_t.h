#ifndef APTRACER_SKYBOXES_SKYBOXFLAT_T_H
#define APTRACER_SKYBOXES_SKYBOXFLAT_T_H

#include "entities/Skybox_t.h"
#include "entities/Vec3f.h"

using APTracer::Entities::Skybox_t;
using APTracer::Entities::Vec3f;

namespace APTracer { namespace Skyboxes {
    /**
     * @brief The skybox flat class describes a flat coloured skybox.
     * 
     * The colour returned is independent of direction, and is 'background_'.
     */
    class SkyboxFlat_t final : public Skybox_t {
        public:
            /**
             * @brief Construct a new SkyboxFlat_t object of the provided colour.
             * 
             * @param background Colour of the skybox.
             */
            SkyboxFlat_t(const Vec3f &background);

            Vec3f background_; /**< @brief Colour of the skybox, in all directions.*/

            virtual auto get(const Vec3f &xyz) const -> Vec3f final;
    };
}}
#endif