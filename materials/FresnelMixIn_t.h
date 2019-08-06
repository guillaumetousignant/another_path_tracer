#ifndef FRESNELMIXIN_T_H
#define FRESNELMIXIN_T_H

#include "MaterialMix_t.h"
#include "Vec3f.h"
#include "Ray_t.h"
#include <random>

class Material_t;

class FresnelMixIn_t final : public MaterialMix_t{
    public:
        FresnelMixIn_t(Material_t* material_refracted, Material_t* material_reflected, double ind);
        virtual ~FresnelMixIn_t() final;

        double ind_;
        std::uniform_real_distribution<double> unif_;

        virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) final;
};
#endif