#ifndef SCATTEREREXPFULL_T_H
#define SCATTEREREXPFULL_T_H

#include "ScatteringFunction_t.h"
#include "Ray_t.h"
#include "Vec3f.h"
#include <random>

class ScattererExpFull_t final : public ScatteringFunction_t{
    public:
        ScattererExpFull_t(Vec3f emi_vol, Vec3f col_vol, Vec3f emi_scat, Vec3f col_scat, double abs_dist_emi, double abs_dist_col, double scat_dist, double order, double scattering_angle);
        virtual ~ScattererExpFull_t() final;

        Vec3f emission_vol_;
        Vec3f colour_vol_;
        Vec3f emission_scat_;
        Vec3f colour_scat_;
        double scattering_coefficient_;
        double order_; // 0 to 1: back-scatter. 1: flat. 1 to inf: middle spike, 2 is parabole. inf: straight reflection.
        double scattering_angle_; // between 0 and 1, portion of the hemisphere reflected to.
        std::uniform_real_distribution<double> unif_;

        virtual void scatter(Ray_t &ray, bool &intersected) final; 
};
#endif