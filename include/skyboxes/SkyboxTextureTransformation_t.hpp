#ifndef APTRACER_SKYBOXES_SKYBOXTEXTURETRANSFORMATION_T_HPP
#define APTRACER_SKYBOXES_SKYBOXTEXTURETRANSFORMATION_T_HPP

#include "entities/Skybox_t.hpp"
#include "entities/Vec3f.hpp"

namespace APTracer { namespace Entities {
    class Texture_t;
    class TransformMatrix_t;
}}

using APTracer::Entities::Skybox_t;
using APTracer::Entities::Texture_t;
using APTracer::Entities::TransformMatrix_t;
using APTracer::Entities::Vec3f;

namespace APTracer { namespace Skyboxes {
    /**
     * @brief The skybox texture transformation class describes texture coloured skybox that can be rotated.
     *
     * This will use an image as the background of the scene. The skybox can be rotated by modifying its
     * transformation matrix.
     */
    class SkyboxTextureTransformation_t final : public Skybox_t {
        public:
            /**
             * @brief Construct a new SkyboxTextureTransformation_t object from a texture.
             *
             * @param texture Texture used as the background of the skybox.
             * @param transformation Transformation matrix moving the skybox.
             */
            SkyboxTextureTransformation_t(Texture_t* texture, TransformMatrix_t* transformation);

            Texture_t* texture_; /**< @brief Texture used as the background of the skybox.*/
            TransformMatrix_t* transformation_; /**< @brief Transformation matrix moving the skybox.*/

            virtual auto get(const Vec3f& xyz) const -> Vec3f final;
    };
}}
#endif