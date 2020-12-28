#include "materials/Toon_t.h"
#include "entities/Shape_t.h"

using APTracer::Entities::Shape_t;

APTracer::Materials::Toon_t::Toon_t(const Vec3f &colour) : colour_(colour) {}

APTracer::Materials::Toon_t::~Toon_t() = default;

void APTracer::Materials::Toon_t::bounce(const std::array<double, 2> &uv, const APTracer::Entities::Shape_t* hit_obj, APTracer::Entities::Ray_t &ray) {
    ray.colour_ = colour_;
    ray.mask_ = Vec3f();
}