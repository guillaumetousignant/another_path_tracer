#include "FresnelMix_t.h"
#include "Shape_t.h"
#include "Medium_t.h"
#include <math.h> 
#include "RandomGenerator_t.h"
#include "Material_t.h"

FresnelMix_t::FresnelMix_t(Material_t* material_refracted, Material_t* material_reflected, double ind) : 
    MaterialMix_t(material_refracted, material_reflected), ind_(ind), unif_(std::uniform_real_distribution<double>(0, 1)) {}

FresnelMix_t::~FresnelMix_t(){}

void FresnelMix_t::bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) {
    Vec3f normal;
    double cosi;
    double etai, etat;
    double sint, cost;
    double kr;
    double Rs, Rp;

    hit_obj->normal(ray, uv, normal);

    cosi = std::abs(ray.direction_.dot(normal));

    etai = ray.medium_list_.front()->ind_;
    etat = ind_;

    sint = etai/etat * std::sqrt(1 - cosi * cosi);

    if (sint >= 1){
        kr = 1.0;
    }
    else{
        cost = std::sqrt(1 - sint * sint);
        Rs = ((etat * cosi) - (etai * cost))/((etat * cosi) + (etai * cost));
        Rp = ((etai * cosi) - (etat * cost))/((etai * cosi) + (etat * cost));
        kr = (Rs * Rs + Rp * Rp)/2.0;
    }

    if (unif_(my_rand::rng) > kr){ // refracted
        material_refracted_->bounce(uv, hit_obj, ray);
    }
    else{ // reflected
        material_reflected_->bounce(uv, hit_obj, ray);
    }
}