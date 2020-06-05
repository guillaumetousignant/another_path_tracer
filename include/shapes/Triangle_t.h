#ifndef TRIANGLE_T_H
#define TRIANGLE_T_H

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
            virtual Vec3f normaluv(double time, const double (&uv)[2], double (&tuv)[2]) const final;
            virtual Vec3f normal(double time, const double (&uv)[2]) const final;
            virtual Vec3f normal_uv_tangent(double time, const double (&uv)[2], double (&tuv)[2], Vec3f &tangentvec) const final;
            virtual Vec3f normal_face(double time) const final;
            virtual Vec3f mincoord() const final;
            virtual Vec3f maxcoord() const final;
    };
}}
#endif