#ifndef ACCELERATIONSTRUCTURE_T_H
#define ACCELERATIONSTRUCTURE_T_H

#include "Ray_t.h"

class Shape_t;

class AccelerationStructure_t{
    public:
        AccelerationStructure_t(){};
        virtual ~AccelerationStructure_t(){};

        unsigned int n_obj_;

        virtual void update() = 0;
        virtual void intersect(const Ray_t &ray, Shape_t* &hit_obj, double &t, double (&uv)[2]) const = 0;
        virtual void add(Shape_t* item) = 0;
};

#endif