#include "TriangleTop_t.h"
#include "TransformMatrix_t.h"
#include "Material_t.h"
#include <math.h>
#include <limits>
#include "Referentials.h"

#define PI 3.141592653589793238463

TriangleTop_t::TriangleTop_t(Material_t *material, TransformMatrix_t *transform_matrix): Shape_t(material, transform_matrix){

}

TriangleTop_t::~TriangleTop_t(){}

void TriangleTop_t::intersection(const Ray_t &ray, bool &intersected, double &t, double (&uv)[2]) const {

}

void TriangleTop_t::normal(const Ray_t &ray, const double (&uv)[2], Vec3f &normalvec) const {

}

void TriangleTop_t::normal_face(Vec3f &normalvec) const{
    
}

Vec3f TriangleTop_t::mincoord() const {
    return Vec3f();
}

Vec3f TriangleTop_t::maxcoord() const {
    return Vec3f();
}