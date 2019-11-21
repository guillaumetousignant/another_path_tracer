#ifndef FRESNELMIXNORMAL_T_H
#define FRESNELMIXNORMAL_T_H

#include "MaterialMix_t.h"
#include "Vec3f.h"
#include "Ray_t.h"
#include <random>

class Material_t;
class Texture_t;

class FresnelMixNormal_t final : public MaterialMix_t{
    public:
        FresnelMixNormal_t(Material_t* material_refracted, Material_t* material_reflected, double ind, const Texture_t* normal_map);
        virtual ~FresnelMixNormal_t() final;

        double ind_;
        const Texture_t* normal_map_;
        std::uniform_real_distribution<double> unif_;

        virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) final;
};
#endif