#ifndef REFLECTIVEREFRACTIVEFUZZ_T_H
#define REFLECTIVEREFRACTIVEFUZZ_T_H

#include "Medium_t.h"
#include "Vec3f.h"
#include <random>

class ScatteringFunction_t;

class ReflectiveRefractiveFuzz_t : public Medium_t{
    public:
        ReflectiveRefractiveFuzz_t(const Vec3f &emission, const Vec3f &colour, double ind, unsigned int priority, double order, double diffusivity, ScatteringFunction_t* scattering);
        ~ReflectiveRefractiveFuzz_t();

        Vec3f emission_;
        Vec3f colour_;
        double order_;
        double diffusivity_;
        std::uniform_real_distribution<double> unif_;

        virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray);
};
#endif