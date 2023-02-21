#include "materials/RandomMix_t.hpp"
#include "entities/RandomGenerator_t.hpp"
#include "entities/Shape_t.hpp"

APTracer::Materials::RandomMix_t::RandomMix_t(APTracer::Entities::Material_t* first_material, APTracer::Entities::Material_t* second_material, double ratio) :
        first_material_(first_material), second_material_(second_material), ratio_(ratio), unif_(std::uniform_real_distribution<double>(0, 1)) {}

auto APTracer::Materials::RandomMix_t::bounce(std::array<double, 2> uv, const APTracer::Entities::Shape_t* hit_obj, APTracer::Entities::Ray_t& ray) -> void {
    if (unif_(APTracer::Entities::rng()) < ratio_) { // refracted
        first_material_->bounce(uv, hit_obj, ray);
    }
    else { // reflected
        second_material_->bounce(uv, hit_obj, ray);
    }
}