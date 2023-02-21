#include "shapes/MeshTop_t.hpp"
#include "entities/MeshGeometry_t.hpp"
#include "shapes/TriangleMesh_t.hpp"
#include <iostream>
#include <limits>

using APTracer::Entities::Vec3f;

APTracer::Shapes::MeshTop_t::MeshTop_t(APTracer::Entities::Material_t* material, APTracer::Entities::TransformMatrix_t* transform_matrix, APTracer::Entities::MeshGeometry_t* geom) :
        material_(material), transformation_(transform_matrix), geom_(geom) {}

auto APTracer::Shapes::MeshTop_t::update() -> void {
    for (const auto& triangle: triangles_) {
        triangle->update();
    }
}

auto APTracer::Shapes::MeshTop_t::intersection(const APTracer::Entities::Ray_t& ray, bool& intersected, double& t, std::array<double, 2>& uv) const -> void {
    intersected = false;
    std::cout << "Warning, do not intersect meshes directly." << std::endl;
}

auto APTracer::Shapes::MeshTop_t::normaluv(const APTracer::Entities::Ray_t& ray, std::array<double, 2> uv, std::array<double, 2>& tuv, Vec3f& normalvec) const -> void {
    tuv       = {0.0, 0.0};
    normalvec = Vec3f();
    std::cout << "Warning, do not calculate normals on meshes." << std::endl;
}

auto APTracer::Shapes::MeshTop_t::normal(const APTracer::Entities::Ray_t& ray, std::array<double, 2> uv, Vec3f& normalvec) const -> void {
    normalvec = Vec3f();
    std::cout << "Warning, do not calculate normals on meshes." << std::endl;
}

auto APTracer::Shapes::MeshTop_t::mincoord() const -> Vec3f {
    Vec3f coord = Vec3f(std::numeric_limits<double>::max());
    for (const auto& triangle: triangles_) {
        coord.min(triangle->mincoord());
    }
    return coord;
}

auto APTracer::Shapes::MeshTop_t::maxcoord() const -> Vec3f {
    Vec3f coord = Vec3f(std::numeric_limits<double>::lowest());
    for (const auto& triangle: triangles_) {
        coord.max(triangle->maxcoord());
    }
    return coord;
}