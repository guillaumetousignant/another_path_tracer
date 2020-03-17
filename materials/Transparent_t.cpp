#include "Transparent_t.h"
#include "ScatteringFunction_t.h"
#include "Shape_t.h"

#define EPSILON 0.00000001

using APTracer::Materials::Transparent_t;
using APTracer::Entities::ScatteringFunction_t;
using APTracer::Entities::Shape_t;
using APTracer::Entities::Medium_t;
using APTracer::Entities::Ray_t;
using APTracer::Entities::Vec3f;

Transparent_t::Transparent_t(unsigned int priority, ScatteringFunction_t* scattering) : Medium_t(1.0, priority, scattering) {

}

Transparent_t::~Transparent_t() {

}

void Transparent_t::bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray){
    Vec3f normal;

    hit_obj->normal(ray, uv, normal);
    const double cosi = ray.direction_.dot(normal);

    if (cosi < 0.0) {
        ray.origin_ += ray.direction_ * ray.dist_ - normal * EPSILON;
        ray.add_to_mediums(this);
    }
    else{
        ray.origin_ += ray.direction_ * ray.dist_ + normal * EPSILON;
        ray.remove_from_mediums(this);
    }
}