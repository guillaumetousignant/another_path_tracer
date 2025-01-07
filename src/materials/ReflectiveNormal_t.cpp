#include "materials/ReflectiveNormal_t.hpp"
#include "entities/Shape_t.hpp"

constexpr double epsilon = 0.00000001;

using APTracer::Entities::Vec3f;

APTracer::Materials::ReflectiveNormal_t::ReflectiveNormal_t(const Entities::Vec3f& emission, const Entities::Vec3f& colour, const APTracer::Entities::Texture_t* normal_map) :
        emission_(emission), colour_(colour), normal_map_(normal_map) {}

auto APTracer::Materials::ReflectiveNormal_t::bounce(std::array<double, 2> uv, const APTracer::Entities::Shape_t* hit_obj, APTracer::Entities::Ray_t& ray) -> void {
    Vec3f tangent;
    std::array<double, 2> tuv{};
    Vec3f normal = hit_obj->normal_uv_tangent(ray.time_, uv, tuv, tangent);

    const Vec3f bitangent       = normal.cross(tangent);
    const Vec3f tangent_weights = normal_map_->get(tuv) * 2.0 - 1.0;
    normal                      = (tangent * tangent_weights[0] + bitangent * tangent_weights[1] + normal * tangent_weights[2]).normalize_inplace();

    if (normal.dot(ray.direction_) > 0.0) {
        normal *= -1.0;
    }

    ray.origin_ += ray.direction_ * ray.dist_ + normal * epsilon;
    ray.direction_ = ray.direction_ - normal * 2.0 * ray.direction_.dot(normal);

    ray.colour_ += ray.mask_ * emission_;
    ray.mask_ *= colour_;
}
