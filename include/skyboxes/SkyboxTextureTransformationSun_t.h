#ifndef SKYBOXTEXTURETRANSFORMATIONSUN_T_H
#define SKYBOXTEXTURETRANSFORMATIONSUN_T_H

#include "entities/Skybox_t.h"
#include "entities/Vec3f.h"

namespace APTracer { namespace Entities {
    class Texture_t;
    class TransformMatrix_t;
}}

using APTracer::Entities::Skybox_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Texture_t;
using APTracer::Entities::TransformMatrix_t;

namespace APTracer { namespace Skyboxes {
    class SkyboxTextureTransformationSun_t final : public Skybox_t{
        public:
            SkyboxTextureTransformationSun_t(Texture_t* texture, TransformMatrix_t* transformation, const double (&sun_pos)[2], Vec3f sun_col, double sun_rad);
            virtual ~SkyboxTextureTransformationSun_t() final;

            Texture_t* texture_;
            TransformMatrix_t* transformation_;
            double sun_pos_[2];
            Vec3f sun_col_;
            double sun_rad_;
            
            virtual Vec3f get(const Vec3f &xyz) const final;
    };
}}
#endif