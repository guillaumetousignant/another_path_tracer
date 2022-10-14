#include "materials/TextureMixIn_t.h"
#include "entities/RandomGenerator_t.h"
#include "entities/Shape_t.h"
#include "entities/Texture_t.h"
#include "entities/Vec3f.h"

using APTracer::Entities::Vec3f;

APTracer::Materials::TextureMixIn_t::TextureMixIn_t(Material_t* first_material, Material_t* second_material, const Texture_t* texture) :
        first_material_(first_material), second_material_(second_material), texture_(texture), unif_(std::uniform_real_distribution<double>(0, 1)) {}

auto APTracer::Materials::TextureMixIn_t::bounce(std::array<double, 2> uv, const APTracer::Entities::Shape_t* hit_obj, APTracer::Entities::Ray_t& ray) -> void {
    std::array<double, 2> tuv;
    const Vec3f normal = hit_obj->normaluv(ray.time_, uv, tuv);
    const double ratio = texture_->get(tuv).x();

    if ((ray.direction_.dot(normal) > 0.0) || (unif_(APTracer::Entities::rng()) < ratio)) { // refracted
        first_material_->bounce(uv, hit_obj, ray);
    }
    else { // reflected
        second_material_->bounce(uv, hit_obj, ray);
    }
}