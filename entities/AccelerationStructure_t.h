#ifndef ACCELERATIONSTRUCTURE_T_H
#define ACCELERATIONSTRUCTURE_T_H

#include "Ray_t.h"
#include "Shape_t.h"

class AccelerationStructure_t{
    public:
        AccelerationStructure_t(){};
        ~AccelerationStructure_t(){};

        virtual void update() = 0;
        virtual void intersect(Ray_t &ray, Shape_t* hit_obj, double &t, double (&uv)[2]) = 0;
};

#endif