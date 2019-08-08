#include "DistanceMaterial_t.h"
#include "Shape_t.h"
#include "Vec3f.h"
#include <cmath>

DistanceMaterial_t::DistanceMaterial_t(double focal_length, double exponent) : focal_length_(focal_length - 1.0), exponent_(exponent) {}

DistanceMaterial_t::~DistanceMaterial_t(){}

void DistanceMaterial_t::bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) {
    ray.colour_ = Vec3f(1.0/(std::pow(ray.dist_ - focal_length_, exponent_)));
    ray.mask_ = Vec3f();
}