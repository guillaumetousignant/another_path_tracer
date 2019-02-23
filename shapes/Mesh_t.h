#include "Material_t.h"
#include "TransformMatrix_t.h"
#include "Ray_t.h"
#include "Vec3f.h"
#include "Shape_t.h"
#include "MeshGeometry_t.h"

#ifndef MESH_T_H
#define MESH_T_H

class Mesh_t : public Shape_t{
    public:
        Mesh_t(const Material_t *material, const TransformMatrix_t *transform_matrix): Shape_t(material, transform_matrix){};
        virtual ~Mesh_t(){};

        MeshGeometry_t* geom_;
        unsigned int n_tris_;
        Shape_t** triangled_; // Maybe should be triangle**?

        virtual void update(){};
        virtual void intersection(const Ray_t *ray, bool &intersected, double &t, double (&uv)[2]) const {}; // Never use this
        virtual void normaluv(const Ray_t *ray, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec) const {};
        virtual void normal(const Ray_t *ray, const double (&uv)[2], Vec3f &normalvec) const {};
        virtual void mincoord(Vec3f &coord) const {};
        virtual void maxcoord(Vec3f &coord) const {};
        virtual void createTriangles(){};
};
#endif