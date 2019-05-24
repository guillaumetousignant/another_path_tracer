#ifndef TRANSPARENT_T_H
#define TRANSPARENT_T_H

#include "Medium_t.h"
#include "Vec3f.h"
#include "Ray_t.h"

class ScatteringFunction_t;
class Shape_t;

class Transparent_t : public Medium_t {
    public:
        Transparent_t(unsigned int priority, ScatteringFunction_t* scattering);
        virtual ~Transparent_t();

        virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray);
};
#endif