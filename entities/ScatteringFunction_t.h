#ifndef SCATTERINGFUNCTION_T_H
#define SCATTERINGFUNCTION_T_H

#include "Ray_t.h"

class ScatteringFunction_t{
    public:
        ScatteringFunction_t(){};
        virtual ~ScatteringFunction_t(){};

        virtual bool scatter(Ray_t &ray) = 0; 
};

#endif