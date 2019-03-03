#ifndef DIFFUSE_T_H
#define DIFFUSE_T_H

#include "Material_t.h"
#include "Vec3f.h"
#include <random>

class Diffuse_t : public Material_t{
    public:
        Diffuse_t(Vec3f emission, Vec3f colour, double roughness);
        ~Diffuse_t();

        Vec3f emission_;
        Vec3f colour_;
        double roughness_;
        std::uniform_real_distribution<double> unif_;

        virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray);
};
#endif