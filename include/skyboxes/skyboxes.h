#ifndef APTRACER_SKYBOXES_H
#define APTRACER_SKYBOXES_H

namespace APTracer { 
    /**
     * @brief Contains different skybox types that can be used.
     * 
     * Skyboxes describe how the light get coloured when they miss geometry. It is the 
     * background of scenes. They output the background colour depending on a ray's
     * direction. Several types exist, some taking their colour from a texture, a 
     * flat colour, light sources, or a combination of these.
     */
    namespace Skyboxes {}
}

#include "SkyboxFlat_t.h"
#include "SkyboxFlatSun_t.h"
#include "SkyboxTexture_t.h"
#include "SkyboxTextureSun_t.h"
#include "SkyboxTextureTransformation_t.h"
#include "SkyboxTextureTransformationSun_t.h"

#endif