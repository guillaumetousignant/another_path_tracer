#ifndef APTRACER_SKYBOXES_SKYBOXTEXTURE_T_HPP
#define APTRACER_SKYBOXES_SKYBOXTEXTURE_T_HPP

#include "entities/Skybox_t.hpp"
#include "entities/Texture_t.hpp"
#include "entities/Vec3f.hpp"

namespace APTracer { namespace Skyboxes {
    /**
     * @brief The skybox texture class describes texture coloured skybox.
     *
     * This will use an image as the background of the scene.
     */
    class SkyboxTexture_t final : public Entities::Skybox_t {
        public:
            /**
             * @brief Construct a new SkyboxTexture_t object from a texture.
             *
             * @param texture Texture used as the background of the skybox.
             */
            explicit SkyboxTexture_t(Entities::Texture_t* texture);

            Entities::Texture_t* texture_; /**< @brief Texture used as the background of the skybox.*/

            auto get(const Entities::Vec3f& xyz) const -> Entities::Vec3f final;
    };
}}
#endif