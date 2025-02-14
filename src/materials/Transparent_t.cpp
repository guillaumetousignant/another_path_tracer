#include "materials/Transparent_t.hpp"
#include "entities/Shape_t.hpp"

constexpr double epsilon = 0.00000001;

using APTracer::Entities::Vec3f;

APTracer::Materials::Transparent_t::Transparent_t(APTracer::Entities::Medium_t* medium) : medium_(medium) {}

auto APTracer::Materials::Transparent_t::bounce(std::array<double, 2> uv, const APTracer::Entities::Shape_t* hit_obj, APTracer::Entities::Ray_t& ray) -> void {
    const Vec3f normal = hit_obj->normal(ray.time_, uv);

    const double cosi = ray.direction_.dot(normal);

    if (cosi < 0.0) {
        ray.origin_ += ray.direction_ * ray.dist_ - normal * epsilon;
        ray.add_to_mediums(medium_);
    }
    else {
        ray.origin_ += ray.direction_ * ray.dist_ + normal * epsilon;
        ray.remove_from_mediums(medium_);
    }
}
