#include "Scene_t.h"
#include "Shape_t.h"
#include "AccelerationStructure_t.h"
#include "Mesh_t.h"

Scene_t::Scene_t() {

}

Scene_t::~Scene_t() {

}

void Scene_t::add(Shape_t* shape) {

}

void Scene_t::add(Shape_t** shapes, unsigned int n_shapes) {

}

void Scene_t::add(Mesh_t* mesh) {

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

void Scene_t::update() const {

}

void Scene_t::build_acc() {

}

void Scene_t::intersect_brute(Ray_t &ray, Shape_t* hit_obj, double &t, double (&uv)[2]) const {

}

void Scene_t::intersect(Ray_t &ray, Shape_t* hit_obj, double &t, double (&uv)[2]) const {

}