#include "Mesh_t.h"

Mesh_t::Mesh_t(Material_t *material, TransformMatrix_t *transform_matrix): Shape_t(material, transform_matrix){

}

Mesh_t::~Mesh_t(){

}

void Mesh_t::update(){

}

void Mesh_t::intersection(const Ray_t &ray, bool &intersected, double &t, double (&uv)[2]) const {

}

void Mesh_t::normaluv(const Ray_t &ray, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec) const {

}

void Mesh_t::normal(const Ray_t &ray, const double (&uv)[2], Vec3f &normalvec) const {

}

Vec3f Mesh_t::mincoord() const {
    return Vec3f();
}

Vec3f Mesh_t::maxcoord() const {
    return Vec3f();
}

void Mesh_t::createTriangles(){

}