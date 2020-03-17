#ifndef NONABSORBER_T_H
#define NONABSORBER_T_H

#include "ScatteringFunction_t.h"
#include "Ray_t.h"

using APTracer::Entities::ScatteringFunction_t;
using APTracer::Entities::Ray_t;

namespace APTracer::Materials {
    class NonAbsorber_t final : public ScatteringFunction_t{
        public:
            NonAbsorber_t();
            virtual ~NonAbsorber_t() final;

            virtual bool scatter(Ray_t &ray) final; 
    };
}
#endif