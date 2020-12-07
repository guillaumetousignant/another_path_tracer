#ifndef APTRACER_SKYBOXFLATSUN_T_H
#define APTRACER_SKYBOXFLATSUN_T_H

#include "entities/Skybox_t.h"
#include "entities/Vec3f.h"

namespace APTracer { namespace Entities {
    class DirectionalLight_t;
}}

using APTracer::Entities::Skybox_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::DirectionalLight_t;

namespace APTracer { namespace Skyboxes {
    /**
     * @brief The skybox flat sun class describes a flat coloured skybox with lights, such as a sun.
     * 
     * The colour returned is independent of direction, and is 'background_', unless it falls in the radius of a light, in which case
     * the colour of the light is returned.
     */
    class SkyboxFlatSun_t final : public Skybox_t{
        public:
            /**
             * @brief Construct a new SkyboxFlatSun_t object of the provided colour with one light.
             * 
             * @param background Background colour of the skybox.
             * @param light Directional light used for the skybox.
             */
            SkyboxFlatSun_t(const Vec3f &background, DirectionalLight_t* light);

            /**
             * @brief Construct a new SkyboxFlatSun_t object of the provided colour with several lights.
             * 
             * @param background Background colour of the skybox.
             * @param lights Array of directional lights used for the skybox.
             * @param n_lights Number of directional lights.
             */
            SkyboxFlatSun_t(const Vec3f &background, DirectionalLight_t** lights, size_t n_lights);

            /**
             * @brief Destroy the SkyboxFlatSun_t object, freeing the light array it holds.
             */
            virtual ~SkyboxFlatSun_t() final;

            Vec3f background_; /**< @brief Colour of the skybox if a light is not hit.*/
            DirectionalLight_t** lights_; /**< @brief Array of directional lights in the skybox.*/
            size_t n_lights_; /**< @brief Number of directional lights in the skybox.*/

            virtual Vec3f get(const Vec3f &xyz) const final;
    };
}}
#endif