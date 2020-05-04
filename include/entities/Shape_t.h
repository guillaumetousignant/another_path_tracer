#ifndef SHAPE_T_H
#define SHAPE_T_H

#include "entities/Ray_t.h"
#include "entities/Vec3f.h"

namespace APTracer { namespace Entities {
    class Material_t;
    class TransformMatrix_t;

    class Shape_t{
        public:
            Shape_t(Material_t *material, TransformMatrix_t *transform_matrix): material_(material), transformation_(transform_matrix){};
            virtual ~Shape_t(){};

            virtual void update() = 0;
            virtual bool intersection(const Ray_t &ray, double &t, double (&uv)[2]) const = 0;
            virtual void normal(const Ray_t &ray, const double (&uv)[2], Vec3f &normalvec) const = 0;
            virtual void normaluv(const Ray_t &ray, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec) const = 0;
            virtual void normal_uv_tangent(const Ray_t &ray, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec, Vec3f &tangentvec) const = 0;
            virtual Vec3f mincoord() const = 0;
            virtual Vec3f maxcoord() const = 0;

            Material_t *material_;
            TransformMatrix_t *transformation_;
    };
}}
#endif