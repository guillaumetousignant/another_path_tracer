#ifndef APTRACER_SKYBOXES_SKYBOXTEXTURE_T_HPP
#define APTRACER_SKYBOXES_SKYBOXTEXTURE_T_HPP

#include "entities/Skybox_t.hpp"
#include "entities/Vec3f.hpp"

namespace APTracer { namespace Entities {
    class Texture_t;
}}

using APTracer::Entities::Skybox_t;
using APTracer::Entities::Texture_t;
using APTracer::Entities::Vec3f;

namespace APTracer { namespace Skyboxes {
    /**
     * @brief The skybox texture class describes texture coloured skybox.
     *
     * This will use an image as the background of the scene.
     */
    class SkyboxTexture_t final : public Skybox_t {
        public:
            /**
             * @brief Construct a new SkyboxTexture_t object from a texture.
             *
             * @param texture Texture used as the background of the skybox.
             */
            SkyboxTexture_t(Texture_t* texture);

            Texture_t* texture_; /**< @brief Texture used as the background of the skybox.*/

            virtual auto get(const Vec3f& xyz) const -> Vec3f final;
    };
}}
#endif