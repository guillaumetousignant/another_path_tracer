#include "FresnelMixIn_t.h"
#include "Shape_t.h"
#include "Medium_t.h"
#include <math.h> 
#include "RandomGenerator_t.h"

FresnelMixIn_t::FresnelMixIn_t(Material_t* material_refracted, Material_t* material_reflected, double ind) : 
    material_refracted_(material_refracted), material_reflected_(material_reflected), ind_(ind), unif_(std::uniform_real_distribution<double>(0, 1)) {}

FresnelMixIn_t::~FresnelMixIn_t(){}

void FresnelMixIn_t::bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) {
    Vec3f normal;
    double cosi;
    double etai, etat;
    double sint, cost;
    double kr;
    double Rs, Rp;
    double rand1;

    hit_obj->normal(ray, uv, normal);

    cosi = ray.direction_.dot(normal);
    if (cosi > 0){ // going out
        material_refracted_->bounce(uv, hit_obj, ray);
        return;
    }
    else{
        cosi *= -1.0;
    }

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