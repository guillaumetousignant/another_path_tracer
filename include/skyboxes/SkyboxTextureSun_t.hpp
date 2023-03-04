#ifndef APTRACER_SKYBOXES_SKYBOXTEXTURESUN_T_HPP
#define APTRACER_SKYBOXES_SKYBOXTEXTURESUN_T_HPP

#include "entities/Skybox_t.hpp"
#include "entities/Texture_t.hpp"
#include "entities/Vec3f.hpp"
#include <array>

namespace APTracer { namespace Skyboxes {
    /**
     * @brief The skybox texture sun class describes texture coloured skybox with an additional single light source.
     *
     * This will use an image as the background of the scene, and return the sun colour if inside its radius.
     */
    class SkyboxTextureSun_t final : public Entities::Skybox_t {
        public:
            /**
             * @brief Construct a new SkyboxTextureSun_t object from a texture and a light source.
             *
             * @param texture Texture used as the background of the skybox.
             * @param sun_pos Position of the light source in normalized coordinates on the image, [x, y] both from 0 to 1, from bottom left corner.
             * @param sun_col Colour of the light source. Can be above [1, 1, 1].
             * @param sun_rad Radius of the light source in normalized coordinates, from 0 to sqrt(2). sqrt(2) is max distance in 2D space of [0..1, 0..1].
             */
            SkyboxTextureSun_t(Entities::Texture_t* texture, std::array<double, 2> sun_pos, Entities::Vec3f sun_col, double sun_rad);

            Entities::Texture_t* texture_; /**< @brief Texture used as the background of the skybox.*/
            std::array<double, 2> sun_pos_; /**< @brief Position of the light source in normalized coordinates on the image, [x, y] both from 0 to 1, from bottom left corner.*/
            Entities::Vec3f sun_col_; /**< @brief Colour of the light source. Can be above [1, 1, 1].*/
            double sun_rad_; /**< @brief Radius of the light source in normalized coordinates, from 0 to sqrt(2). sqrt(2) is max distance in 2D space of [0..1, 0..1].*/

            auto get(const Entities::Vec3f& xyz) const -> Entities::Vec3f final;
    };
}}
#endif
