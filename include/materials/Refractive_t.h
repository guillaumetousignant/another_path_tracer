#ifndef APTRACER_REFRACTIVE_T_H
#define APTRACER_REFRACTIVE_T_H

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
    class Refractive_t final : public Material_t{
        public:
            Refractive_t(const Vec3f &emission, const Vec3f &colour, Medium_t* medium);
            virtual ~Refractive_t() final;

            Vec3f emission_;
            Vec3f colour_;
            Medium_t* medium_;

            virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) final;
    };
}}
#endif