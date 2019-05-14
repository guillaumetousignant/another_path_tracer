#include "MeshMotionblur_t.h"
#include "MeshGeometry_t.h"
#include "MaterialMap_t.h"
#include "TriangleMeshMotionblur_t.h"
#include <iostream>
#include <limits>

MeshMotionblur_t::MeshMotionblur_t(Material_t *material, TransformMatrix_t *transform_matrix, MeshGeometry_t* geom) 
    : MeshTop_t(material, transform_matrix, geom) {
    createTriangles();
}

MeshMotionblur_t::MeshMotionblur_t(MaterialMap_t *materialmap, TransformMatrix_t *transform_matrix, MeshGeometry_t* geom) 
    : MeshTop_t(materialmap->getFirst(), transform_matrix, geom) {
    createTriangles(materialmap);
}      

MeshMotionblur_t::~MeshMotionblur_t(){}  

void MeshMotionblur_t::createTriangles(){
    std::cout << "Motion function" << std::endl; // REMOVE

    triangles_ = new Shape_t*[n_tris_];
    for (unsigned int i = 0; i < n_tris_; i++){
        triangles_[i] = new TriangleMeshMotionblur_t(material_, transformation_, geom_, i);
    }
}

void MeshMotionblur_t::createTriangles(MaterialMap_t *materialmap){
    triangles_ = new Shape_t*[n_tris_];
    for (unsigned int i = 0; i < n_tris_; i++){
        triangles_[i] = new TriangleMeshMotionblur_t(materialmap->getMaterial(geom_->mat_[i]), transformation_, geom_, i);
    }
}