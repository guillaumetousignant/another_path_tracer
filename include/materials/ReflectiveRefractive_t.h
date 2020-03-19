#ifndef REFLECTIVEREFRACTIVE_T_H
#define REFLECTIVEREFRACTIVE_T_H

#include "Medium_t.h"
#include "Vec3f.h"
#include "Ray_t.h"
#include <random>

namespace APTracer::Entities {
    class ScatteringFunction_t;
    class Shape_t;
}

using APTracer::Entities::Medium_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;
using APTracer::Entities::ScatteringFunction_t;

namespace APTracer::Materials {
    class ReflectiveRefractive_t final : public Medium_t{
        public:
            ReflectiveRefractive_t(const Vec3f &emission, const Vec3f &colour, double ind, unsigned int priority, ScatteringFunction_t* scattering);
            virtual ~ReflectiveRefractive_t() final;

            Vec3f emission_;
            Vec3f colour_;
            std::uniform_real_distribution<double> unif_;

            virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) final;
    };
}
#endif