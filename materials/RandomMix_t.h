#ifndef RANDOMMIX_T_H
#define RANDOMMIX_T_H

#include "MaterialMix_t.h"
#include "Ray_t.h"
#include <random>

class Material_t;

class RandomMix_t : public MaterialMix_t{
    public:
        RandomMix_t(Material_t* material_refracted, Material_t* material_reflected, double ratio);
        virtual ~RandomMix_t();

        double ratio_;
        std::uniform_real_distribution<double> unif_;

        virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray);
};
#endif