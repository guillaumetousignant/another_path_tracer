#ifndef SKYBOXTEXTURESUN_T_H
#define SKYBOXTEXTURESUN_T_H

#include "entities/Skybox_t.h"
#include "entities/Vec3f.h"

namespace APTracer { namespace Entities {
    class Texture_t;
}}

using APTracer::Entities::Skybox_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Texture_t;

namespace APTracer { namespace Skyboxes {
    class SkyboxTextureSun_t final : public Skybox_t{
        public:
            SkyboxTextureSun_t(Texture_t* texture, const double (&sun_pos)[2], Vec3f sun_col, double sun_rad);
            virtual ~SkyboxTextureSun_t() final;

            Texture_t* texture_;
            double sun_pos_[2];
            Vec3f sun_col_;
            double sun_rad_;

            virtual Vec3f get(const Vec3f &xyz) const final;
    };
}}
#endif