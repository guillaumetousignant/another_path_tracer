#ifndef REFRACTIVE_T_H
#define REFRACTIVE_T_H

#include "Medium_t.h"
#include "Vec3f.h"

class ScatteringFunction_t;

class Refractive_t : public Medium_t{
    public:
        Refractive_t(const Vec3f &emission, const Vec3f &colour, double ind, unsigned int priority, ScatteringFunction_t* scattering);
        ~Refractive_t();

        Vec3f emission_;
        Vec3f colour_;

        virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray);
};
#endif