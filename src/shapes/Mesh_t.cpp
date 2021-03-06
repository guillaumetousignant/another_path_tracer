#include "shapes/Mesh_t.h"
#include "entities/MeshGeometry_t.h"
#include "entities/MaterialMap_t.h"
#include "shapes/TriangleMesh_t.h"
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

APTracer::Shapes::Mesh_t::~Mesh_t() = default;

APTracer::Shapes::Mesh_t::Mesh_t(const APTracer::Shapes::Mesh_t& other)
        : MeshTop_t(other.material_, other.transformation_, other.geom_) {
    createTriangles();
}

APTracer::Shapes::Mesh_t::Mesh_t(APTracer::Shapes::Mesh_t&& other)
        : MeshTop_t(other.material_, other.transformation_, other.geom_) {
    triangles_ = std::move(other.triangles_);
}

APTracer::Shapes::Mesh_t& APTracer::Shapes::Mesh_t::operator=(const APTracer::Shapes::Mesh_t& other) {
    material_ = other.material_;
    transformation_ = other.transformation_;
    geom_ = other.geom_;
    n_tris_ = other.n_tris_;
    
    for (auto* triangle: triangles_) {
        delete triangle;
    }
    createTriangles();

    return *this;
}

APTracer::Shapes::Mesh_t& APTracer::Shapes::Mesh_t::operator=(APTracer::Shapes::Mesh_t&& other) {
    material_ = other.material_;
    transformation_ = other.transformation_;
    geom_ = other.geom_;
    n_tris_ = other.n_tris_;
    triangles_ = std::move(other.triangles_);
    
    return *this;
}

void APTracer::Shapes::Mesh_t::createTriangles() {    
    triangles_ = std::vector<APTracer::Entities::Shape_t*>(n_tris_);
    for (size_t i = 0; i < n_tris_; i++) {
        triangles_[i] = new APTracer::Shapes::TriangleMesh_t(material_, transformation_, geom_, i);
    }
}

void APTracer::Shapes::Mesh_t::createTriangles(APTracer::Entities::MaterialMap_t *materialmap) {
    triangles_ = std::vector<APTracer::Entities::Shape_t*>(n_tris_);
    for (size_t i = 0; i < n_tris_; i++) {
        triangles_[i] = new APTracer::Shapes::TriangleMesh_t(materialmap->getMaterial(geom_->mat_[i]), transformation_, geom_, i);
    }
}