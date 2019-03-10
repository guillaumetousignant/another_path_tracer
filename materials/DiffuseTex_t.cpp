#include "DiffuseTex_t.h"
#include "Shape_t.h"
#include "Texture_t.h"
#include <math.h>
#include "RandomGenerator_t.h"
#include "Referentials.h"

#define EPSILON 0.00000001
#define PI 3.141592653589793238463

DiffuseTex_t::DiffuseTex_t(const Vec3f &emission, const Texture_t* texture, double roughness) : 
    emission_(emission), texture_(texture), roughness_(roughness), unif_(std::uniform_real_distribution<double>(0, 1)) {}

DiffuseTex_t::~DiffuseTex_t(){}

void DiffuseTex_t::bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) {
    Vec3f axis;
    Vec3f normal;
    Vec3f u, v;
    Vec3f newdir;
    double rand1, rand2, rand2s;
    double tuv[2];

    hit_obj->normaluv(ray, uv, tuv, normal);

    rand1 = unif_(my_rand::rng)*2*PI;
    rand2 = unif_(my_rand::rng);
    rand2s = sqrt(rand2);

    if (normal.dot(ray.direction_) > 0.0){
        normal *= -1.0;
    }

    axis = std::abs(normal[0]) > 0.1 ? Vec3f(0.0, 1.0, 0.0) : Vec3f(1.0, 0.0, 0.0);

    u = axis.cross(normal).normalize();
    v = normal.cross(u).normalize(); // wasn't normalized before

    newdir = u*cos(rand1)*rand2s + v*sin(rand1)*rand2s + normal*sqrt(1.0-rand2);
    newdir = newdir.normalize();

    ray.origin_ += ray.direction_ * ray.dist_ + normal * EPSILON;
    ray.direction_ = newdir;

    ray.colour_ += ray.mask_ * emission_;
    ray.mask_ *= texture_->get(tuv) * pow(newdir.dot(normal), roughness_);
}