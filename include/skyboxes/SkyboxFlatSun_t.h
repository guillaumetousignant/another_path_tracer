#ifndef SKYBOXFLATSUN_T_H
#define SKYBOXFLATSUN_T_H

#include "entities/Skybox_t.h"
#include "entities/Vec3f.h"

namespace APTracer { namespace Entities {
    class DirectionalLight_t;
}}

using APTracer::Entities::Skybox_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::DirectionalLight_t;

namespace APTracer { namespace Skyboxes {
    class SkyboxFlatSun_t final : public Skybox_t{
        public:
            SkyboxFlatSun_t(const Vec3f &background, DirectionalLight_t* light);
            SkyboxFlatSun_t(const Vec3f &background, DirectionalLight_t** lights, unsigned int n_lights);
            virtual ~SkyboxFlatSun_t() final;

            Vec3f background_;
            DirectionalLight_t** lights_;
            unsigned int n_lights_;

            virtual Vec3f get(const Vec3f &xyz) const final;
    };
}}
#endif