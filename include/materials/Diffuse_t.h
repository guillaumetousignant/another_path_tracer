#ifndef APTRACER_DIFFUSE_T_H
#define APTRACER_DIFFUSE_T_H

#include "entities/Material_t.h"
#include "entities/Vec3f.h"
#include "entities/Ray_t.h"
#include <random>

namespace APTracer { namespace Entities {
    class Shape_t;
}}

using APTracer::Entities::Material_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;

namespace APTracer { namespace Materials {
    class Diffuse_t final : public Material_t{
        public:
            Diffuse_t(const Vec3f &emission, const Vec3f &colour, double roughness);
            virtual ~Diffuse_t() final;

            Vec3f emission_;
            Vec3f colour_;
            double roughness_;
            std::uniform_real_distribution<double> unif_;

            virtual void bounce(const std::array<double, 2> &uv, const Shape_t* hit_obj, Ray_t &ray) final;
    };
}}
#endif