#ifndef SKYBOXFLAT_T_H
#define SKYBOXFLAT_T_H

#include "entities/Skybox_t.h"
#include "entities/Vec3f.h"

using APTracer::Entities::Skybox_t;
using APTracer::Entities::Vec3f;

namespace APTracer::Skyboxes {
    class SkyboxFlat_t final : public Skybox_t{
        public:
            SkyboxFlat_t(const Vec3f &background);
            virtual ~SkyboxFlat_t() final;

            Vec3f background_;

            virtual Vec3f get(const Vec3f &xyz) const final;
    };
}
#endif