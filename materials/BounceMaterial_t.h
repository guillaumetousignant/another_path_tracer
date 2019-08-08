#ifndef BOUNCEMATERIAL_T_H
#define BOUNCEMATERIAL_T_H

#include "Material_t.h"
#include "Vec3f.h"
#include <random>

class Shape_t;

class BounceMaterial_t final : public Material_t{
    public:
        BounceMaterial_t(unsigned int max_bounces);
        virtual ~BounceMaterial_t() final;

        Vec3f emission_;        
        std::uniform_real_distribution<double> unif_;

        virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) final;
};
#endif