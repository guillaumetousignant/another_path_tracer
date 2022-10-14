#include "materials/NormalDiffuseMaterial_t.h"
#include "entities/RandomGenerator_t.h"
#include "entities/Shape_t.h"
#include "entities/Vec3f.h"
#include <cmath>

constexpr double epsilon = 0.00001; // was 0.00001, 0.01 works ok when *t
constexpr double pi      = 3.141592653589793238463;

using APTracer::Entities::Vec3f;

APTracer::Materials::NormalDiffuseMaterial_t::NormalDiffuseMaterial_t(double roughness) : roughness_(roughness), unif_(std::uniform_real_distribution<double>(0, 1)) {}

auto APTracer::Materials::NormalDiffuseMaterial_t::bounce(std::array<double, 2> uv, const APTracer::Entities::Shape_t* hit_obj, APTracer::Entities::Ray_t& ray) -> void {
    Vec3f normal = hit_obj->normal(ray.time_, uv);

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

    ray.origin_ += ray.direction_ * ray.dist_ + normal * epsilon; //*ray.dist_; // Made EPSILON relative, check // well guess what wasn,t a goood idea
    ray.direction_ = newdir;

    ray.mask_ *= (normal * 0.5 + 0.5) * pow(newdir.dot(normal), roughness_);
}