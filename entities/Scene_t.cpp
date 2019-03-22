#include "Scene_t.h"
#include "Shape_t.h"
#include "AccelerationStructure_t.h"
#include "Mesh_t.h"
#include <limits>

Scene_t::Scene_t() : geometry_(nullptr), n_obj_(0), acc_(nullptr) {}

Scene_t::~Scene_t() {
    if (geometry_ != nullptr){
        delete [] geometry_;
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

}

void Scene_t::add(Mesh_t* mesh) {
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

void Scene_t::add(Mesh_t** meshes, unsigned int n_meshes) {

}

void Scene_t::remove(Shape_t* shape) {

}

void Scene_t::remove(Shape_t** shapes, unsigned int n_shapes) {

}

void Scene_t::remove(Mesh_t* mesh) {

}

void Scene_t::remove(Mesh_t** meshes, unsigned int n_meshes) {

}

void Scene_t::update() {
    for (unsigned int i = 0; i < n_obj_; i++){
        geometry_[i]->update();
    }
}

void Scene_t::build_acc() {

}

void Scene_t::intersect_brute(Ray_t &ray, Shape_t* &hit_obj, double &t, double (&uv)[2]) const {
    double t_temp;
    bool intersected;
    double uv_temp[2];
    
    t = std::numeric_limits<double>::infinity();

    for (unsigned int i = 0; i < n_obj_; i++){
        geometry_[i]->intersection(ray, intersected, t_temp, uv_temp);
        if (intersected){
            if (t_temp < t){
                hit_obj = geometry_[i];
                uv[0] = uv_temp[0];
                uv[1] = uv_temp[1];
                t = t_temp;
            }
        }
    }
}

void Scene_t::intersect(Ray_t &ray, Shape_t* &hit_obj, double &t, double (&uv)[2]) const {
    intersect_brute(ray, hit_obj, t, uv);
}