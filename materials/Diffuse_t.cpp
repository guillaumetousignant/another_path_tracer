#include "Diffuse_t.h"
#include "Shape_t.h"
#include <math.h>
#include "RandomGenerator_t.h"

//#include <iostream> // REMOVE

#define EPSILON 0.00001
#define PI 3.141592653589793238463

Diffuse_t::Diffuse_t(Vec3f emission, Vec3f colour, double roughness) : 
    emission_(emission), colour_(colour), roughness_(roughness), unif_(std::uniform_real_distribution<double>(0, 1)) {}

Diffuse_t::~Diffuse_t(){

}

void Diffuse_t::bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) {
    Vec3f axis;
    Vec3f normal;
    Vec3f u, v;
    Vec3f newdir;

    hit_obj->normal(ray, uv, normal);

    double rand1 = unif_(my_rand::rng);
    double rand2 = unif_(my_rand::rng);
    double rand2s = sqrt(rand2);

    if (normal.dot(ray.direction_) > 0){
        normal *= -1;
    }

    if (normal[0] > 0.1){
        axis = Vec3f(0, 1, 0);
    }
    else{
        axis = Vec3f(1, 0, 0);
    }

    u = axis.cross(normal).normalize(); // Maybe won't work
    v = normal.cross(u);

    newdir = u * cos(rand1)*rand2s + v*sin(rand1)*rand2s + normal*sqrt(1-rand2);
    newdir = newdir.normalize();

    ray.origin_ = ray.origin_ + ray.direction_ * ray.dist_ + normal * EPSILON;
    ray.direction_ = newdir;

    ray.colour_ += ray.mask_ * emission_;
    ray.mask_ *= colour_ * pow(newdir.dot(normal), roughness_);

    //std::cout << ray.mask_[0] << " " << ray.mask_[1] << " " << ray.mask_[2] << std::endl; // REMOVE
}
