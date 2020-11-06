#ifndef TRIANGLEMESH_T_H
#define TRIANGLEMESH_T_H

#include "entities/Ray_t.h"
#include "entities/Vec3f.h"
#include "entities/Shape_t.h"

namespace APTracer { namespace Entities {
    class TransformMatrix_t;
    class Material_t;
    class MeshGeometry_t;
}}

using APTracer::Entities::Ray_t;
using APTracer::Entities::Vec3f;
using APTracer::Entities::Material_t;
using APTracer::Entities::TransformMatrix_t;
using APTracer::Entities::Shape_t;
using APTracer::Entities::MeshGeometry_t;

namespace APTracer { namespace Shapes {
    /**
     * @brief 
     * 
     */
    class TriangleMesh_t final : public Shape_t{
        public:
            TriangleMesh_t(Material_t *material, TransformMatrix_t *transform_matrix, MeshGeometry_t* geom, unsigned int index);
            virtual ~TriangleMesh_t() final;

            Vec3f points_[3];
            Vec3f normals_[3];
            Vec3f v0v1_;
            Vec3f v0v2_;
            MeshGeometry_t* geom_;
            unsigned int index_;
            double tuv_[6];
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