#ifndef SPHERE_T_H
#define SPHERE_T_H

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
    class Sphere_t final : public Shape_t{
        public:
            Sphere_t(Material_t *material, TransformMatrix_t *transform_matrix);
            virtual ~Sphere_t() final;

            double radius_;
            Vec3f origin_;
            Vec3f direction_sph_;

            virtual void update() final;
            virtual bool intersection(const Ray_t &ray, double &t, double (&uv)[2]) const final; 
            virtual void normaluv(double time, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec) const final;
            virtual void normal(double time, const double (&uv)[2], Vec3f &normalvec) const final;
            virtual void normal_uv_tangent(double time, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec, Vec3f &tangentvec) const final;
            virtual Vec3f mincoord() const final;
            virtual Vec3f maxcoord() const final;
    };
}}
#endif