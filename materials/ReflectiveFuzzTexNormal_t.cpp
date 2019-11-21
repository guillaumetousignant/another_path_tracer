#include "ReflectiveFuzzTexNormal_t.h"
#include "Shape_t.h"
#include "RandomGenerator_t.h"
#include <cmath>
#include "Texture_t.h"

#define EPSILON 0.00000001
#define PI 3.141592653589793238463

ReflectiveFuzzTexNormal_t::ReflectiveFuzzTexNormal_t(const Vec3f &emission, Texture_t* texture, const Texture_t* normal_map, double order, double diffusivity)
    : texture_(texture), emission_(emission), normal_map_(normal_map), order_(order), diffusivity_(diffusivity), unif_(0, 1) {}

ReflectiveFuzzTexNormal_t::~ReflectiveFuzzTexNormal_t(){}

void ReflectiveFuzzTexNormal_t::bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) {
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

    const double rand1 = unif_(my_rand::rng)*2*PI;
    const double rand2 = std::pow(unif_(my_rand::rng), order_) * diffusivity_;
    const double rand2s = sqrt(rand2);

    const Vec3f axis = std::abs(normal[0]) > 0.1 ? Vec3f(0.0, 1.0, 0.0) : Vec3f(1.0, 0.0, 0.0);

    const Vec3f u = axis.cross(normal).normalize();
    const Vec3f v = normal.cross(u).normalize(); // wasn't normalized before

    const Vec3f normal_fuzz = (u * std::cos(rand1)*rand2s + v*std::sin(rand1)*rand2s + normal*std::sqrt(1.0-rand2)).normalize();
    
    ray.origin_ += ray.direction_ * ray.dist_ + normal * EPSILON;
    ray.direction_ = ray.direction_ - normal_fuzz * 2.0 * ray.direction_.dot(normal_fuzz);

    ray.colour_ += ray.mask_ * emission_;
    ray.mask_ *= texture_->get(tuv);
}