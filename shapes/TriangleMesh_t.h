#ifndef TRIANGLEMESH_T_H
#define TRIANGLEMESH_T_H

#include "Ray_t.h"
#include "Vec3f.h"
#include "TriangleTop_t.h"

class TransformMatrix_t;
class Material_t;
class MeshGeometry_t;

class TriangleMesh_t : public TriangleTop_t{
    public:
        TriangleMesh_t(Material_t *material, TransformMatrix_t *transform_matrix, MeshGeometry_t* geom, unsigned int index);
        virtual ~TriangleMesh_t();

        MeshGeometry_t* geom_;
        unsigned int index_;

        virtual void update();
        virtual void normaluv(const Ray_t &ray, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec) const;
};
#endif