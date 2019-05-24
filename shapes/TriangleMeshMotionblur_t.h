#ifndef TRIANGLEMESHMOTIONBLUR_T_H
#define TRIANGLEMESHMOTIONBLUR_T_H

#include "Ray_t.h"
#include "Vec3f.h"
#include "TriangleMesh_t.h"

class TransformMatrix_t;
class Material_t;
class MeshGeometry_t;

class TriangleMeshMotionblur_t : public TriangleMesh_t{
    public:
        TriangleMeshMotionblur_t(Material_t *material, TransformMatrix_t *transform_matrix, MeshGeometry_t* geom, unsigned int index);
        virtual ~TriangleMeshMotionblur_t();

        Vec3f points_last_[3];
        Vec3f normals_last_[3];
        Vec3f v0v1_last_;
        Vec3f v0v2_last_;

        virtual void update();
        virtual void intersection(const Ray_t &ray, bool &intersected, double &t, double (&uv)[2]) const; 
        virtual void normaluv(const Ray_t &ray, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec) const;
        virtual void normal(const Ray_t &ray, const double (&uv)[2], Vec3f &normalvec) const;
        virtual void normal_face(const Ray_t &ray, Vec3f &normalvec) const;
        virtual Vec3f mincoord() const;
        virtual Vec3f maxcoord() const;};
#endif