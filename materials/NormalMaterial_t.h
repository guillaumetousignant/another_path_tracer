#ifndef NORMALMATERIAL_T_H
#define NORMALMATERIAL_T_H

#include "Material_t.h"

class Shape_t;

class NormalMaterial_t : public Material_t{
    public:
        NormalMaterial_t();
        ~NormalMaterial_t();

        virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray);
};
#endif