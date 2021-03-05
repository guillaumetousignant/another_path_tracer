#include "entities/Scene_t.h"
#include "entities/Shape_t.h"
#include "entities/AccelerationStructure_t.h"
#include "shapes/MeshTop_t.h"
#include <limits>
#include <algorithm>
#include "acceleration/AccelerationMultiGridVector_t.h"

using APTracer::Entities::Scene_t;
using APTracer::Entities::Shape_t;
using APTracer::Shapes::MeshTop_t;
using APTracer::Acceleration::AccelerationMultiGridVector_t;

Scene_t::Scene_t() {}

Scene_t::Scene_t(Shape_t* shape) : geometry_(1, shape) {}

Scene_t::Scene_t(Shape_t** shapes, size_t n_shapes) : geometry_(shapes, shapes + n_shapes) {}

Scene_t::Scene_t(MeshTop_t* mesh) : geometry_(mesh->triangles_, mesh->triangles_ + mesh->n_tris_) {}

Scene_t::Scene_t(MeshTop_t** meshes, size_t n_meshes) {
    for (size_t i = 0; i < n_meshes; i++) {
        geometry_.insert(geometry_.end(), meshes[i]->triangles_, meshes[i]->triangles_ + meshes[i]->n_tris_);
    }
}

auto Scene_t::add(Shape_t* shape) -> void {
    geometry_.push_back(shape);
}

auto Scene_t::add(Shape_t** shapes, size_t n_shapes) -> void {
    geometry_.insert(geometry_.end(), shapes, shapes + n_shapes);
}

auto Scene_t::add(MeshTop_t* mesh) -> void {
    geometry_.insert(geometry_.end(), mesh->triangles_, mesh->triangles_ + mesh->n_tris_);
}

auto Scene_t::add(MeshTop_t** meshes, size_t n_meshes) -> void {
    for (size_t i = 0; i < n_meshes; i++) {
        geometry_.insert(geometry_.end(), meshes[i]->triangles_, meshes[i]->triangles_ + meshes[i]->n_tris_);
    }
}

auto Scene_t::remove(Shape_t* shape) -> void {
    for (size_t i = 0; i < geometry_.size(); ++i) {
        if (geometry_[i] == shape) {
            geometry_.erase(geometry_.begin() + i);
            break;
        }
    }
}

auto Scene_t::remove(Shape_t** shapes, size_t n_shapes) -> void {
    for (size_t j = 0; j < n_shapes; ++j) {
        for (size_t i = 0; i < geometry_.size(); ++i) {
            if (geometry_[i] == shapes[j]) {
                geometry_.erase(geometry_.begin() + i);
                break;
            }
        }
    }
}

auto Scene_t::remove(MeshTop_t* mesh) -> void {
    if (mesh->n_tris_ > 0) {
        for (size_t i = 0; i < geometry_.size(); ++i) {
            if (geometry_[i] == mesh->triangles_[0]) {
                geometry_.erase(geometry_.begin() + i, geometry_.begin() + std::max(i + mesh->n_tris_, geometry_.size()));
                break;
            }
        }
    }
}

auto Scene_t::remove(MeshTop_t** meshes, size_t n_meshes) -> void {
    for (size_t k = 0; k < n_meshes; ++k) {
        if (meshes[k]->n_tris_ > 0) {
            for (size_t i = 0; i < geometry_.size(); ++i) {
                if (geometry_[i] == meshes[k]->triangles_[0]) {
                    geometry_.erase(geometry_.begin() + i, geometry_.begin() + std::max(i + meshes[k]->n_tris_, geometry_.size()));
                    break;
                }
            }
        }
    }
}

auto Scene_t::update() -> void {
    for (auto* shape: geometry_) {
        shape->update();
    }
}

auto Scene_t::build_acc() -> void {
    acc_ = std::unique_ptr<AccelerationStructure_t>(new AccelerationMultiGridVector_t(geometry_.data(), geometry_.size(), 1, 128, 32, 1));
}

auto Scene_t::intersect_brute(const Ray_t &ray, double &t, std::array<double, 2> &uv) const -> Shape_t* {
    double t_temp;
    std::array<double, 2> uv_temp;
    
    t = std::numeric_limits<double>::infinity();
    Shape_t* hit_obj = nullptr;

    for (auto* shape: geometry_) {
        if (shape->intersection(ray, t_temp, uv_temp) && (t_temp < t)) {
            hit_obj = shape;
            uv = uv_temp;
            t = t_temp;
        }
    }
    return hit_obj;
}

auto Scene_t::intersect(const Ray_t &ray, double &t, std::array<double, 2> &uv) const -> Shape_t* {
    return acc_->intersect(ray, t, uv);
}