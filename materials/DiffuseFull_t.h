#ifndef DIFFUSEFULL_T_H
#define DIFFUSEFULL_T_H

#include "Material_t.h"
#include "Vec3f.h"
#include <random>

class Texture_t;

class DiffuseFull_t : public Material_t{
    public:
        DiffuseFull_t(const Texture_t* emission_map, const Texture_t* texture, double roughness);
        ~DiffuseFull_t();

        const Texture_t* emission_map_;
        const Texture_t* texture_;
        double roughness_;
        std::uniform_real_distribution<double> unif_;

        virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray);
};
#endif