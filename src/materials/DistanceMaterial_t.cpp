#include "materials/DistanceMaterial_t.h"
#include "entities/Shape_t.h"
#include "entities/Vec3f.h"
#include <cmath>

using APTracer::Entities::Vec3f;

APTracer::Materials::DistanceMaterial_t::DistanceMaterial_t(double focal_length, double exponent) : focal_length_(focal_length - 1.0), exponent_(exponent) {}

APTracer::Materials::DistanceMaterial_t::~DistanceMaterial_t(){}

void APTracer::Materials::DistanceMaterial_t::bounce(const double (&uv)[2], const APTracer::Entities::Shape_t* hit_obj, APTracer::Entities::Ray_t &ray) {
    ray.colour_ = Vec3f(1.0/(std::pow(ray.dist_ - focal_length_, exponent_)));
    ray.mask_ = Vec3f();
}