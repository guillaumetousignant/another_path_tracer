#ifndef APTRACER_SKYBOXES_SKYBOXES_HPP
#define APTRACER_SKYBOXES_SKYBOXES_HPP

namespace APTracer {
    /**
     * @brief Contains different skybox types that can be used.
     *
     * Skyboxes describe how the light get coloured when they miss geometry. It is the
     * background of scenes. They output the background colour depending on a ray's
     * direction. Several types exist, some taking their colour from a texture, a
     * flat colour, light sources, or a combination of these.
     */
    namespace Skyboxes {
    }
}

#include "SkyboxFlat_t.hpp"
#include "SkyboxFlatSun_t.hpp"
#include "SkyboxTexture_t.hpp"
#include "SkyboxTextureSun_t.hpp"
#include "SkyboxTextureTransformation_t.hpp"
#include "SkyboxTextureTransformationSun_t.hpp"

#endif
