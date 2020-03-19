#ifndef SCATTERERFULL_T_H
#define SCATTERERFULL_T_H

#include "ScatteringFunction_t.h"
#include "Ray_t.h"
#include "Vec3f.h"
#include <random>

using APTracer::Entities::ScatteringFunction_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Ray_t;

namespace APTracer::Materials {
    class ScattererFull_t final : public ScatteringFunction_t{
        public:
            ScattererFull_t(Vec3f emi_vol, Vec3f col_vol, Vec3f emi_scat, Vec3f col_scat, double abs_dist_emi, double abs_dist_col, double scat_dist);
            virtual ~ScattererFull_t() final;

            Vec3f emission_vol_;
            Vec3f colour_vol_;
            Vec3f emission_scat_;
            Vec3f colour_scat_;
            double scattering_coefficient_;
            std::uniform_real_distribution<double> unif_;

            virtual bool scatter(Ray_t &ray) final; 
    };
}
#endif