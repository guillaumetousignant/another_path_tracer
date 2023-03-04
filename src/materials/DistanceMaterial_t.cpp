#include "materials/DistanceMaterial_t.hpp"
#include "entities/Shape_t.hpp"
#include "entities/Vec3f.hpp"
#include <cmath>

using APTracer::Entities::Vec3f;

APTracer::Materials::DistanceMaterial_t::DistanceMaterial_t(double focus_distance, double exponent) : focus_distance_(focus_distance - 1.0), exponent_(exponent) {}

auto APTracer::Materials::DistanceMaterial_t::bounce(std::array<double, 2> uv, const APTracer::Entities::Shape_t* hit_obj, APTracer::Entities::Ray_t& ray) -> void {
    ray.colour_ = Vec3f(1.0 / (std::pow(ray.dist_ - focus_distance_, exponent_)));
    ray.mask_   = Vec3f();
}
