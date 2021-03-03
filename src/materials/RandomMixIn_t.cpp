#include "materials/RandomMixIn_t.h"
#include "entities/Shape_t.h"
#include "entities/RandomGenerator_t.h"
#include "entities/Vec3f.h"

using APTracer::Entities::Vec3f;

APTracer::Materials::RandomMixIn_t::RandomMixIn_t(APTracer::Entities::Material_t* first_material, APTracer::Entities::Material_t* second_material, double ratio) : 
    first_material_(first_material), second_material_(second_material), ratio_(ratio), unif_(std::uniform_real_distribution<double>(0, 1)) {}

APTracer::Materials::RandomMixIn_t::~RandomMixIn_t() = default;

void APTracer::Materials::RandomMixIn_t::bounce(std::array<double, 2> uv, const APTracer::Entities::Shape_t* hit_obj, APTracer::Entities::Ray_t &ray) {
    if ((ray.direction_.dot(hit_obj->normal(ray.time_, uv)) > 0.0) || (unif_(APTracer::Entities::rng()) < ratio_)) { // refracted
        first_material_->bounce(uv, hit_obj, ray);
    }
    else {   // reflected
        second_material_->bounce(uv, hit_obj, ray);
    }
}