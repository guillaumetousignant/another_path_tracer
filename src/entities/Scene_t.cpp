#include "entities/Scene_t.h"
#include "acceleration/AccelerationMultiGridVector_t.h"
#include "entities/AccelerationStructure_t.h"
#include "entities/Shape_t.h"
#include "shapes/MeshTop_t.h"
#include <algorithm>
#include <limits>

using APTracer::Acceleration::AccelerationMultiGridVector_t;
using APTracer::Entities::Scene_t;
using APTracer::Entities::Shape_t;
using APTracer::Shapes::MeshTop_t;

Scene_t::Scene_t(Shape_t* shape) : geometry_(1, shape) {}

Scene_t::Scene_t(const std::vector<Shape_t*>& shapes) : geometry_(shapes) {}

Scene_t::Scene_t(MeshTop_t* mesh) : geometry_(mesh->triangles_.size()) {
    for (size_t i = 0; i < mesh->triangles_.size(); ++i) {
        geometry_[i] = mesh->triangles_[i].get();
    }
}

Scene_t::Scene_t(const std::vector<MeshTop_t*>& meshes) {
    size_t additional_size = 0;
    for (const auto& mesh: meshes) {
        additional_size += mesh->triangles_.size();
    }

    geometry_ = std::vector<Shape_t*>(additional_size);

    size_t index = 0;
    for (const auto& mesh: meshes) {
        for (size_t i = 0; i < mesh->triangles_.size(); ++i) {
            geometry_[index + i] = mesh->triangles_[i].get();
        }
        index += mesh->triangles_.size();
    }
}

auto Scene_t::add(Shape_t* shape) -> void {
    geometry_.push_back(shape);
}

auto Scene_t::add(const std::vector<Shape_t*>& shapes) -> void {
    geometry_.insert(geometry_.end(), shapes.begin(), shapes.end());
}

auto Scene_t::add(MeshTop_t* mesh) -> void {

    const size_t index = geometry_.size();
    geometry_.resize(geometry_.size() + mesh->triangles_.size());

    for (size_t i = 0; i < mesh->triangles_.size(); ++i) {
        geometry_[index + i] = mesh->triangles_[i].get();
    }
}

auto Scene_t::add(const std::vector<MeshTop_t*>& meshes) -> void {
    size_t additional_size = 0;
    for (const auto& mesh: meshes) {
        additional_size += mesh->triangles_.size();
    }

    size_t index = geometry_.size();
    geometry_.resize(geometry_.size() + additional_size);

    for (const auto& mesh: meshes) {
        for (size_t i = 0; i < mesh->triangles_.size(); ++i) {
            geometry_[index + i] = mesh->triangles_[i].get();
        }
        index += mesh->triangles_.size();
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

auto Scene_t::remove(const std::vector<Shape_t*>& shapes) -> void {
    for (const auto& shape: shapes) {
        for (size_t i = 0; i < geometry_.size(); ++i) {
            if (geometry_[i] == shape) {
                geometry_.erase(geometry_.begin() + i);
                break;
            }
        }
    }
}

auto Scene_t::remove(MeshTop_t* mesh) -> void {
    if (!mesh->triangles_.empty()) {
        for (size_t i = 0; i < geometry_.size(); ++i) {
            if (geometry_[i] == mesh->triangles_[0].get()) {
                geometry_.erase(geometry_.begin() + i, geometry_.begin() + std::max(i + mesh->triangles_.size(), geometry_.size()));
                break;
            }
        }
    }
}

auto Scene_t::remove(const std::vector<MeshTop_t*>& meshes) -> void {
    for (const auto& mesh: meshes) {
        if (!mesh->triangles_.empty()) {
            for (size_t i = 0; i < geometry_.size(); ++i) {
                if (geometry_[i] == mesh->triangles_[0].get()) {
                    geometry_.erase(geometry_.begin() + i, geometry_.begin() + std::max(i + mesh->triangles_.size(), geometry_.size()));
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
    acc_ = std::make_unique<AccelerationMultiGridVector_t>(geometry_, 1, 128, 32, 1);
}

auto Scene_t::intersect_brute(const Ray_t& ray, double& t, std::array<double, 2>& uv) const -> Shape_t* {
    double t_temp;
    std::array<double, 2> uv_temp;

    t                = std::numeric_limits<double>::max();
    Shape_t* hit_obj = nullptr;

    for (auto* shape: geometry_) {
        if (shape->intersection(ray, t_temp, uv_temp) && (t_temp < t)) {
            hit_obj = shape;
            uv      = uv_temp;
            t       = t_temp;
        }
    }
    return hit_obj;
}

auto Scene_t::intersect(const Ray_t& ray, double& t, std::array<double, 2>& uv) const -> Shape_t* {
    return acc_->intersect(ray, t, uv);
}