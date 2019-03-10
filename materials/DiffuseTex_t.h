#ifndef DIFFUSETEX_T_H
#define DIFFUSETEX_T_H

#include "Material_t.h"
#include "Vec3f.h"
#include <random>

class Texture_t;

class DiffuseTex_t : public Material_t{
    public:
        DiffuseTex_t(const Vec3f &emission, const Texture_t* texture, double roughness);
        ~DiffuseTex_t();

        Vec3f emission_;
        const Texture_t* texture_;
        double roughness_;
        std::uniform_real_distribution<double> unif_;

        virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray);
};
#endif