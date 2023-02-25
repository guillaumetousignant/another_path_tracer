#include "materials/Reflective_t.hpp"
#include "entities/Shape_t.hpp"

constexpr double epsilon = 0.00000001;

using APTracer::Entities::Vec3f;

APTracer::Materials::Reflective_t::Reflective_t(const Entities::Vec3f& emission, const Entities::Vec3f& colour) : emission_(emission), colour_(colour) {}

auto APTracer::Materials::Reflective_t::bounce(std::array<double, 2> uv, const APTracer::Entities::Shape_t* hit_obj, APTracer::Entities::Ray_t& ray) -> void {
    Vec3f normal = hit_obj->normal(ray.time_, uv);

    if (normal.dot(ray.direction_) > 0.0) {
        normal *= -1.0;
    }

    ray.origin_ += ray.direction_ * ray.dist_ + normal * epsilon;
    ray.direction_ = ray.direction_ - normal * 2.0 * ray.direction_.dot(normal);

    ray.colour_ += ray.mask_ * emission_;
    ray.mask_ *= colour_;
}