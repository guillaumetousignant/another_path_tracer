#ifndef APTRACER_TRANSPARENT_T_H
#define APTRACER_TRANSPARENT_T_H

#include "entities/Material_t.h"
#include "entities/Vec3f.h"
#include "entities/Ray_t.h"

namespace APTracer { namespace Entities {
    class Medium_t;
    class Shape_t;
}}

using APTracer::Entities::Material_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;
using APTracer::Entities::Medium_t;

namespace APTracer { namespace Materials {
    class Transparent_t final : public Material_t {
        public:
            Transparent_t(Medium_t* scattering);
            virtual ~Transparent_t() final;

            Medium_t* medium_;

            virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) final;
    };
}}
#endif