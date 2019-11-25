#ifndef REFLECTIVEFUZZNORMAL_T_H
#define REFLECTIVEFUZZNORMAL_T_H

#include "Material_t.h"
#include "Vec3f.h"
#include <random>

class Texture_t;

class ReflectiveFuzzNormal_t final : public Material_t{
    public:
        ReflectiveFuzzNormal_t(const Vec3f &emission, const Vec3f &colour, const Texture_t* normal_map, double order, double diffusivity);
        virtual ~ReflectiveFuzzNormal_t() final;

        Vec3f emission_;
        Vec3f colour_;        
        const Texture_t* normal_map_;
        double order_; // 1 to inf, 1 is flat, 2 is parabole,  higher looks like normal and inf is straight reflection.
        double diffusivity_; // between 0 and 1, portion of the hemisphere reflected to.
        std::uniform_real_distribution<double> unif_;

        virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) final;
};
#endif