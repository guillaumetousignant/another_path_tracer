#ifndef SPHEREMOTIONBLUR_T_H
#define SPHEREMOTIONBLUR_T_H

#include "Ray_t.h"
#include "Vec3f.h"
#include "Shape_t.h"

class TransformMatrix_t;
class Material_t;

class SphereMotionblur_t final : public Shape_t{
    public:
        SphereMotionblur_t(Material_t *material, TransformMatrix_t *transform_matrix);
        virtual ~SphereMotionblur_t() final;

        double radius_;
        Vec3f origin_;
        Vec3f direction_sph_;
        double radius_last_;
        Vec3f origin_last_;
        Vec3f direction_sph_last_;

        virtual void update() final;
        virtual bool intersection(const Ray_t &ray, double &t, double (&uv)[2]) const final; 
        virtual void normaluv(const Ray_t &ray, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec) const final;
        virtual void normal(const Ray_t &ray, const double (&uv)[2], Vec3f &normalvec) const final;
        virtual void normal_uv_tangent(const Ray_t &ray, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec, Vec3f &tangentvec) const final;
        virtual Vec3f mincoord() const final;
        virtual Vec3f maxcoord() const final;
};
#endif