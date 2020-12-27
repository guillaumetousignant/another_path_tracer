#ifndef APTRACER_NORMALMATERIAL_T_H
#define APTRACER_NORMALMATERIAL_T_H

#include "entities/Material_t.h"
#include "entities/Ray_t.h"

namespace APTracer { namespace Entities {
    class Shape_t;
}}

using APTracer::Entities::Material_t;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;

namespace APTracer { namespace Materials {
    class NormalMaterial_t final : public Material_t{
        public:
            NormalMaterial_t();
            virtual ~NormalMaterial_t() final;

            virtual void bounce(std::array<double, 2> uv, const Shape_t* hit_obj, Ray_t &ray) final;
    };
}}
#endif