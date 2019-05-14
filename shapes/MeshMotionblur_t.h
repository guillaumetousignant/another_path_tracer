#ifndef MESHMOTIONBLUR_T_H
#define MESHMOTIONBLUR_T_H

#include "Ray_t.h"
#include "Vec3f.h"
#include "MeshTop_t.h"

class Material_t;
class TransformMatrix_t;
class MeshGeometry_t;
class MaterialMap_t;

class MeshMotionblur_t : public MeshTop_t{
    public:
        MeshMotionblur_t(Material_t *material, TransformMatrix_t *transform_matrix, MeshGeometry_t* geom);
        MeshMotionblur_t(MaterialMap_t *materialmap, TransformMatrix_t *transform_matrix, MeshGeometry_t* geom);
        virtual ~MeshMotionblur_t();

        virtual void createTriangles();
        virtual void createTriangles(MaterialMap_t *materialmap);
};
#endif