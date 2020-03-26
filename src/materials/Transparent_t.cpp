#include "materials/Transparent_t.h"
#include "entities/ScatteringFunction_t.h"
#include "entities/Shape_t.h"

#define EPSILON 0.00000001

using APTracer::Entities::Vec3f;

APTracer::Materials::Transparent_t::Transparent_t(unsigned int priority, APTracer::Entities::ScatteringFunction_t* scattering) : Medium_t(1.0, priority, scattering) {

}

APTracer::Materials::Transparent_t::~Transparent_t() {

}

void APTracer::Materials::Transparent_t::bounce(const double (&uv)[2], const APTracer::Entities::Shape_t* hit_obj, APTracer::Entities::Ray_t &ray){
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