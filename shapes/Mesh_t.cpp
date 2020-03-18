#include "Mesh_t.h"
#include "MeshGeometry_t.h"
#include "MaterialMap_t.h"
#include "TriangleMesh_t.h"
#include <iostream>
#include <limits>

APTracer::Shapes::Mesh_t::Mesh_t(APTracer::Entities::Material_t *material, APTracer::Entities::TransformMatrix_t *transform_matrix, APTracer::Entities::MeshGeometry_t* geom) 
    : MeshTop_t(material, transform_matrix, geom) {
    
    createTriangles();
}

APTracer::Shapes::Mesh_t::Mesh_t(APTracer::Entities::MaterialMap_t *materialmap, APTracer::Entities::TransformMatrix_t *transform_matrix, APTracer::Entities::MeshGeometry_t* geom) 
    : MeshTop_t(materialmap->getFirst(), transform_matrix, geom) {

    createTriangles(materialmap);
}        

APTracer::Shapes::Mesh_t::~Mesh_t(){}

void APTracer::Shapes::Mesh_t::createTriangles(){    
    triangles_ = new APTracer::Entities::Shape_t*[n_tris_];
    for (unsigned int i = 0; i < n_tris_; i++){
        triangles_[i] = new APTracer::Shapes::TriangleMesh_t(material_, transformation_, geom_, i);
    }
}

void APTracer::Shapes::Mesh_t::createTriangles(APTracer::Entities::MaterialMap_t *materialmap){
    triangles_ = new APTracer::Entities::Shape_t*[n_tris_];
    for (unsigned int i = 0; i < n_tris_; i++){
        triangles_[i] = new APTracer::Shapes::TriangleMesh_t(materialmap->getMaterial(geom_->mat_[i]), transformation_, geom_, i);
    }
}