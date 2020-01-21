#ifndef TRIANGLE_T_H
#define TRIANGLE_T_H

#include "Ray_t.h"
#include "Vec3f.h"
#include "Shape_t.h"

class TransformMatrix_t;
class Material_t;

class Triangle_t final : public Shape_t{
    public:
        Triangle_t(Material_t *material, TransformMatrix_t *transform_matrix, Vec3f* points, Vec3f* normals, double** texcoord);
        virtual ~Triangle_t() final;

        Vec3f points_orig_[3];
        Vec3f normals_orig_[3];
        double texture_coordinates_[3][2];
        Vec3f points_[3];
        Vec3f normals_[3];
        Vec3f v0v1_;
        Vec3f v0v2_;
        double tuv_to_world_[2];
        Vec3f tangent_vec_; // Points up

        virtual void update() final;
        virtual bool intersection(const Ray_t &ray, double &t, double (&uv)[2]) const final; 
        virtual void normaluv(const Ray_t &ray, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec) const final;
        virtual void normal(const Ray_t &ray, const double (&uv)[2], Vec3f &normalvec) const final;
        virtual void normal_uv_tangent(const Ray_t &ray, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec, Vec3f &tangentvec) const final;
        virtual void normal_face(const Ray_t &ray, Vec3f &normalvec) const final;
        virtual Vec3f mincoord() const final;
        virtual Vec3f maxcoord() const final;
};
#endif