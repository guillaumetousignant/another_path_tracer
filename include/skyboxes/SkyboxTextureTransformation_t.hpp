#ifndef APTRACER_SKYBOXES_SKYBOXTEXTURETRANSFORMATION_T_HPP
#define APTRACER_SKYBOXES_SKYBOXTEXTURETRANSFORMATION_T_HPP

#include "entities/Skybox_t.hpp"
#include "entities/Texture_t.hpp"
#include "entities/TransformMatrix_t.hpp"
#include "entities/Vec3f.hpp"

namespace APTracer { namespace Skyboxes {
    /**
     * @brief The skybox texture transformation class describes texture coloured skybox that can be rotated.
     *
     * This will use an image as the background of the scene. The skybox can be rotated by modifying its
     * transformation matrix.
     */
    class SkyboxTextureTransformation_t final : public Entities::Skybox_t {
        public:
            /**
             * @brief Construct a new SkyboxTextureTransformation_t object from a texture.
             *
             * @param texture Texture used as the background of the skybox.
             * @param transformation Transformation matrix moving the skybox.
             */
            SkyboxTextureTransformation_t(Entities::Texture_t* texture, Entities::TransformMatrix_t* transformation);

            Entities::Texture_t* texture_; /**< @brief Texture used as the background of the skybox.*/
            Entities::TransformMatrix_t* transformation_; /**< @brief Transformation matrix moving the skybox.*/

            auto get(const Entities::Vec3f& xyz) const -> Entities::Vec3f final;
    };
}}
#endif