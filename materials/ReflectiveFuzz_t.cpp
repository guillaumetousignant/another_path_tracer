#include "ReflectiveFuzz_t.h"
#include "Shape_t.h"
#include "RandomGenerator_t.h"
#include <cmath>

#define EPSILON 0.00000001
#define PI 3.141592653589793238463

ReflectiveFuzz_t::ReflectiveFuzz_t(const Vec3f &emission, const Vec3f &colour, double order, double diffusivity) : 
    emission_(emission), colour_(colour), order_(order), diffusivity_(diffusivity), unif_(0, 1) {}

ReflectiveFuzz_t::~ReflectiveFuzz_t(){}

void ReflectiveFuzz_t::bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) {
    Vec3f normal, normal_fuzz;
    Vec3f axis;
    Vec3f u, v;
    double rand1, rand2, rand2s;

    hit_obj->normal(ray, uv, normal);

    if (normal.dot(ray.direction_) > 0.0){
        normal *= -1.0;
    }

    rand1 = unif_(my_rand::rng)*2*PI;
    rand2 = std::pow(unif_(my_rand::rng), order_) * diffusivity_;
    rand2s = sqrt(rand2);

    axis = std::abs(normal[0]) > 0.1 ? Vec3f(0.0, 1.0, 0.0) : Vec3f(1.0, 0.0, 0.0);

    u = axis.cross(normal).normalize();
    v = normal.cross(u).normalize(); // wasn't normalized before

    normal_fuzz = (u * std::cos(rand1)*rand2s + v*std::sin(rand1)*rand2s + normal*std::sqrt(1.0-rand2)).normalize();
    
    ray.origin_ += ray.direction_ * ray.dist_ + normal * EPSILON;
    ray.direction_ = ray.direction_ - normal_fuzz * 2.0 * ray.direction_.dot(normal_fuzz);

    ray.colour_ += ray.mask_ * emission_;
    ray.mask_ *= colour_;
}