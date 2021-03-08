#include "materials/FresnelMix_t.h"
#include "entities/Shape_t.h"
#include "entities/Medium_t.h"
#include <cmath> 
#include "entities/RandomGenerator_t.h"

using APTracer::Entities::Vec3f;

APTracer::Materials::FresnelMix_t::FresnelMix_t(APTracer::Entities::Material_t* material_refracted, APTracer::Entities::Material_t* material_reflected, double ind) : 
    material_refracted_(material_refracted), material_reflected_(material_reflected), ind_(ind), unif_(std::uniform_real_distribution<double>(0, 1)) {}

auto APTracer::Materials::FresnelMix_t::bounce(std::array<double, 2> uv, const APTracer::Entities::Shape_t* hit_obj, APTracer::Entities::Ray_t &ray) -> void {
    const Vec3f normal = hit_obj->normal(ray.time_, uv);
    double kr;

    const double cosi = std::abs(ray.direction_.dot(normal));

    const double etai = ray.medium_list_.front()->ind_;
    const double etat = ind_;

    const double sint = etai/etat * std::sqrt(1 - cosi * cosi);

    if (sint >= 1) {
        kr = 1.0;
    }
    else {
        const double cost = std::sqrt(1 - sint * sint);
        const double Rs = ((etat * cosi) - (etai * cost))/((etat * cosi) + (etai * cost));
        const double Rp = ((etai * cosi) - (etat * cost))/((etai * cosi) + (etat * cost));
        kr = (Rs * Rs + Rp * Rp)/2.0;
    }

    if (unif_(APTracer::Entities::rng()) > kr) { // refracted
        material_refracted_->bounce(uv, hit_obj, ray);
    }
    else { // reflected
        material_reflected_->bounce(uv, hit_obj, ray);
    }
}