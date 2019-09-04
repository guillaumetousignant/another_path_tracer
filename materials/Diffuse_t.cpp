#include "Diffuse_t.h"
#include "Shape_t.h"
#include <cmath>
#include "RandomGenerator_t.h"

#define EPSILON 0.00001 // was 0.00001, 0.01 works ok when *t
#define PI 3.141592653589793238463

Diffuse_t::Diffuse_t(const Vec3f &emission, const Vec3f &colour, double roughness) : 
    emission_(emission), colour_(colour), roughness_(roughness), unif_(std::uniform_real_distribution<double>(0, 1)) {}

Diffuse_t::~Diffuse_t(){}

void Diffuse_t::bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) {
    Vec3f normal;

    hit_obj->normal(ray, uv, normal);

    const double rand1 = unif_(my_rand::rng)*2*PI;
    const double rand2 = unif_(my_rand::rng);
    const double rand2s = sqrt(rand2);

    if (normal.dot(ray.direction_) > 0.0){
        normal *= -1.0;
    }

    const Vec3f axis = std::abs(normal[0]) > 0.1 ? Vec3f(0.0, 1.0, 0.0) : Vec3f(1.0, 0.0, 0.0);

    const Vec3f u = axis.cross(normal).normalize();
    const Vec3f v = normal.cross(u).normalize(); // wasn't normalized before

    const Vec3f newdir = (u*cos(rand1)*rand2s + v*sin(rand1)*rand2s + normal*sqrt(1.0-rand2)).normalize();

    ray.origin_ += ray.direction_ * ray.dist_ + normal * EPSILON;//*ray.dist_; // Made EPSILON relative, check // well guess what wasn,t a goood idea
    ray.direction_ = newdir;

    ray.colour_ += ray.mask_ * emission_;
    ray.mask_ *= colour_ * pow(newdir.dot(normal), roughness_);
}