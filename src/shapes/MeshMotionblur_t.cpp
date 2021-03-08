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

APTracer::Shapes::MeshMotionblur_t::~MeshMotionblur_t() = default;  

APTracer::Shapes::MeshMotionblur_t::MeshMotionblur_t(const APTracer::Shapes::MeshMotionblur_t& other)
        : MeshTop_t(other.material_, other.transformation_, other.geom_) {
    createTriangles();
}

APTracer::Shapes::MeshMotionblur_t::MeshMotionblur_t(APTracer::Shapes::MeshMotionblur_t&& other) noexcept
        : MeshTop_t(other.material_, other.transformation_, other.geom_) {
    triangles_ = std::move(other.triangles_);
}

auto APTracer::Shapes::MeshMotionblur_t::operator=(const APTracer::Shapes::MeshMotionblur_t& other) -> APTracer::Shapes::MeshMotionblur_t& {
    if (&other == this) {
        return *this;
    }
    
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

auto APTracer::Shapes::MeshMotionblur_t::operator=(APTracer::Shapes::MeshMotionblur_t&& other) noexcept -> APTracer::Shapes::MeshMotionblur_t& {
    material_ = other.material_;
    transformation_ = other.transformation_;
    geom_ = other.geom_;
    n_tris_ = other.n_tris_;
    triangles_ = std::move(other.triangles_);
    
    return *this;
}

auto APTracer::Shapes::MeshMotionblur_t::createTriangles() -> void {
    triangles_ = std::vector<APTracer::Entities::Shape_t*>(n_tris_);
    for (size_t i = 0; i < n_tris_; i++) {
        triangles_[i] = new APTracer::Shapes::TriangleMeshMotionblur_t(material_, transformation_, geom_, i);
    }
}

auto APTracer::Shapes::MeshMotionblur_t::createTriangles(MaterialMap_t *materialmap) -> void {
    triangles_ = std::vector<APTracer::Entities::Shape_t*>(n_tris_);
    for (size_t i = 0; i < n_tris_; i++) {
        triangles_[i] = new APTracer::Shapes::TriangleMeshMotionblur_t(materialmap->getMaterial(geom_->mat_[i]), transformation_, geom_, i);
    }
}