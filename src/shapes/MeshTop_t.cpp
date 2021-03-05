#include "shapes/MeshTop_t.h"
#include "entities/MeshGeometry_t.h"
#include "shapes/TriangleMesh_t.h"
#include <iostream>
#include <limits>

using APTracer::Entities::Vec3f;

APTracer::Shapes::MeshTop_t::MeshTop_t(APTracer::Entities::Material_t *material, APTracer::Entities::TransformMatrix_t *transform_matrix, APTracer::Entities::MeshGeometry_t* geom) 
    : material_(material), transformation_(transform_matrix), geom_(geom), n_tris_(geom->n_tris_) {
}      

void APTracer::Shapes::MeshTop_t::update() {
    for (auto* triangle: triangles_) {
        triangle->update();
    }
}

void APTracer::Shapes::MeshTop_t::intersection(const APTracer::Entities::Ray_t &ray, bool &intersected, double &t, std::array<double, 2> &uv) const {
    t = std::numeric_limits<double>::infinity();
    intersected = false;
    uv = {0.0, 0.0};
    std::cout << "Warning, do not intersect meshes directly." << std::endl;   
}

void APTracer::Shapes::MeshTop_t::normaluv(const APTracer::Entities::Ray_t &ray, std::array<double, 2> uv, std::array<double, 2> &tuv, Vec3f &normalvec) const {
    tuv = {0.0, 0.0};
    normalvec = Vec3f();
    std::cout << "Warning, do not calculate normals on meshes." << std::endl;
}

void APTracer::Shapes::MeshTop_t::normal(const APTracer::Entities::Ray_t &ray, std::array<double, 2> uv, Vec3f &normalvec) const {
    normalvec = Vec3f();
    std::cout << "Warning, do not calculate normals on meshes." << std::endl;
}

Vec3f APTracer::Shapes::MeshTop_t::mincoord() const {
    Vec3f coord = Vec3f(std::numeric_limits<double>::infinity());
    for (auto* triangle: triangles_) {
        coord.min(triangle->mincoord());
    }    
    return coord;
}

Vec3f APTracer::Shapes::MeshTop_t::maxcoord() const {
    Vec3f coord = Vec3f(-std::numeric_limits<double>::infinity());
    for (auto* triangle: triangles_) {
        coord.max(triangle->maxcoord());
    }
    return coord;
}