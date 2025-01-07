#ifndef APTRACER_SKYBOXES_SKYBOXFLAT_T_HPP
#define APTRACER_SKYBOXES_SKYBOXFLAT_T_HPP

#include "entities/Skybox_t.hpp"
#include "entities/Vec3f.hpp"

namespace APTracer { namespace Skyboxes {
    /**
     * @brief The skybox flat class describes a flat coloured skybox.
     *
     * The colour returned is independent of direction, and is 'background_'.
     */
    class SkyboxFlat_t final : public Entities::Skybox_t {
        public:
            /**
             * @brief Construct a new SkyboxFlat_t object of the provided colour.
             *
             * @param background Colour of the skybox.
             */
            explicit SkyboxFlat_t(const Entities::Vec3f& background);

            Entities::Vec3f background_; /**< @brief Colour of the skybox, in all directions.*/

            auto get(const Entities::Vec3f& xyz) const -> Entities::Vec3f final;
    };
}}
#endif
