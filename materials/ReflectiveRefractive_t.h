#ifndef REFLECTIVEREFRACTIVE_T_H
#define REFLECTIVEREFRACTIVE_T_H

#include "Medium_t.h"
#include "Vec3f.h"
#include <random>

class ScatteringFunction_t;

class ReflectiveRefractive_t : public Medium_t{
    public:
        ReflectiveRefractive_t(const Vec3f &emission, const Vec3f &colour, double ind, unsigned int priority, ScatteringFunction_t* scattering);
        ~ReflectiveRefractive_t();

        Vec3f emission_;
        Vec3f colour_;
        std::uniform_real_distribution<double> unif_;

        virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray);
};
#endif