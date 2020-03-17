#include "RandomMixIn_t.h"
#include "Shape_t.h"
#include "RandomGenerator_t.h"
#include "Vec3f.h"
#include "Material_t.h"

RandomMixIn_t::RandomMixIn_t(Material_t* material_refracted, Material_t* material_reflected, double ratio) : 
    MaterialMix_t(material_refracted, material_reflected), ratio_(ratio), unif_(std::uniform_real_distribution<double>(0, 1)) {}

RandomMixIn_t::~RandomMixIn_t(){}

void RandomMixIn_t::bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) {
    Vec3f normal;
    //double cosi;

    hit_obj->normal(ray, uv, normal);
    //cosi = ray.direction_.dot(normal);

    if ((ray.direction_.dot(normal) > 0.0) || (unif_(APTracer::Entities::rng) < ratio_)){ // refracted
        material_refracted_->bounce(uv, hit_obj, ray);
    }
    else{   // reflected
        material_reflected_->bounce(uv, hit_obj, ray);
    }
}