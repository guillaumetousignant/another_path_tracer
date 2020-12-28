#ifndef APTRACER_REFLECTIVE_T_H
#define APTRACER_REFLECTIVE_T_H

#include "entities/Material_t.h"
#include "entities/Vec3f.h"
#include "entities/Ray_t.h"

namespace APTracer { namespace Entities {
    class Shape_t;
}}

using APTracer::Entities::Material_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;

namespace APTracer { namespace Materials {
    class Reflective_t final : public Material_t{
        public:
            Reflective_t(const Vec3f &emission, const Vec3f &colour);
            virtual ~Reflective_t() final;

            Vec3f emission_;
            Vec3f colour_;

            virtual void bounce(std::array<double, 2> uv, const Shape_t* hit_obj, Ray_t &ray) final;
    };
}}
#endif