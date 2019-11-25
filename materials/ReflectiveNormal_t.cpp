#include "ReflectiveNormal_t.h"
#include "Shape_t.h"
#include "Texture_t.h"

#define EPSILON 0.00000001

ReflectiveNormal_t::ReflectiveNormal_t(const Vec3f &emission, const Vec3f &colour, const Texture_t* normal_map) : 
    emission_(emission), colour_(colour), normal_map_(normal_map) {}

ReflectiveNormal_t::~ReflectiveNormal_t(){}

void ReflectiveNormal_t::bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) {
    Vec3f normal;
    Vec3f tangent;
    Vec3f bitangent;
    Vec3f tangent_weights;
    double tuv[2];

    hit_obj->normal_uv_tangent(ray, uv, tuv, normal, tangent);
    bitangent = normal.cross(tangent);
    tangent_weights = normal_map_->get(tuv) * 2.0 - 1.0;
    normal = (tangent * tangent_weights[0] + bitangent * tangent_weights[1] + normal * tangent_weights[2]).normalize();


    if (normal.dot(ray.direction_) > 0.0){
        normal *= -1.0;
    }
    
    ray.origin_ += ray.direction_ * ray.dist_ + normal * EPSILON;
    ray.direction_ = ray.direction_ - normal * 2.0 * ray.direction_.dot(normal);

    ray.colour_ += ray.mask_ * emission_;
    ray.mask_ *= colour_;
}