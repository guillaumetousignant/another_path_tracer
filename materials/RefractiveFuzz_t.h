#ifndef REFRACTIVEFUZZ_T_H
#define REFRACTIVEFUZZ_T_H

#include "Medium_t.h"
#include "Vec3f.h"
#include <random>

class ScatteringFunction_t;

class RefractiveFuzz_t : public Medium_t{
    public:
        RefractiveFuzz_t(const Vec3f &emission, const Vec3f &colour, double ind, unsigned int priority, double order, double diffusivity, ScatteringFunction_t* scattering);
        virtual ~RefractiveFuzz_t();

        Vec3f emission_;
        Vec3f colour_;
        double order_; // 1 to inf, 1 is flat, 2 is parabole,  higher looks like normal and inf is straight reflection.
        double diffusivity_; // between 0 and 1, portion of the hemisphere reflected to.
        std::uniform_real_distribution<double> unif_;

        virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray);
};
#endif