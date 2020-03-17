#ifndef NORMALMATERIAL_T_H
#define NORMALMATERIAL_T_H

#include "Material_t.h"

using APTracer::Entities::Material_t;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;

class Shape_t;

namespace APTracer::Materials {
    class NormalMaterial_t final : public Material_t{
        public:
            NormalMaterial_t();
            virtual ~NormalMaterial_t() final;

            virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) final;
    };
}
#endif