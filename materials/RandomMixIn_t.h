#ifndef RANDOMMIXIN_T_H
#define RANDOMMIXIN_T_H

#include "MaterialMix_t.h"
#include "Ray_t.h"
#include <random>

class Material_t;

class RandomMixIn_t : public MaterialMix_t{
    public:
        RandomMixIn_t(Material_t* material_refracted, Material_t* material_reflected, double ratio);
        ~RandomMixIn_t();

        double ratio_;
        std::uniform_real_distribution<double> unif_;

        virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray);
};
#endif