#include "Transparent_t.h"
#include "ScatteringFunction_t.h"
#include "Shape_t.h"

#define EPSILON 0.00000001

Transparent_t::Transparent_t(unsigned int priority, ScatteringFunction_t* scattering) : Medium_t(priority, scattering) {

}

Transparent_t::~Transparent_t() {

}

void Transparent_t::bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray){
    Vec3f axis;
    Vec3f normal;
    Vec3f u, v;
    Vec3f newdir;
    double cosi;

    hit_obj->normal(ray, uv, normal);
    cosi = ray.direction_.dot(normal);

    if (cosi < 0.0) {
        ray.origin_ += ray.direction_ * ray.dist_ - normal * EPSILON;
        ray.add_to_mediums(this);
    }
    else{
        ray.origin_ += ray.direction_ * ray.dist_ + normal * EPSILON;
        ray.remove_from_mediums(this);
    }
}