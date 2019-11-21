#include "FresnelMixNormal_t.h"
#include "Shape_t.h"
#include "Medium_t.h"
#include <cmath> 
#include "RandomGenerator_t.h"
#include "Material_t.h"
#include "Texture_t.h"

FresnelMixNormal_t::FresnelMixNormal_t(Material_t* material_refracted, Material_t* material_reflected, double ind, const Texture_t* normal_map) : 
    MaterialMix_t(material_refracted, material_reflected), ind_(ind), normal_map_(normal_map), unif_(std::uniform_real_distribution<double>(0, 1)) {}

FresnelMixNormal_t::~FresnelMixNormal_t(){}

void FresnelMixNormal_t::bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) {
    Vec3f normal;
    Vec3f tangent;
    Vec3f bitangent;
    Vec3f tangent_weights;
    double tuv[2];
    double kr;

    hit_obj->normal_uv_tangent(ray, uv, tuv, normal, tangent);
    bitangent = normal.cross(tangent);
    tangent_weights = normal_map_->get(tuv) * 2.0 - 1.0;
    normal = (tangent * tangent_weights[0] + bitangent * tangent_weights[1] + normal * tangent_weights[2]).normalize();

    const double cosi = std::abs(ray.direction_.dot(normal));

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

    if (unif_(my_rand::rng) > kr){ // refracted
        material_refracted_->bounce(uv, hit_obj, ray);
    }
    else{ // reflected
        material_reflected_->bounce(uv, hit_obj, ray);
    }
}