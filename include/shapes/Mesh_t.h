#ifndef MESH_T_H
#define MESH_T_H

#include "entities/Ray_t.h"
#include "entities/Vec3f.h"
#include "shapes/MeshTop_t.h"

namespace APTracer { namespace Entities {
    class Material_t;
    class TransformMatrix_t;
    class MeshGeometry_t;
    class MaterialMap_t;
}}

using APTracer::Entities::Material_t;
using APTracer::Entities::TransformMatrix_t;
using APTracer::Entities::MeshGeometry_t;
using APTracer::Entities::MaterialMap_t;
using APTracer::Shapes::MeshTop_t;

namespace APTracer { namespace Shapes {
    class Mesh_t final : public MeshTop_t{
        public:
            Mesh_t(Material_t *material, TransformMatrix_t *transform_matrix, MeshGeometry_t* geom);
            Mesh_t(MaterialMap_t *materialmap, TransformMatrix_t *transform_matrix, MeshGeometry_t* geom);
            virtual ~Mesh_t() final;

            virtual void createTriangles() final;
            virtual void createTriangles(MaterialMap_t *materialmap) final;
    };
}}
#endif