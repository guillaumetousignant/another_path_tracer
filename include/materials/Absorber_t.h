#ifndef APTRACER_ABSORBER_T_H
#define APTRACER_ABSORBER_T_H

#include "entities/Medium_t.h"
#include "entities/Ray_t.h"
#include "entities/Vec3f.h"

using APTracer::Entities::Medium_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Ray_t;

namespace APTracer { namespace Materials {
    class Absorber_t final : public Medium_t{
        public:
            Absorber_t(Vec3f emi_vol, Vec3f col_vol, double abs_dist_emi, double abs_dist_col, double ind, unsigned int priority);
            virtual ~Absorber_t() final;

            Vec3f emission_vol_;
            Vec3f colour_vol_;

            virtual bool scatter(Ray_t &ray) final; 
    };
}}
#endif