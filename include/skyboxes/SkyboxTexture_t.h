#ifndef SKYBOXTEXTURE_T_H
#define SKYBOXTEXTURE_T_H

#include "entities/Skybox_t.h"
#include "entities/Vec3f.h"

namespace APTracer { namespace Entities {
    class Texture_t;
}}

using APTracer::Entities::Skybox_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Texture_t;

namespace APTracer { namespace Skyboxes {
    class SkyboxTexture_t final : public Skybox_t{
        public:
            SkyboxTexture_t(Texture_t* texture);
            virtual ~SkyboxTexture_t() final;

            Texture_t* texture_;

            virtual Vec3f get(const Vec3f &xyz) const final;
    };
}}
#endif