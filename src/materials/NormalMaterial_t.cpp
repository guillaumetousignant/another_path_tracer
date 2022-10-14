#include "materials/NormalMaterial_t.h"
#include "entities/Shape_t.h"
#include "entities/Vec3f.h"

auto APTracer::Materials::NormalMaterial_t::bounce(std::array<double, 2> uv, const APTracer::Entities::Shape_t* hit_obj, APTracer::Entities::Ray_t& ray) -> void {
    ray.colour_ = hit_obj->normal(ray.time_, uv) * 0.5 + 0.5;
    ray.mask_   = APTracer::Entities::Vec3f();
}