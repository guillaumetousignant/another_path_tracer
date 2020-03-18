#include "ReflectiveFuzz_t.h"
#include "Shape_t.h"
#include "RandomGenerator_t.h"
#include <cmath>

#define EPSILON 0.00000001
#define PI 3.141592653589793238463

using APTracer::Entities::Vec3f;

APTracer::Materials::ReflectiveFuzz_t::ReflectiveFuzz_t(const Vec3f &emission, const Vec3f &colour, double order, double diffusivity) : 
    emission_(emission), colour_(colour), order_(order), diffusivity_(diffusivity), unif_(0, 1) {}

APTracer::Materials::ReflectiveFuzz_t::~ReflectiveFuzz_t(){}

void APTracer::Materials::ReflectiveFuzz_t::bounce(const double (&uv)[2], const APTracer::Entities::Shape_t* hit_obj, APTracer::Entities::Ray_t &ray) {
    Vec3f normal;

    hit_obj->normal(ray, uv, normal);

    if (normal.dot(ray.direction_) > 0.0){
        normal *= -1.0;
    }

    const double rand1 = unif_(APTracer::Entities::rng)*2*PI;
    const double rand2 = std::pow(unif_(APTracer::Entities::rng), order_) * diffusivity_;
    const double rand2s = sqrt(rand2);

    const Vec3f axis = std::abs(normal[0]) > 0.1 ? Vec3f(0.0, 1.0, 0.0) : Vec3f(1.0, 0.0, 0.0);

    const Vec3f u = axis.cross(normal).normalize_inplace();
    const Vec3f v = normal.cross(u).normalize_inplace(); // wasn't normalized before

    const Vec3f normal_fuzz = (u * std::cos(rand1)*rand2s + v*std::sin(rand1)*rand2s + normal*std::sqrt(1.0-rand2)).normalize_inplace();
    
    ray.origin_ += ray.direction_ * ray.dist_ + normal * EPSILON;
    ray.direction_ = ray.direction_ - normal_fuzz * 2.0 * ray.direction_.dot(normal_fuzz);

    ray.colour_ += ray.mask_ * emission_;
    ray.mask_ *= colour_;
}