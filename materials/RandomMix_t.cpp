#include "RandomMix_t.h"
#include "Shape_t.h"
#include "RandomGenerator_t.h"
#include "Material_t.h"

using APTracer::Materials::RandomMix_t;

RandomMix_t::RandomMix_t(Material_t* material_refracted, Material_t* material_reflected, double ratio) : 
    MaterialMix_t(material_refracted, material_reflected), ratio_(ratio), unif_(std::uniform_real_distribution<double>(0, 1)) {}

RandomMix_t::~RandomMix_t(){}

void RandomMix_t::bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) {
    if (unif_(APTracer::Entities::rng) < ratio_){ // refracted
        material_refracted_->bounce(uv, hit_obj, ray);
    }
    else{   // reflected
        material_reflected_->bounce(uv, hit_obj, ray);
    }
}