#ifndef SCATTEREREXP_T_H
#define SCATTEREREXP_T_H

#include "entities/ScatteringFunction_t.h"
#include "entities/Ray_t.h"
#include "entities/Vec3f.h"
#include <random>

using APTracer::Entities::ScatteringFunction_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Ray_t;

namespace APTracer { namespace Materials {
    class ScattererExp_t final : public ScatteringFunction_t{
        public:
            ScattererExp_t(Vec3f emi_vol, Vec3f col_vol, double abs_dist_emi, double abs_dist_col, double scat_dist, double order, double scattering_angle);
            virtual ~ScattererExp_t() final;

            Vec3f emission_vol_;
            Vec3f colour_vol_;
            double scattering_coefficient_;
            double order_; // 0 to 1: back-scatter. 1: flat. 1 to inf: middle spike, 2 is parabole. inf: straight reflection.
            double scattering_angle_; // between 0 and 1, portion of the hemisphere reflected to.
            std::uniform_real_distribution<double> unif_;

            virtual bool scatter(Ray_t &) final; 
    };
}}
#endif