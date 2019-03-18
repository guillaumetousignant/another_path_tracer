#ifndef TRIANGLE_T_H
#define TRIANGLE_T_H

//#include "Material_t.h"
//#include "TransformMatrix_t.h"
#include "Ray_t.h"
#include "Vec3f.h"
#include "TriangleTop_t.h"

class TransformMatrix_t;
class Material_t;

class Triangle_t : public TriangleTop_t{
    public:
        Triangle_t(Material_t *material, TransformMatrix_t *transform_matrix, Vec3f* points, Vec3f* normals, double** texcoord);
        virtual ~Triangle_t();

        Vec3f points_orig_[3];
        Vec3f normals_orig_[3];
        double texture_coordinates_[3][2];

        virtual void update();
        virtual void normaluv(const Ray_t &ray, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec) const;
};
#endif