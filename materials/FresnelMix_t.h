#ifndef FRESNELMIX_T_H
#define FRESNELMIX_T_H

#include "MaterialMix_t.h"
#include "Vec3f.h"
#include "Ray_t.h"
#include <random>

class Material_t;

class FresnelMix_t : public MaterialMix_t{
    public:
        FresnelMix_t(Material_t* material_refracted, Material_t* material_reflected, double ind);
        ~FresnelMix_t();

        double ind_;
        std::uniform_real_distribution<double> unif_;

        virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray);
};
#endif