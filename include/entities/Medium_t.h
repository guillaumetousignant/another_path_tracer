#ifndef MEDIUM_T_H
#define MEDIUM_T_H

#include "entities/Ray_t.h"

namespace APTracer { namespace Entities {

    class Medium_t {
        public:
            Medium_t(double ind, unsigned int priority): ind_(ind), priority_(priority) {};
            virtual ~Medium_t(){};

            double ind_;
            unsigned int priority_;

            virtual bool scatter(Ray_t &ray) = 0;
    };
}}

#endif