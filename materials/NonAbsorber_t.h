#ifndef NONABSORBER_T_H
#define NONABSORBER_T_H

#include "ScatteringFunction_t.h"

class NonAbsorber_t : public ScatteringFunction_t{
    public:
        NonAbsorber_t();
        ~NonAbsorber_t();

        virtual void scatter(Ray_t &ray, bool &intersected) const; 
};
#endif