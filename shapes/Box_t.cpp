#include "Box_t.h"
#include "TransformMatrix_t.h"
#include "Material_t.h"

Box_t::Box_t(Material_t *material, TransformMatrix_t *transform_matrix, Vec3f (&coord)[2]) 
    : Shape_t(material, transform_matrix), coordinates_{coord[0], coord[1]} {}

Box_t::~Box_t(){}

void Box_t::update(){

}

void Box_t::intersection(const Ray_t &ray, bool &intersected, double &t, double (&uv)[2]) const {

}

void Box_t::normaluv(const Ray_t &ray, const double (&uv)[2], double (&tuv)[2], Vec3f &normalvec) const {

}

void Box_t::normal(const Ray_t &ray, const double (&uv)[2], Vec3f &normalvec) const {

}

Vec3f Box_t::mincoord() const {

}

Vec3f Sphere_t::maxcoord() const {

}