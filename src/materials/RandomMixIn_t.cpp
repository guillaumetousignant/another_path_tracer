#include "materials/RandomMixIn_t.h"
#include "entities/Shape_t.h"
#include "entities/RandomGenerator_t.h"
#include "entities/Vec3f.h"
#include "entities/Material_t.h"

using APTracer::Entities::Vec3f;

APTracer::Materials::RandomMixIn_t::RandomMixIn_t(APTracer::Entities::Material_t* material_refracted, APTracer::Entities::Material_t* material_reflected, double ratio) : 
    MaterialMix_t(material_refracted, material_reflected), ratio_(ratio), unif_(std::uniform_real_distribution<double>(0, 1)) {}

APTracer::Materials::RandomMixIn_t::~RandomMixIn_t() {}

void APTracer::Materials::RandomMixIn_t::bounce(const double (&uv)[2], const APTracer::Entities::Shape_t* hit_obj, APTracer::Entities::Ray_t &ray) {
    if ((ray.direction_.dot(hit_obj->normal(ray.time_, uv)) > 0.0) || (unif_(APTracer::Entities::rng) < ratio_)) { // refracted
        material_refracted_->bounce(uv, hit_obj, ray);
    }
    else{   // reflected
        material_reflected_->bounce(uv, hit_obj, ray);
    }
}