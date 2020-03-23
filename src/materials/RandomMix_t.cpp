#include "RandomMix_t.h"
#include "entities/Shape_t.h"
#include "RandomGenerator_t.h"
#include "entities/Material_t.h"

APTracer::Materials::RandomMix_t::RandomMix_t(APTracer::Entities::Material_t* material_refracted, APTracer::Entities::Material_t* material_reflected, double ratio) : 
    MaterialMix_t(material_refracted, material_reflected), ratio_(ratio), unif_(std::uniform_real_distribution<double>(0, 1)) {}

APTracer::Materials::RandomMix_t::~RandomMix_t(){}

void APTracer::Materials::RandomMix_t::bounce(const double (&uv)[2], const APTracer::Entities::Shape_t* hit_obj, APTracer::Entities::Ray_t &ray) {
    if (unif_(APTracer::Entities::rng) < ratio_){ // refracted
        material_refracted_->bounce(uv, hit_obj, ray);
    }
    else{   // reflected
        material_reflected_->bounce(uv, hit_obj, ray);
    }
}