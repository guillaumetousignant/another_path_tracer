#include "materials/TextureMix_t.hpp"
#include "entities/RandomGenerator_t.hpp"
#include "entities/Shape_t.hpp"

using APTracer::Entities::Texture_t;

APTracer::Materials::TextureMix_t::TextureMix_t(Material_t* first_material, Material_t* second_material, const Texture_t* texture) :
        first_material_(first_material), second_material_(second_material), texture_(texture), unif_(std::uniform_real_distribution<double>(0, 1)) {}

auto APTracer::Materials::TextureMix_t::bounce(std::array<double, 2> uv, const APTracer::Entities::Shape_t* hit_obj, APTracer::Entities::Ray_t& ray) -> void {
    std::array<double, 2> tuv{};
    hit_obj->normaluv(ray.time_, uv, tuv);
    const double ratio = texture_->get(tuv).x();

    if (unif_(APTracer::Entities::rng()) < ratio) { // refracted
        first_material_->bounce(uv, hit_obj, ray);
    }
    else { // reflected
        second_material_->bounce(uv, hit_obj, ray);
    }
}
