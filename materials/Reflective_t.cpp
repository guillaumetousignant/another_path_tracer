#include "Reflective_t.h"
#include "Shape_t.h"

#define EPSILON 0.00000001
#define PI 3.141592653589793238463

Reflective_t::Reflective_t(const Vec3f &emission, const Vec3f &colour) : 
    emission_(emission), colour_(colour) {}

Reflective_t::~Reflective_t(){}

void Reflective_t::bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) {
    Vec3f normal;
    Vec3f newdir;

    hit_obj->normal(ray, uv, normal);

    if (normal.dot(ray.direction_) > 0.0){
        normal *= -1.0;
    }

    newdir = ray.direction_ - normal * 2.0 * ray.direction_.dot(normal);
    
    ray.origin_ += ray.direction_ * ray.dist_ + normal * EPSILON;
    ray.direction_ = newdir;

    ray.colour_ += ray.mask_ * emission_;
    ray.mask_ *= colour_;
}