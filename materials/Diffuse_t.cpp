#include "Diffuse_t.h"
#include "Shape_t.h"
#include <math.h>
#include "RandomGenerator_t.h"
#include "Referentials.h"

#include <iostream> // REMOVE

#define EPSILON 0.00001 // was 0.00001
#define PI 3.141592653589793238463

Diffuse_t::Diffuse_t(const Vec3f &emission, const Vec3f &colour, double roughness) : 
    emission_(emission), colour_(colour), roughness_(roughness), unif_(std::uniform_real_distribution<double>(0, 1)) {}

Diffuse_t::~Diffuse_t(){}

void Diffuse_t::bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) {
    Vec3f axis;
    Vec3f normal;
    Vec3f u, v;
    Vec3f newdir;

    hit_obj->normal(ray, uv, normal);

    double rand1 = unif_(my_rand::rng);
    double rand2 = unif_(my_rand::rng);
    double rand2s = sqrt(rand2);

    //std::cout << "Normal: " << normal[0] << " " << normal[1] << " " << normal[2] << std::endl; // REMOVE

    if (normal.dot(ray.direction_) > 0.0){
        normal *= -1.0;
    }

    if (normal[0] > 0.1){ // This seems like crap, was 0.1
        axis = Vec3f(0.0, 1.0, 0.0);
        //std::cout << "AAAAAAAAAAAA" << std::endl; // REMOVE
    }
    else{
        axis = Vec3f(1.0, 0.0, 0.0);
    }

    u = axis.cross(normal).normalize(); // Maybe won't work
    v = normal.cross(u).normalize(); // wasn't normalized before

    //std::cout << "u: " << u[0] << " " << u[1] << " " << u[2] << std::endl; // REMOVE
    //std::cout << "v: " << v[0] << " " << v[1] << " " << v[2] << std::endl; // REMOVE
    //std::cout <<"rand 1 2 2s: " <<  rand1 << " " << rand2 << " " << rand2s << std::endl; // REMOVE

    newdir = u*cos(rand1)*rand2s + v*sin(rand1)*rand2s + normal*sqrt(1.0-rand2);
    newdir = newdir.normalize();

    //newdir = to_xyz_offset(Vec3f(1, rand1, rand2), normal, u, v);

    ray.origin_ += ray.direction_ * ray.dist_ + normal * EPSILON;
    ray.direction_ = newdir;

    //std::cout << newdir[0] << " " << newdir[1] << " " << newdir[2] << std::endl; // REMOVE

    ray.colour_ += ray.mask_ * emission_;
    ray.mask_ *= colour_ * pow(newdir.dot(normal), roughness_);
}
