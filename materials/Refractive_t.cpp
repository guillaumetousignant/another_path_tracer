#include "Refractive_t.h"
#include "Shape_t.h"
#include <cmath>

#define EPSILON 0.00000001

Refractive_t::Refractive_t(const Vec3f &emission, const Vec3f &colour, double ind, unsigned int priority, ScatteringFunction_t* scattering) : 
    Medium_t(ind, priority, scattering), emission_(emission), colour_(colour) {}

Refractive_t::~Refractive_t(){}

void Refractive_t::bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) {
    Vec3f normal;
    Vec3f newdir;

    hit_obj->normal(ray, uv, normal);
    double cosi = ray.direction_.dot(normal);

    if (priority_ >= ray.medium_list_.front()->priority_){ // CHECK also discard if priority is equal, but watch for going out case
        Vec3f n;
        double etai, etat;

        if (cosi < 0){ // Coming in
            etai = ray.medium_list_.front()->ind_;
            etat = ind_;
            cosi *= -1;
            n = normal;
        }
        else{ // Goind out
            etat = (*std::next(ray.medium_list_.begin()))->ind_;
            etai = ind_;
            n = -normal;
        }

        const double eta = etai/etat;
        const double k = 1.0 - eta*eta * (1.0 - cosi*cosi);

        //newdir = k < 0 ? Vec3f() : (ray.direction_*eta + n * (eta*cosi - std::sqrt(k))).normalize();
        newdir = k < 0.0 ? (ray.direction_ - n * 2.0 * cosi).normalize() : (ray.direction_*eta + n * (eta*cosi - std::sqrt(k))).normalize(); // Now reflects completely if k < 0
        // set to 0, 0, 0? maybe mask to 0, 0, 0 also? 
        // Normalize newdir?

        ray.colour_ += ray.mask_ * emission_;
        ray.mask_ *= colour_;
    }
    else{
        newdir = ray.direction_;
    }

    if (newdir.dot(normal) < 0.0){ // Coming in
        ray.origin_ += ray.direction_ * ray.dist_ - normal * EPSILON; // n or normal?
        if (ray.direction_.dot(normal) < 0.0){
            ray.add_to_mediums(this);
        }
    }
    else{ // Going out
        ray.origin_ += ray.direction_ * ray.dist_ + normal * EPSILON; // n or normal?
        ray.remove_from_mediums(this);
    }

    ray.direction_ = newdir;
}