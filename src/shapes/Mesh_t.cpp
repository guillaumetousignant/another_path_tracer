#include "shapes/Mesh_t.hpp"
#include "shapes/TriangleMesh_t.hpp"
#include <iostream>

APTracer::Shapes::Mesh_t::Mesh_t(APTracer::Entities::Material_t* material, APTracer::Entities::TransformMatrix_t* transform_matrix, APTracer::Entities::MeshGeometry_t* geom) :
        MeshTop_t(material, transform_matrix, geom) {

    createTriangles();
}

APTracer::Shapes::Mesh_t::Mesh_t(APTracer::Entities::MaterialMap_t* materialmap, APTracer::Entities::TransformMatrix_t* transform_matrix, APTracer::Entities::MeshGeometry_t* geom) :
        MeshTop_t(materialmap->getFirst(), transform_matrix, geom) {

    createTriangles(materialmap);
}

auto APTracer::Shapes::Mesh_t::createTriangles() -> void {
    triangles_ = std::vector<std::unique_ptr<APTracer::Entities::Shape_t>>(geom_->v_.size());
    for (size_t i = 0; i < triangles_.size(); i++) {
        triangles_[i] = std::make_unique<APTracer::Shapes::TriangleMesh_t>(material_, transformation_, geom_, i);
    }
}

auto APTracer::Shapes::Mesh_t::createTriangles(APTracer::Entities::MaterialMap_t* materialmap) -> void {
    triangles_ = std::vector<std::unique_ptr<APTracer::Entities::Shape_t>>(geom_->v_.size());
    for (size_t i = 0; i < triangles_.size(); i++) {
        triangles_[i] = std::make_unique<APTracer::Shapes::TriangleMesh_t>(materialmap->getMaterial(geom_->mat_[i]), transformation_, geom_, i);
    }
}
