#ifndef MATERIAL_T_H
#define MATERIAL_T_H

//#include "Shape_t.h"
#include "Ray_t.h"

class Shape_t;

class Material_t{
    public:
        Material_t(){};
        ~Material_t(){};

        virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) = 0;
};

#endif