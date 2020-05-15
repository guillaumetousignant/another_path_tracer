#include "materials/NormalMaterial_t.h"
#include "entities/Shape_t.h"
#include "entities/Vec3f.h"

using APTracer::Entities::Vec3f;

APTracer::Materials::NormalMaterial_t::NormalMaterial_t() {}

APTracer::Materials::NormalMaterial_t::~NormalMaterial_t(){}

void APTracer::Materials::NormalMaterial_t::bounce(const double (&uv)[2], const APTracer::Entities::Shape_t* hit_obj, APTracer::Entities::Ray_t &ray) {
    Vec3f normal;

    hit_obj->normal(ray.time_, uv, normal);
    ray.colour_ = normal*0.5 + 0.5;
    ray.mask_ = Vec3f();
}