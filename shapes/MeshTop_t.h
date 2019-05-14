#ifndef MESHTOP_T_H
#define MESHTOP_T_H

//#include "Material_t.h"
//#include "TransformMatrix_t.h"
#include "Ray_t.h"
#include "Vec3f.h"
#include "Shape_t.h"
//#include "MeshGeometry_t.h"

class Material_t;
class TransformMatrix_t;
class MeshGeometry_t;
class MaterialMap_t;

class MeshTop_t : public Shape_t{
    public:
        MeshTop_t(Material_t *material, TransformMatrix_t *transform_matrix, MeshGeometry_t* geom);
        virtual ~MeshTop_t();

        MeshGeometry_t* geom_;
        unsigned int n_tris_;
        Shape_t** triangles_; // Maybe should be triangle**?

        virtual void update();
        virtual void intersection(const Ray_t &ray, bool &intersected, double &t, double (&uv)[2]) const; // Never use this
        virtual void normaluv(const Ray_t &ray, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec) const;
        virtual void normal(const Ray_t &ray, const double (&uv)[2], Vec3f &normalvec) const;
        virtual Vec3f mincoord() const;
        virtual Vec3f maxcoord() const;
        virtual void createTriangles() = 0;
        virtual void createTriangles(MaterialMap_t *materialmap) = 0;
};
#endif