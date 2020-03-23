#ifndef SCATTERER_T_H
#define SCATTERER_T_H

#include "entities/ScatteringFunction_t.h"
#include "entities/Ray_t.h"
#include "entities/Vec3f.h"
#include <random>

using APTracer::Entities::ScatteringFunction_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Ray_t;

namespace APTracer::Materials {
    class Scatterer_t final : public ScatteringFunction_t{
        public:
            Scatterer_t(Vec3f emi_vol, Vec3f col_vol, double abs_dist_emi, double abs_dist_col, double scat_dist);
            virtual ~Scatterer_t() final;

            Vec3f emission_vol_;
            Vec3f colour_vol_;
            double scattering_coefficient_;
            std::uniform_real_distribution<double> unif_;

            virtual bool scatter(Ray_t &ray) final; 
    };
}
#endif