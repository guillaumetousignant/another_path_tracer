#include "materials/Toon_t.hpp"
#include "entities/Shape_t.hpp"

using APTracer::Entities::Shape_t;

APTracer::Materials::Toon_t::Toon_t(const Vec3f& colour) : colour_(colour) {}

auto APTracer::Materials::Toon_t::bounce(std::array<double, 2> uv, const APTracer::Entities::Shape_t* hit_obj, APTracer::Entities::Ray_t& ray) -> void {
    ray.colour_ = colour_;
    ray.mask_   = Vec3f();
}