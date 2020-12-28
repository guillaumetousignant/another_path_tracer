#ifndef APTRACER_NORMALDIFFUSEMATERIAL_T_H
#define APTRACER_NORMALDIFFUSEMATERIAL_T_H

#include "entities/Material_t.h"
#include "entities/Ray_t.h"
#include <random>

namespace APTracer { namespace Entities {
    class Shape_t;
}}

using APTracer::Entities::Material_t;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;

namespace APTracer { namespace Materials {
    class NormalDiffuseMaterial_t final : public Material_t{
        public:
            NormalDiffuseMaterial_t(double roughness);
            virtual ~NormalDiffuseMaterial_t() final;

            double roughness_;
            std::uniform_real_distribution<double> unif_;

            virtual void bounce(const std::array<double, 2> &uv, const Shape_t* hit_obj, Ray_t &ray) final;
    };
}}
#endif