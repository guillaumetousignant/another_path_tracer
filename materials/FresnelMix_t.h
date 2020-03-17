#ifndef FRESNELMIX_T_H
#define FRESNELMIX_T_H

#include "MaterialMix_t.h"
#include "Vec3f.h"
#include "Ray_t.h"
#include <random>

using APTracer::Entities::MaterialMix_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Shape_t;
using APTracer::Entities::Material_t;

class Material_t;

namespace APTracer::Materials {
    class FresnelMix_t final : public MaterialMix_t{
        public:
            FresnelMix_t(Material_t* material_refracted, Material_t* material_reflected, double ind);
            virtual ~FresnelMix_t() final;

            double ind_;
            std::uniform_real_distribution<double> unif_;

            virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) final;
    };
}
#endif