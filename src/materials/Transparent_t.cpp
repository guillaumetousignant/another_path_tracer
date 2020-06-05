#include "materials/Transparent_t.h"
#include "entities/Medium_t.h"
#include "entities/Shape_t.h"

#define EPSILON 0.00000001

using APTracer::Entities::Vec3f;

APTracer::Materials::Transparent_t::Transparent_t(APTracer::Entities::Medium_t* medium) : medium_(medium) {
}

APTracer::Materials::Transparent_t::~Transparent_t() {
}

void APTracer::Materials::Transparent_t::bounce(const double (&uv)[2], const APTracer::Entities::Shape_t* hit_obj, APTracer::Entities::Ray_t &ray){
    const Vec3f normal = hit_obj->normal(ray.time_, uv);

    const double cosi = ray.direction_.dot(normal);

    if (cosi < 0.0) {
        ray.origin_ += ray.direction_ * ray.dist_ - normal * EPSILON;
        ray.add_to_mediums(medium_);
    }
    else{
        ray.origin_ += ray.direction_ * ray.dist_ + normal * EPSILON;
        ray.remove_from_mediums(medium_);
    }
}