#include "materials/DiffuseTex_t.hpp"
#include "entities/RandomGenerator_t.hpp"
#include "entities/Shape_t.hpp"
#include <cmath>

constexpr double epsilon = 0.00000001;
constexpr double pi      = 3.141592653589793238463;

using APTracer::Entities::Vec3f;

APTracer::Materials::DiffuseTex_t::DiffuseTex_t(const Entities::Vec3f& emission, const APTracer::Entities::Texture_t* texture, double roughness) :
        emission_(emission), texture_(texture), roughness_(roughness), unif_(std::uniform_real_distribution<double>(0, 1)) {}

auto APTracer::Materials::DiffuseTex_t::bounce(std::array<double, 2> uv, const APTracer::Entities::Shape_t* hit_obj, APTracer::Entities::Ray_t& ray) -> void {
    std::array<double, 2> tuv{};
    Vec3f normal = hit_obj->normaluv(ray.time_, uv, tuv);

    const double rand1  = unif_(APTracer::Entities::rng()) * 2.0 * pi;
    const double rand2  = unif_(APTracer::Entities::rng());
    const double rand2s = sqrt(rand2);

    if (normal.dot(ray.direction_) > 0.0) {
        normal *= -1.0;
    }

    const Vec3f axis = std::abs(normal[0]) > 0.1 ? Vec3f(0.0, 1.0, 0.0) : Vec3f(1.0, 0.0, 0.0);

    const Vec3f u = axis.cross(normal).normalize_inplace();
    const Vec3f v = normal.cross(u).normalize_inplace(); // wasn't normalized before

    const Vec3f newdir = (u * cos(rand1) * rand2s + v * sin(rand1) * rand2s + normal * sqrt(1.0 - rand2)).normalize_inplace();

    ray.origin_ += ray.direction_ * ray.dist_ + normal * epsilon;
    ray.direction_ = newdir;

    ray.colour_ += ray.mask_ * emission_;
    ray.mask_ *= texture_->get(tuv) * pow(newdir.dot(normal), roughness_);
}