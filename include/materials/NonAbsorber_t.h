#ifndef NONABSORBER_T_H
#define NONABSORBER_T_H

#include "entities/Medium_t.h"
#include "entities/Ray_t.h"

using APTracer::Entities::Medium_t;
using APTracer::Entities::Ray_t;

namespace APTracer { namespace Materials {
    class NonAbsorber_t final : public Medium_t{
        public:
            NonAbsorber_t(double ind, unsigned int priority);
            virtual ~NonAbsorber_t() final;

            virtual bool scatter(Ray_t &ray) final; 
    };
}}
#endif