#ifndef REFLECTIVEREFRACTIVEFUZZ_T_H
#define REFLECTIVEREFRACTIVEFUZZ_T_H

#include "entities/Medium_t.h"
#include "entities/Vec3f.h"
#include "entities/Ray_t.h"
#include <random>

namespace APTracer { namespace Entities {
    class ScatteringFunction_t;
    class Shape_t;
}}

using APTracer::Entities::Medium_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;
using APTracer::Entities::ScatteringFunction_t;

namespace APTracer { namespace Materials {
    class ReflectiveRefractiveFuzz_t final : public Medium_t{
        public:
            ReflectiveRefractiveFuzz_t(const Vec3f &emission, const Vec3f &colour, double ind, unsigned int priority, double order, double diffusivity, ScatteringFunction_t* scattering);
            virtual ~ReflectiveRefractiveFuzz_t() final;

            Vec3f emission_;
            Vec3f colour_;
            double order_;
            double diffusivity_;
            std::uniform_real_distribution<double> unif_;

            virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) final;
    };
}}
#endif