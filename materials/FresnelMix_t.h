#ifndef FRESNELMIX_T_H
#define FRESNELMIX_T_H

#include "Material_t.h"
#include "Vec3f.h"
#include "Ray_t.h"
#include <random>

class FresnelMix_t : public Material_t{
    public:
        FresnelMix_t(Material_t* material_refracted, Material_t* material_reflected, double ind);
        ~FresnelMix_t();

        Material_t* material_refracted_;
        Material_t* material_reflected_;
        double ind_;
        std::uniform_real_distribution<double> unif_;

        virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray);
};
#endif