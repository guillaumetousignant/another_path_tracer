#ifndef TRIANGLEMOTIONBLUR_T_H
#define TRIANGLEMOTIONBLUR_T_H

#include "Ray_t.h"
#include "Vec3f.h"
#include "Shape_t.h"

class TransformMatrix_t;
class Material_t;

class TriangleMotionblur_t : public Shape_t{
    public:
        TriangleMotionblur_t(Material_t *material, TransformMatrix_t *transform_matrix, Vec3f* points, Vec3f* normals, double** texcoord);
        virtual ~TriangleMotionblur_t();

        Vec3f points_orig_[3];
        Vec3f normals_orig_[3];
        double texture_coordinates_[3][2];
        Vec3f points_[3];
        Vec3f normals_[3];
        Vec3f v0v1_;
        Vec3f v0v2_;
        Vec3f points_last_[3];
        Vec3f normals_last_[3];
        Vec3f v0v1_last_;
        Vec3f v0v2_last_;

        virtual void update();
        virtual void intersection(const Ray_t &ray, bool &intersected, double &t, double (&uv)[2]) const; 
        virtual void normaluv(const Ray_t &ray, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec) const;
        virtual void normal(const Ray_t &ray, const double (&uv)[2], Vec3f &normalvec) const;
        virtual void normal_face(const Ray_t &ray, Vec3f &normalvec) const;
        virtual Vec3f mincoord() const;
        virtual Vec3f maxcoord() const;
};
#endif