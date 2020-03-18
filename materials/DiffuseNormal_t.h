#ifndef DIFFUSENORMAL_T_H
#define DIFFUSENORMAL_T_H

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
    class DiffuseNormal_t final : public Material_t{
        public:
            DiffuseNormal_t(const Vec3f &emission, const Vec3f &colour, const Texture_t* normal_map, double roughness);
            virtual ~DiffuseNormal_t() final;

            Vec3f emission_;
            Vec3f colour_;
            const Texture_t* normal_map_;
            double roughness_;
            std::uniform_real_distribution<double> unif_;

            virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) final;
    };
}
#endif