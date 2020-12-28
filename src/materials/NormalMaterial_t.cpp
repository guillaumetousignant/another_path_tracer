#include "materials/NormalMaterial_t.h"
#include "entities/Shape_t.h"
#include "entities/Vec3f.h"

using APTracer::Entities::Vec3f;

APTracer::Materials::NormalMaterial_t::NormalMaterial_t() {}

APTracer::Materials::NormalMaterial_t::~NormalMaterial_t() = default;

void APTracer::Materials::NormalMaterial_t::bounce(const std::array<double, 2> &uv, const APTracer::Entities::Shape_t* hit_obj, APTracer::Entities::Ray_t &ray) {
    ray.colour_ = hit_obj->normal(ray.time_, uv) * 0.5 + 0.5;
    ray.mask_ = Vec3f();
}