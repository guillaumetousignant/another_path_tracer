#include "materials/Diffuse_t.h"
#include "entities/Shape_t.h"
#include <cmath>
#include "entities/RandomGenerator_t.h"

#define EPSILON 0.00001 // was 0.00001, 0.01 works ok when *t
#define PI 3.141592653589793238463

using APTracer::Entities::Vec3f;

APTracer::Materials::Diffuse_t::Diffuse_t(const Vec3f &emission, const Vec3f &colour, double roughness) : 
    emission_(emission), colour_(colour), roughness_(roughness), unif_(std::uniform_real_distribution<double>(0, 1)) {}

APTracer::Materials::Diffuse_t::~Diffuse_t(){}

void APTracer::Materials::Diffuse_t::bounce(const double (&uv)[2], const APTracer::Entities::Shape_t* hit_obj, APTracer::Entities::Ray_t &ray) {
    Vec3f normal = hit_obj->normal(ray.time_, uv);

    const double rand1 = unif_(APTracer::Entities::rng)*2*PI;
    const double rand2 = unif_(APTracer::Entities::rng);
    const double rand2s = sqrt(rand2);

    if (normal.dot(ray.direction_) > 0.0){
        normal *= -1.0;
    }

    const Vec3f axis = std::abs(normal[0]) > 0.1 ? Vec3f(0.0, 1.0, 0.0) : Vec3f(1.0, 0.0, 0.0);

    const Vec3f u = axis.cross(normal).normalize_inplace();
    const Vec3f v = normal.cross(u).normalize_inplace(); // wasn't normalized before

    const Vec3f newdir = (u*cos(rand1)*rand2s + v*sin(rand1)*rand2s + normal*sqrt(1.0-rand2)).normalize_inplace();

    ray.origin_ += ray.direction_ * ray.dist_ + normal * EPSILON;//*ray.dist_; // Made EPSILON relative, check // well guess what wasn,t a goood idea
    ray.direction_ = newdir;

    ray.colour_ += ray.mask_ * emission_;
    ray.mask_ *= colour_ * pow(newdir.dot(normal), roughness_);
}