#ifndef MEDIUM_T_H
#define MEDIUM_T_H

#include "Material_t.h"
#include "Ray_t.h"
#include "ScatteringFunction_t.h"

class Medium_t : public Material_t{
    public:
        Medium_t(unsigned int priority, ScatteringFunction_t* scattering): priority_(priority), scattering_(scattering) {};
        virtual ~Medium_t(){};

        unsigned int priority_;
        ScatteringFunction_t* scattering_;
};

#endif