#ifndef APTRACER_SKYBOXES_SKYBOXFLATSUN_T_HPP
#define APTRACER_SKYBOXES_SKYBOXFLATSUN_T_HPP

#include "entities/DirectionalLight_t.hpp"
#include "entities/Skybox_t.hpp"
#include "entities/Vec3f.hpp"
#include <vector>

namespace APTracer { namespace Skyboxes {
    /**
     * @brief The skybox flat sun class describes a flat coloured skybox with lights, such as a sun.
     *
     * The colour returned is independent of direction, and is 'background_', unless it falls in the radius of a light, in which case
     * the colour of the light is returned.
     */
    class SkyboxFlatSun_t final : public Entities::Skybox_t {
        public:
            /**
             * @brief Construct a new SkyboxFlatSun_t object of the provided colour with one light.
             *
             * @param background Background colour of the skybox.
             * @param light Directional light used for the skybox.
             */
            SkyboxFlatSun_t(const Entities::Vec3f& background, Entities::DirectionalLight_t* light);

            /**
             * @brief Construct a new SkyboxFlatSun_t object of the provided colour with several lights.
             *
             * @param background Background colour of the skybox.
             * @param lights Array of directional lights used for the skybox.
             */
            SkyboxFlatSun_t(const Entities::Vec3f& background, std::vector<Entities::DirectionalLight_t*> lights);

            Entities::Vec3f background_; /**< @brief Colour of the skybox if a light is not hit.*/
            std::vector<Entities::DirectionalLight_t*> lights_; /**< @brief Array of directional lights in the skybox.*/

            auto get(const Entities::Vec3f& xyz) const -> Entities::Vec3f final;
    };
}}
#endif