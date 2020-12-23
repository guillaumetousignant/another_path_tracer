#include "shapes/MeshMotionblur_t.h"
#include "entities/MeshGeometry_t.h"
#include "entities/MaterialMap_t.h"
#include "shapes/TriangleMeshMotionblur_t.h"
#include <iostream>
#include <limits>

APTracer::Shapes::MeshMotionblur_t::MeshMotionblur_t(APTracer::Entities::Material_t *material, APTracer::Entities::TransformMatrix_t *transform_matrix, APTracer::Entities::MeshGeometry_t* geom) 
    : MeshTop_t(material, transform_matrix, geom) {
    createTriangles();
}

APTracer::Shapes::MeshMotionblur_t::MeshMotionblur_t(APTracer::Entities::MaterialMap_t *materialmap, APTracer::Entities::TransformMatrix_t *transform_matrix, APTracer::Entities::MeshGeometry_t* geom) 
    : MeshTop_t(materialmap->getFirst(), transform_matrix, geom) {
    createTriangles(materialmap);
}      

APTracer::Shapes::MeshMotionblur_t::~MeshMotionblur_t() {}  

void APTracer::Shapes::MeshMotionblur_t::createTriangles() {
    triangles_ = new APTracer::Entities::Shape_t*[n_tris_];
    for (size_t i = 0; i < n_tris_; i++) {
        triangles_[i] = new APTracer::Shapes::TriangleMeshMotionblur_t(material_, transformation_, geom_, i);
    }
}

void APTracer::Shapes::MeshMotionblur_t::createTriangles(MaterialMap_t *materialmap) {
    triangles_ = new APTracer::Entities::Shape_t*[n_tris_];
    for (size_t i = 0; i < n_tris_; i++) {
        triangles_[i] = new APTracer::Shapes::TriangleMeshMotionblur_t(materialmap->getMaterial(geom_->mat_[i]), transformation_, geom_, i);
    }
}