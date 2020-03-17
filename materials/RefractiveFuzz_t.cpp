#include "RefractiveFuzz_t.h"
#include "Shape_t.h"
#include "RandomGenerator_t.h"
#include <cmath>

#define EPSILON 0.00000001
#define PI 3.141592653589793238463

using APTracer::Materials::RefractiveFuzz_t;

RefractiveFuzz_t::RefractiveFuzz_t(const Vec3f &emission, const Vec3f &colour, double ind, unsigned int priority, double order, double diffusivity, ScatteringFunction_t* scattering) : 
    Medium_t(ind, priority, scattering), emission_(emission), colour_(colour), order_(order), diffusivity_(diffusivity), unif_(0.0, 1.0) {}

RefractiveFuzz_t::~RefractiveFuzz_t(){}

void RefractiveFuzz_t::bounce(const double (&uv)[2], const Shape_t* hit_obj, Ray_t &ray) {
    Vec3f normal;
    Vec3f newdir;

    hit_obj->normal(ray, uv, normal);
    double cosi = ray.direction_.dot(normal);

    if (priority_ >= ray.medium_list_.front()->priority_){ // CHECK also discard if priority is equal, but watch for going out case
        double etai, etat;

        const double rand1 = unif_(APTracer::Entities::rng)*2.0*PI;
        const double rand2 = std::pow(unif_(APTracer::Entities::rng), order_) * diffusivity_;
        const double rand2s = std::sqrt(rand2);

        const Vec3f axis = std::abs(normal[0]) > 0.1 ? Vec3f(0.0, 1.0, 0.0) : Vec3f(1.0, 0.0, 0.0);

        const Vec3f u = axis.cross(normal).normalize_inplace();
        const Vec3f v = normal.cross(u).normalize_inplace(); // wasn't normalized before

        Vec3f normal_fuzz = (u * std::cos(rand1)*rand2s + v*std::sin(rand1)*rand2s + normal*std::sqrt(1.0-rand2)).normalize_inplace();
        cosi = ray.direction_.dot(normal_fuzz);

        if (cosi < 0.0){ // Coming in
            etai = ray.medium_list_.front()->ind_;
            etat = ind_;
            cosi *= -1.0;
        }
        else{ // Going out
            etat = (*std::next(ray.medium_list_.begin()))->ind_;
            etai = ind_;
            normal_fuzz *= -1.0;
        }        

        const double eta = etai/etat;
        const double k = 1.0 - eta*eta * (1.0 - cosi*cosi);

        //newdir = k < 0 ? Vec3f() : (ray.direction_*eta + normal_fuzz * (eta*cosi - std::sqrt(k))).normalize_inplace();
        newdir = k < 0.0 ? (ray.direction_ - normal_fuzz * 2.0 * cosi).normalize_inplace() : (ray.direction_*eta + normal_fuzz * (eta*cosi - std::sqrt(k))).normalize_inplace();
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