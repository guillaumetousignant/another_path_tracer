#ifndef BOX_T_H
#define BOX_T_H

#include "Ray_t.h"
#include "Vec3f.h"
#include "Shape_t.h"

class TransformMatrix_t;
class Material_t;

class Box_t : public Shape_t{
    public:
        Box_t(Material_t *material, TransformMatrix_t *transform_matrix, Vec3f (&coord)[2]);
        virtual ~Box_t();

        Vec3f coordinates_[2];

        virtual void update();
        virtual void intersection(const Ray_t &ray, bool &intersected, double &t, double (&uv)[2]) const; 
        virtual void normaluv(const Ray_t &ray, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec) const;
        virtual void normal(const Ray_t &ray, const double (&uv)[2], Vec3f &normalvec) const;
        virtual Vec3f mincoord() const;
        virtual Vec3f maxcoord() const;
};
#endif