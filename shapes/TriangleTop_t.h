#ifndef TRIANGLETOP_T_H
#define TRIANGLETOP_T_H

//#include "Material_t.h"
//#include "TransformMatrix_t.h"
#include "Ray_t.h"
#include "Vec3f.h"
#include "Shape_t.h"

class TransformMatrix_t;
class Material_t;

class TriangleTop_t : public Shape_t{
    public:
        TriangleTop_t(Material_t *material, TransformMatrix_t *transform_matrix);
        virtual ~TriangleTop_t();

        Vec3f points_[3];
        Vec3f normals_[3];
        Vec3f v0v1_;
        Vec3f v0v2_;

        virtual void update() = 0;
        virtual void intersection(const Ray_t &ray, bool &intersected, double &t, double (&uv)[2]) const; 
        virtual void normaluv(const Ray_t &ray, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec) const = 0;
        virtual void normal(const Ray_t &ray, const double (&uv)[2], Vec3f &normalvec) const;
        virtual void normal_face(Vec3f &normalvec) const;
        virtual Vec3f mincoord() const;
        virtual Vec3f maxcoord() const;
};
#endif