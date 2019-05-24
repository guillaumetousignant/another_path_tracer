#ifndef SPHEREMOTIONBLUR_T_H
#define SPHEREMOTIONBLUR_T_H

#include "Ray_t.h"
#include "Vec3f.h"
#include "Sphere_t.h"

class TransformMatrix_t;
class Material_t;

class SphereMotionblur_t : public Sphere_t{
    public:
        SphereMotionblur_t(Material_t *material, TransformMatrix_t *transform_matrix);
        virtual ~SphereMotionblur_t();

        double radius_last_;
        Vec3f origin_last_;
        Vec3f direction_sph_last_;

        virtual void update();
        virtual void intersection(const Ray_t &ray, bool &intersected, double &t, double (&uv)[2]) const; 
        virtual void normaluv(const Ray_t &ray, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec) const;
        virtual Vec3f mincoord() const;
        virtual Vec3f maxcoord() const;
};
#endif