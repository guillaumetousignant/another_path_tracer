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

Scene_t::Scene_t() : geometry_(), acc_(nullptr) {}

Scene_t::Scene_t(Shape_t* shape) : geometry_(1, shape), acc_(nullptr) {}

Scene_t::Scene_t(Shape_t** shapes, size_t n_shapes) : geometry_(shapes, shapes + n_shapes), acc_(nullptr) {}

Scene_t::Scene_t(MeshTop_t* mesh) : geometry_(mesh->triangles_, mesh->triangles_ + mesh->n_tris_), acc_(nullptr) {}

Scene_t::Scene_t(MeshTop_t** meshes, size_t n_meshes) : geometry_(), acc_(nullptr) {
    for (size_t i = 0; i < n_meshes; i++){
        geometry_.insert(geometry_.end(), meshes[i]->triangles_, meshes[i]->triangles_ + meshes[i]->n_tris_);
    }
}

Scene_t::~Scene_t() {
    if (acc_ != nullptr){
        delete acc_;
    }
}

void Scene_t::add(Shape_t* shape) {
    geometry_.push_back(shape);
}

void Scene_t::add(Shape_t** shapes, size_t n_shapes) {
    geometry_.insert(geometry_.end(), shapes, shapes + n_shapes);
}

void Scene_t::add(MeshTop_t* mesh) {
    geometry_.insert(geometry_.end(), mesh->triangles_, mesh->triangles_ + mesh->n_tris_);
}

void Scene_t::add(MeshTop_t** meshes, size_t n_meshes) {
    for (size_t i = 0; i < n_meshes; i++){
        geometry_.insert(geometry_.end(), meshes[i]->triangles_, meshes[i]->triangles_ + meshes[i]->n_tris_);
    }
}

void Scene_t::remove(Shape_t* shape) {
    for (size_t i = 0; i < geometry_.size(); ++i) {
        if (geometry_[i] == shape) {
            geometry_.erase(geometry_.begin() + i);
            break;
        }
    }
}

void Scene_t::remove(Shape_t** shapes, size_t n_shapes) {
    for (size_t j = 0; j < n_shapes; ++j) {
        for (size_t i = 0; i < geometry_.size(); ++i) {
            if (geometry_[i] == shapes[j]) {
                geometry_.erase(geometry_.begin() + i);
                break;
            }
        }
    }
}

void Scene_t::remove(MeshTop_t* mesh) {
    if (mesh->n_tris_ > 0) {
        for (size_t i = 0; i < geometry_.size(); ++i) {
            if (geometry_[i] == mesh->triangles_[0]) {
                geometry_.erase(geometry_.begin() + i, geometry_.begin() + std::max(i + mesh->n_tris_, geometry_.size()));
                break;
            }
        }
    }
}

void Scene_t::remove(MeshTop_t** meshes, size_t n_meshes) {
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

void Scene_t::update() {
    for (size_t i = 0; i < geometry_.size(); i++){
        geometry_[i]->update();
    }
}

void Scene_t::build_acc() {
    if (acc_ != nullptr){
        delete acc_;
    }
    acc_ = new AccelerationMultiGridVector_t(geometry_.data(), geometry_.size());
}

Shape_t* Scene_t::intersect_brute(const Ray_t &ray, double &t, double (&uv)[2]) const {
    double t_temp;
    double uv_temp[2];
    
    t = std::numeric_limits<double>::infinity();
    Shape_t* hit_obj = nullptr;

    for (size_t i = 0; i < geometry_.size(); i++){
        if (geometry_[i]->intersection(ray, t_temp, uv_temp) && (t_temp < t)){
            hit_obj = geometry_[i];
            uv[0] = uv_temp[0];
            uv[1] = uv_temp[1];
            t = t_temp;
        }
    }
    return hit_obj;
}

Shape_t* Scene_t::intersect(const Ray_t &ray, double &t, double (&uv)[2]) const {
    return acc_->intersect(ray, t, uv);
}