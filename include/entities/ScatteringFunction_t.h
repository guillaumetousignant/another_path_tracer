#ifndef SCATTERINGFUNCTION_T_H
#define SCATTERINGFUNCTION_T_H

#include "entities/Ray_t.h"

namespace APTracer { namespace Entities {
    class ScatteringFunction_t{
        public:
            ScatteringFunction_t(){};
            virtual ~ScatteringFunction_t(){};

            virtual bool scatter(Ray_t &ray) = 0; 
    };
}

#endif