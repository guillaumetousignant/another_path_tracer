#ifndef DIFFUSETEXNORMAL_T_H
#define DIFFUSETEXNORMAL_T_H

#include "Material_t.h"
#include "Vec3f.h"
#include <random>

class Texture_t;

class DiffuseTexNormal_t final : public Material_t{
    public:
        DiffuseTexNormal_t(const Vec3f &emission, const Texture_t* texture, const Texture_t* normal_map, double roughness);
        virtual ~DiffuseTexNormal_t() final;

        Vec3f emission_;
        const Texture_t* texture_;
        const Texture_t* normal_map_;
        double roughness_;
        std::uniform_real_distribution<double> unif_;

        virtual void bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) final;
};
#endif