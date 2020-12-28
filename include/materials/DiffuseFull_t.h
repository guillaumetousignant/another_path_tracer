#ifndef APTRACER_DIFFUSEFULL_T_H
#define APTRACER_DIFFUSEFULL_T_H

#include "entities/Material_t.h"
#include "entities/Vec3f.h"
#include "entities/Ray_t.h"
#include <random>

namespace APTracer { namespace Entities {
    class Texture_t;
    class Shape_t;
}}

using APTracer::Entities::Material_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;
using APTracer::Entities::Texture_t;

namespace APTracer { namespace Materials {
    class DiffuseFull_t final : public Material_t{
        public:
            DiffuseFull_t(const Texture_t* emission_map, const Texture_t* texture, double roughness);
            virtual ~DiffuseFull_t() final;

            const Texture_t* emission_map_;
            const Texture_t* texture_;
            double roughness_;
            std::uniform_real_distribution<double> unif_;

            virtual void bounce(std::array<double, 2> uv, const Shape_t* hit_obj, Ray_t &ray) final;
    };
}}
#endif