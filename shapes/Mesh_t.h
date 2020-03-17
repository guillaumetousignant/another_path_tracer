#ifndef MESH_T_H
#define MESH_T_H

#include "Ray_t.h"
#include "Vec3f.h"
#include "MeshTop_t.h"

using APTracer::Entities::Material_t;
using APTracer::Entities::TransformMatrix_t;
using APTracer::Entities::MeshGeometry_t;
using APTracer::Entities::MaterialMap_t;

class Material_t;
class TransformMatrix_t;
class MeshGeometry_t;
class MaterialMap_t;

namespace APTracer::Shapes {
    class Mesh_t final : public MeshTop_t{
        public:
            Mesh_t(Material_t *material, TransformMatrix_t *transform_matrix, MeshGeometry_t* geom);
            Mesh_t(MaterialMap_t *materialmap, TransformMatrix_t *transform_matrix, MeshGeometry_t* geom);
            virtual ~Mesh_t() final;

            virtual void createTriangles() final;
            virtual void createTriangles(MaterialMap_t *materialmap) final;
    };
}
#endif