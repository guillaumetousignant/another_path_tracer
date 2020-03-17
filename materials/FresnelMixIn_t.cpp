#include "FresnelMixIn_t.h"
#include "Shape_t.h"
#include "Medium_t.h"
#include <cmath> 
#include "RandomGenerator_t.h"
#include "Material_t.h"

FresnelMixIn_t::FresnelMixIn_t(Material_t* material_refracted, Material_t* material_reflected, double ind) : 
    MaterialMix_t(material_refracted, material_reflected), ind_(ind), unif_(std::uniform_real_distribution<double>(0, 1)) {}

FresnelMixIn_t::~FresnelMixIn_t(){}

void FresnelMixIn_t::bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) {
    Vec3f normal;
    double kr;

    hit_obj->normal(ray, uv, normal);

    double cosi = ray.direction_.dot(normal);
    if (cosi > 0){ // going out
        material_refracted_->bounce(uv, hit_obj, ray);
        return;
    }
    else{
        cosi *= -1.0;
    }

    const double etai = ray.medium_list_.front()->ind_;
    const double etat = ind_;

    const double sint = etai/etat * std::sqrt(1 - cosi * cosi);

    if (sint >= 1){
        kr = 1.0;
    }
    else{
        const double cost = std::sqrt(1 - sint * sint);
        const double Rs = ((etat * cosi) - (etai * cost))/((etat * cosi) + (etai * cost));
        const double Rp = ((etai * cosi) - (etat * cost))/((etai * cosi) + (etat * cost));
        kr = (Rs * Rs + Rp * Rp)/2.0;
    }

    if (unif_(APTracer::Entities::rng) > kr){ // refracted
        material_refracted_->bounce(uv, hit_obj, ray);
    }
    else{ // reflected
        material_reflected_->bounce(uv, hit_obj, ray);
    }
}