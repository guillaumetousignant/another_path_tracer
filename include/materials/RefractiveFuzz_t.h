#ifndef APTRACER_REFRACTIVEFUZZ_T_H
#define APTRACER_REFRACTIVEFUZZ_T_H

#include "entities/Material_t.h"
#include "entities/Vec3f.h"
#include "entities/Ray_t.h"
#include <random>

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
    class RefractiveFuzz_t final : public Material_t{
        public:
            RefractiveFuzz_t(const Vec3f &emission, const Vec3f &colour, double order, double diffusivity, Medium_t* medium);
            virtual ~RefractiveFuzz_t() final;

            Vec3f emission_;
            Vec3f colour_;
            double order_; // 1 to inf, 1 is flat, 2 is parabole,  higher looks like normal and inf is straight reflection.
            double diffusivity_; // between 0 and 1, portion of the hemisphere reflected to.
            std::uniform_real_distribution<double> unif_;
            Medium_t* medium_;

            virtual void bounce(std::array<double, 2> uv, const Shape_t* hit_obj, Ray_t &ray) final;
    };
}}
#endif