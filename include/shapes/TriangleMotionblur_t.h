#ifndef TRIANGLEMOTIONBLUR_T_H
#define TRIANGLEMOTIONBLUR_T_H

#include "entities/Ray_t.h"
#include "entities/Vec3f.h"
#include "entities/Shape_t.h"

namespace APTracer { namespace Entities {
    class TransformMatrix_t;
    class Material_t;
}}

using APTracer::Entities::Ray_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Material_t;
using APTracer::Entities::TransformMatrix_t;
using APTracer::Entities::Shape_t;

namespace APTracer { namespace Shapes {
    class TriangleMotionblur_t final : public Shape_t{
        public:
            TriangleMotionblur_t(Material_t *material, TransformMatrix_t *transform_matrix, Vec3f* points, Vec3f* normals, double** texcoord);
            virtual ~TriangleMotionblur_t() final;

            Vec3f points_orig_[3];
            Vec3f normals_orig_[3];
            double texture_coordinates_[3][2];
            double tuv_to_world_[2];
            Vec3f points_[3];
            Vec3f normals_[3];
            Vec3f v0v1_;
            Vec3f v0v2_;
            Vec3f tangent_vec_; // Points up
            Vec3f points_last_[3];
            Vec3f normals_last_[3];
            Vec3f v0v1_last_;
            Vec3f v0v2_last_;
            Vec3f tangent_vec_last_; // Points up

            virtual void update() final;
            virtual bool intersection(const Ray_t &ray, double &t, double (&uv)[2]) const final; 
            virtual void normaluv(double time, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec) const final;
            virtual void normal(double time, const double (&uv)[2], Vec3f &normalvec) const final;
            virtual void normal_uv_tangent(double time, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec, Vec3f &tangentvec) const final;
            virtual void normal_face(double time, Vec3f &normalvec) const final;
            virtual Vec3f mincoord() const final;
            virtual Vec3f maxcoord() const final;
    };
}}
#endif