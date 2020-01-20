#ifndef ABSORBER_T_H
#define ABSORBER_T_H

#include "ScatteringFunction_t.h"
#include "Ray_t.h"
#include "Vec3f.h"

class Absorber_t final : public ScatteringFunction_t{
    public:
        Absorber_t(Vec3f emi_vol, Vec3f col_vol, double abs_dist_emi, double abs_dist_col);
        virtual ~Absorber_t() final;

        Vec3f emission_vol_;
        Vec3f colour_vol_;

        virtual bool scatter(Ray_t &ray) final; 
};
#endif