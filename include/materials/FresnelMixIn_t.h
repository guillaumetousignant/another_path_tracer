#ifndef FRESNELMIXIN_T_H
#define FRESNELMIXIN_T_H

#include "entities/MaterialMix_t.h"
#include "entities/Vec3f.h"
#include "entities/Ray_t.h"
#include <random>

namespace APTracer { namespace Entities {
    class Material_t;
    class Shape_t;
}}

using APTracer::Entities::MaterialMix_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;
using APTracer::Entities::Material_t;

namespace APTracer { namespace Materials {
    class FresnelMixIn_t final : public MaterialMix_t{
        public:
            FresnelMixIn_t(Material_t* material_refracted, Material_t* material_reflected, double ind);
            virtual ~FresnelMixIn_t() final;

            double ind_;
            std::uniform_real_distribution<double> unif_;

            virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) final;
    };
}}
#endif