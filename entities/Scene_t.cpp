#include "Scene_t.h"
#include "Shape_t.h"
#include "AccelerationStructure_t.h"
#include "MeshTop_t.h"
#include <limits>
#include "AccelerationMultiGridVector_t.h"

Scene_t::Scene_t() : geometry_(nullptr), n_obj_(0), acc_(nullptr) {}

Scene_t::~Scene_t() {
    if (geometry_ != nullptr){
        delete [] geometry_;
    }
    if (acc_ != nullptr){
        delete acc_;
    }
}

void Scene_t::add(Shape_t* shape) {
    Shape_t** geometry_2 = new Shape_t*[n_obj_+1];
    for (unsigned int i = 0; i < n_obj_; i++){
        geometry_2[i] = geometry_[i];
    }
    geometry_2[n_obj_] = shape;
    if (geometry_ != nullptr){
        delete [] geometry_;
    }
    geometry_ = geometry_2;
    n_obj_++;
}

void Scene_t::add(Shape_t** shapes, unsigned int n_shapes) {
    Shape_t** geometry_2 = new Shape_t*[n_obj_+n_shapes];
    for (unsigned int i = 0; i < n_obj_; i++){
        geometry_2[i] = geometry_[i];
    }
    for (unsigned int i = 0; i < n_shapes; i++){
        geometry_2[i+n_obj_] = shapes[i];
    }
    if (geometry_ != nullptr){
        delete [] geometry_;
    }
    geometry_ = geometry_2;
    n_obj_ += n_shapes;
}

void Scene_t::add(MeshTop_t* mesh) {
    Shape_t** geometry_2 = new Shape_t*[n_obj_+mesh->n_tris_];
    for (unsigned int i = 0; i < n_obj_; i++){
        geometry_2[i] = geometry_[i];
    }
    for (unsigned int i = 0; i < mesh->n_tris_; i++){
        geometry_2[i+n_obj_] = mesh->triangles_[i];
    }
    if (geometry_ != nullptr){
        delete [] geometry_;
    }
    geometry_ = geometry_2;
    n_obj_ += mesh->n_tris_;
}

void Scene_t::add(MeshTop_t** meshes, unsigned int n_meshes) {
    unsigned int n_tris = 0;
    unsigned int index = 0;
    for (unsigned int i = 0; i < n_meshes; i++){
        n_tris += meshes[i]->n_tris_;
    }
    Shape_t** geometry_2 = new Shape_t*[n_obj_+n_tris];
    for (unsigned int i = 0; i < n_obj_; i++){
        geometry_2[i] = geometry_[i];
    }
    for (unsigned int j = 0; j < n_meshes; j++){
        for (unsigned int i = 0; i < meshes[j]->n_tris_; i++){
            geometry_2[i+n_obj_+index] = meshes[j]->triangles_[i];
        }
        index += meshes[j]->n_tris_;
    }
    if (geometry_ != nullptr){
        delete [] geometry_;
    }
    geometry_ = geometry_2;
    n_obj_ += n_tris;
}

void Scene_t::remove(Shape_t* shape) {

}

void Scene_t::remove(Shape_t** shapes, unsigned int n_shapes) {

}

void Scene_t::remove(MeshTop_t* mesh) {

}

void Scene_t::remove(MeshTop_t** meshes, unsigned int n_meshes) {

}

void Scene_t::update() {
    for (unsigned int i = 0; i < n_obj_; i++){
        geometry_[i]->update();
    }
}

void Scene_t::build_acc() {
    if (acc_ != nullptr){
        delete acc_;
    }
    acc_ = new AccelerationMultiGridVector_t(geometry_, n_obj_);
}

Shape_t* Scene_t::intersect_brute(const Ray_t &ray, double &t, double (&uv)[2]) const {
    double t_temp;
    double uv_temp[2];
    
    t = std::numeric_limits<double>::infinity();
    Shape_t* hit_obj = nullptr;

    for (unsigned int i = 0; i < n_obj_; i++){
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