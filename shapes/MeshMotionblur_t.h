#ifndef MESHMOTIONBLUR_T_H
#define MESHMOTIONBLUR_T_H

//#include "Material_t.h"
//#include "TransformMatrix_t.h"
#include "Ray_t.h"
#include "Vec3f.h"
#include "Mesh_t.h"
//#include "MeshGeometry_t.h"

class Material_t;
class TransformMatrix_t;
class MeshGeometry_t;
class MaterialMap_t;

class MeshMotionblur_t : public Mesh_t{
    public:
        MeshMotionblur_t(Material_t *material, TransformMatrix_t *transform_matrix, MeshGeometry_t* geom);
        MeshMotionblur_t(MaterialMap_t *materialmap, TransformMatrix_t *transform_matrix, MeshGeometry_t* geom);
        
        virtual void createTriangles();
        virtual void createTriangles(MaterialMap_t *materialmap);
};
#endif