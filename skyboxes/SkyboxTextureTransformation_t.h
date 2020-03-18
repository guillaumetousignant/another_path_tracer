#ifndef SKYBOXTEXTURETRANSFORMATION_T_H
#define SKYBOXTEXTURETRANSFORMATION_T_H

#include "Skybox_t.h"
#include "Vec3f.h"

namespace APTracer::Entities {
    class Texture_t;
    class TransformMatrix_t;
}

using APTracer::Entities::Skybox_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Texture_t;
using APTracer::Entities::TransformMatrix_t;

namespace APTracer::Skyboxes {
    class SkyboxTextureTransformation_t final : public Skybox_t{
        public:
            SkyboxTextureTransformation_t(Texture_t* texture, TransformMatrix_t* transformation);
            virtual ~SkyboxTextureTransformation_t() final;

            Texture_t* texture_;
            TransformMatrix_t* transformation_;
            
            virtual Vec3f get(const Vec3f &xyz) const final;
    };
}
#endif