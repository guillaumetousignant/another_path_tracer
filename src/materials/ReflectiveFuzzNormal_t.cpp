#include "materials/ReflectiveFuzzNormal_t.h"
#include "entities/RandomGenerator_t.h"
#include "entities/Shape_t.h"
#include "entities/Texture_t.h"
#include <cmath>

constexpr double epsilon = 0.00000001;
constexpr double pi      = 3.141592653589793238463;

using APTracer::Entities::Vec3f;

APTracer::Materials::ReflectiveFuzzNormal_t::ReflectiveFuzzNormal_t(const Vec3f& emission, const Vec3f& colour, const APTracer::Entities::Texture_t* normal_map, double order, double diffusivity) :
        emission_(emission), colour_(colour), normal_map_(normal_map), order_(order), diffusivity_(diffusivity), unif_(0, 1) {}

auto APTracer::Materials::ReflectiveFuzzNormal_t::bounce(std::array<double, 2> uv, const APTracer::Entities::Shape_t* hit_obj, APTracer::Entities::Ray_t& ray) -> void {
    Vec3f tangent;
    std::array<double, 2> tuv;
    Vec3f normal = hit_obj->normal_uv_tangent(ray.time_, uv, tuv, tangent);

    const Vec3f bitangent       = normal.cross(tangent);
    const Vec3f tangent_weights = normal_map_->get(tuv) * 2.0 - 1.0;
    normal                      = (tangent * tangent_weights[0] + bitangent * tangent_weights[1] + normal * tangent_weights[2]).normalize_inplace();

    if (normal.dot(ray.direction_) > 0.0) {
        normal *= -1.0;
    }

    const double rand1  = unif_(APTracer::Entities::rng()) * 2.0 * pi;
    const double rand2  = std::pow(unif_(APTracer::Entities::rng()), order_) * diffusivity_;
    const double rand2s = sqrt(rand2);

    const Vec3f axis = std::abs(normal[0]) > 0.1 ? Vec3f(0.0, 1.0, 0.0) : Vec3f(1.0, 0.0, 0.0);

    const Vec3f u = axis.cross(normal).normalize_inplace();
    const Vec3f v = normal.cross(u).normalize_inplace(); // wasn't normalized before

    const Vec3f normal_fuzz = (u * std::cos(rand1) * rand2s + v * std::sin(rand1) * rand2s + normal * std::sqrt(1.0 - rand2)).normalize_inplace();

    ray.origin_ += ray.direction_ * ray.dist_ + normal * epsilon;
    ray.direction_ = ray.direction_ - normal_fuzz * 2.0 * ray.direction_.dot(normal_fuzz);

    ray.colour_ += ray.mask_ * emission_;
    ray.mask_ *= colour_;
}