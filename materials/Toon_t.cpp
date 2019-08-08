#include "Toon_t.h"
#include "Shape_t.h"

Toon_t::Toon_t(const Vec3f &colour) : colour_(colour) {}

Toon_t::~Toon_t(){}

void Toon_t::bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) {
    ray.colour_ = colour_;
    ray.mask_ = Vec3f();
}