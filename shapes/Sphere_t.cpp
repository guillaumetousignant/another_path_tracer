#include "Sphere_t.h"
#include "TransformMatrix_t.h"

Sphere_t::Sphere_t(Material_t *material, TransformMatrix_t *transform_matrix): Shape_t(material, transform_matrix){

}

Sphere_t::~Sphere_t(){

}

void Sphere_t::update(){

}

void Sphere_t::intersection(const Ray_t &ray, bool &intersected, double &t, double (&uv)[2]) const {

}

void Sphere_t::normaluv(const Ray_t &ray, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec) const {

}

void Sphere_t::normal(const Ray_t &ray, const double (&uv)[2], Vec3f &normalvec) const {

}

Vec3f Sphere_t::mincoord() const {

}

Vec3f Sphere_t::maxcoord() const {
    
}