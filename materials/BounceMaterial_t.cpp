#include "BounceMaterial_t.h"
#include "Shape_t.h"
#include "Vec3f.h"

BounceMaterial_t::BounceMaterial_t(unsigned int max_bounces) {
    emission_ = Vec3f(1.0/max_bounces);
}

BounceMaterial_t::~BounceMaterial_t(){}

void BounceMaterial_t::bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) {
    ray.colour_ += emission_;
    ray.mask_ = Vec3f(1.0);
}