#include "Material_t.h"
#include "TransformMatrix_t.h"
#include "Ray_t.h"
#include "Vec3f.h"

#ifndef SHAPE_T_H
#define SHAPE_T_H

class Shape_t{
    public:
        Shape_t(const Material_t *material, const TransformMatrix_t *transform_matrix): material_(material), transform_matrix_(transform_matrix){};
        virtual ~Shape_t(){};

        virtual void update() = 0;
        virtual void intersection(const Ray_t &ray, bool &intersected, double &t, double (&uv)[2]) const = 0; 
        virtual void normaluv(const Ray_t &ray, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec) const = 0;
        virtual void normal(const Ray_t &ray, const double (&uv)[2], Vec3f &normalvec) const = 0;
        virtual void mincoord(Vec3f &coord) const = 0;
        virtual void maxcoord(Vec3f &coord) const = 0;

        Material_t *material_;
        TransformMatrix_t *transform_matrix_;
};
#endif