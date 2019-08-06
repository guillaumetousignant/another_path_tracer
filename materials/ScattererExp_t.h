#ifndef SCATTEREREXP_T_H
#define SCATTEREREXP_T_H

#include "ScatteringFunction_t.h"
#include "Ray_t.h"
#include "Vec3f.h"
#include <random>

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

        virtual void scatter(Ray_t &ray, bool &intersected) final; 
};
#endif