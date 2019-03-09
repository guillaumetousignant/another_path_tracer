#ifndef MEDIUM_T_H
#define MEDIUM_T_H

#include "Material_t.h"
//#include "Ray_t.h"
//#include "ScatteringFunction_t.h"

class ScatteringFunction_t;

class Medium_t : public Material_t{
    public:
        Medium_t(double ind, unsigned int priority, ScatteringFunction_t* scattering): ind_(ind), priority_(priority), scattering_(scattering) {};
        virtual ~Medium_t(){};

        double ind_;
        unsigned int priority_;
        ScatteringFunction_t* scattering_;
        
};

#endif