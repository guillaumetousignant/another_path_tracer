#ifndef TRIANGLEMESHMOTIONBLUR_T_H
#define TRIANGLEMESHMOTIONBLUR_T_H

#include "Ray_t.h"
#include "Vec3f.h"
#include "Shape_t.h"

using APTracer::Entities::Ray_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Material_t;
using APTracer::Entities::TransformMatrix_t;
using APTracer::Entities::Shape_t;
using APTracer::Entities::MeshGeometry_t;

class TransformMatrix_t;
class Material_t;
class MeshGeometry_t;

namespace APTracer::Shapes {
    class TriangleMeshMotionblur_t final : public Shape_t{
        public:
            TriangleMeshMotionblur_t(Material_t *material, TransformMatrix_t *transform_matrix, MeshGeometry_t* geom, unsigned int index);
            virtual ~TriangleMeshMotionblur_t() final;

            Vec3f points_[3];
            Vec3f normals_[3];
            Vec3f v0v1_;
            Vec3f v0v2_;
            Vec3f tangent_vec_; // Points up
            MeshGeometry_t* geom_;
            unsigned int index_;
            double tuv_to_world_[2];
            Vec3f points_last_[3];
            Vec3f normals_last_[3];
            Vec3f v0v1_last_;
            Vec3f v0v2_last_;
            Vec3f tangent_vec_last_; // Points up

            virtual void update() final;
            virtual bool intersection(const Ray_t &rays, double &t, double (&uv)[2]) const final; 
            virtual void normaluv(const Ray_t &ray, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec) const final;
            virtual void normal(const Ray_t &ray, const double (&uv)[2], Vec3f &normalvec) const final;
            virtual void normal_uv_tangent(const Ray_t &ray, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec, Vec3f &tangentvec) const final;
            virtual void normal_face(const Ray_t &ray, Vec3f &normalvec) const final;
            virtual Vec3f mincoord() const final;
            virtual Vec3f maxcoord() const final;
    };
}
#endif