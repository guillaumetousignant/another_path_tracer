#ifndef SPHERE_T_H
#define SPHERE_T_H

#include "Ray_t.h"
#include "Vec3f.h"
#include "Shape_t.h"

class TransformMatrix_t;
class Material_t;

class Sphere_t : public Shape_t{
    public:
        Sphere_t(Material_t *material, TransformMatrix_t *transform_matrix);
        virtual ~Sphere_t();

        double radius_;
        Vec3f origin_;
        Vec3f direction_sph_;

        virtual void update();
        virtual void intersection(const Ray_t &ray, bool &intersected, double &t, double (&uv)[2]) const; 
        virtual void normaluv(const Ray_t &ray, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec) const;
        virtual void normal(const Ray_t &ray, const double (&uv)[2], Vec3f &normalvec) const;
        virtual Vec3f mincoord() const;
        virtual Vec3f maxcoord() const;
};
#endif