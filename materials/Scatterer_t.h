#ifndef SCATTERER_T_H
#define SCATTERER_T_H

#include "ScatteringFunction_t.h"
#include "Ray_t.h"
#include "Vec3f.h"
#include <random>

class Scatterer_t : public ScatteringFunction_t{
    public:
        Scatterer_t(Vec3f emi_vol, Vec3f col_vol, double abs_dist_emi, double abs_dist_col, double scat_dist);
        virtual ~Scatterer_t();

        Vec3f emission_vol_;
        Vec3f colour_vol_;
        double scattering_coefficient_;
        std::uniform_real_distribution<double> unif_;

        virtual void scatter(Ray_t &ray, bool &intersected); 
};
#endif