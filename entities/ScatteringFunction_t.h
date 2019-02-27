#ifndef SCATTERINGFUNCTION_T_H
#define SCATTERINGFUNCTION_T_H

#include "Ray_t.h"

class ScatteringFunction_t{
    public:
        ScatteringFunction_t(){};
        ~ScatteringFunction_t(){};

        virtual void scatter(Ray_t &ray, bool &intersected) const = 0; 
};

#endif