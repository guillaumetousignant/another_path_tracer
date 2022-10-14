#ifndef APTRACER_SKYBOXES_SKYBOXTEXTURETRANSFORMATIONSUN_T_H
#define APTRACER_SKYBOXES_SKYBOXTEXTURETRANSFORMATIONSUN_T_H

#include "entities/Skybox_t.h"
#include "entities/Vec3f.h"
#include <array>

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
     * @brief The skybox texture transformation sun class describes texture coloured skybox that can be rotated, with an additional single light source.
     *
     * This will use an image as the background of the scene, and return the sun colour if inside its radius. The skybox can be rotated by modifying its
     * transformation matrix.
     */
    class SkyboxTextureTransformationSun_t final : public Skybox_t {
        public:
            /**
             * @brief Construct a new SkyboxTextureTransformationSun_t object from a texture and a light source.
             *
             * @param texture Texture used as the background of the skybox.
             * @param transformation Transformation matrix moving the skybox.
             * @param sun_pos Position of the light source in normalized coordinates on the image, [x, y] both from 0 to 1, from bottom left corner.
             * @param sun_col Colour of the light source. Can be above [1, 1, 1].
             * @param sun_rad Radius of the light source in normalized coordinates, from 0 to sqrt(2). sqrt(2) is max distance in 2D space of [0..1, 0..1].
             */
            SkyboxTextureTransformationSun_t(Texture_t* texture, TransformMatrix_t* transformation, std::array<double, 2> sun_pos, Vec3f sun_col, double sun_rad);

            Texture_t* texture_; /**< @brief Texture used as the background of the skybox.*/
            TransformMatrix_t* transformation_; /**< @brief Transformation matrix moving the skybox.*/
            std::array<double, 2> sun_pos_; /**< @brief TPosition of the light source in normalized coordinates on the image, [x, y] both from 0 to 1, from bottom left corner.*/
            Vec3f sun_col_; /**< @brief Colour of the light source. Can be above [1, 1, 1].*/
            double sun_rad_; /**< @brief Radius of the light source in normalized coordinates, from 0 to sqrt(2). sqrt(2) is max distance in 2D space of [0..1, 0..1].*/

            virtual auto get(const Vec3f& xyz) const -> Vec3f final;
    };
}}
#endif