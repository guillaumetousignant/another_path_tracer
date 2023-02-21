#include "materials/FresnelMixNormal_t.hpp"
#include "entities/Medium_t.hpp"
#include "entities/RandomGenerator_t.hpp"
#include "entities/Shape_t.hpp"
#include "entities/Texture_t.hpp"
#include <cmath>

using APTracer::Entities::Vec3f;

APTracer::Materials::FresnelMixNormal_t::FresnelMixNormal_t(APTracer::Entities::Material_t* material_refracted,
                                                            APTracer::Entities::Material_t* material_reflected,
                                                            double ind,
                                                            const APTracer::Entities::Texture_t* normal_map) :
        material_refracted_(material_refracted), material_reflected_(material_reflected), ind_(ind), normal_map_(normal_map), unif_(std::uniform_real_distribution<double>(0, 1)) {}

auto APTracer::Materials::FresnelMixNormal_t::bounce(std::array<double, 2> uv, const APTracer::Entities::Shape_t* hit_obj, APTracer::Entities::Ray_t& ray) -> void {
    Vec3f tangent;
    std::array<double, 2> tuv;
    double kr;
    Vec3f normal = hit_obj->normal_uv_tangent(ray.time_, uv, tuv, tangent);

    const Vec3f bitangent       = normal.cross(tangent);
    const Vec3f tangent_weights = normal_map_->get(tuv) * 2.0 - 1.0;
    normal                      = (tangent * tangent_weights[0] + bitangent * tangent_weights[1] + normal * tangent_weights[2]).normalize_inplace();

    const double cosi = std::abs(ray.direction_.dot(normal));

    const double etai = ray.medium_list_.front()->ind_;
    const double etat = ind_;

    const double sint = etai / etat * std::sqrt(1 - cosi * cosi);

    if (sint >= 1) {
        kr = 1.0;
    }
    else {
        const double cost = std::sqrt(1 - sint * sint);
        const double Rs   = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
        const double Rp   = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
        kr                = (Rs * Rs + Rp * Rp) / 2.0;
    }

    if (unif_(APTracer::Entities::rng()) > kr) { // refracted
        material_refracted_->bounce(uv, hit_obj, ray);
    }
    else { // reflected
        material_reflected_->bounce(uv, hit_obj, ray);
    }
}