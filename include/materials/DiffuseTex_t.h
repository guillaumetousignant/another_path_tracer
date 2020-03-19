#ifndef DIFFUSETEX_T_H
#define DIFFUSETEX_T_H

#include "Material_t.h"
#include "Vec3f.h"
#include "Ray_t.h"
#include <random>

namespace APTracer::Entities {
    class Texture_t;
    class Shape_t;
}

using APTracer::Entities::Material_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;
using APTracer::Entities::Texture_t;

namespace APTracer::Materials {
    class DiffuseTex_t final : public Material_t{
        public:
            DiffuseTex_t(const Vec3f &emission, const Texture_t* texture, double roughness);
            virtual ~DiffuseTex_t() final;

            Vec3f emission_;
            const Texture_t* texture_;
            double roughness_;
            std::uniform_real_distribution<double> unif_;

            virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) final;
    };
}
#endif