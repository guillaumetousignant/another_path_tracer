#include "materials/ScattererExp_t.h"
#include <cmath>
#include "entities/RandomGenerator_t.h"

constexpr double pi = 3.141592653589793238463;

using APTracer::Entities::Vec3f;

APTracer::Materials::ScattererExp_t::ScattererExp_t(Vec3f emi_vol, Vec3f col_vol, double abs_dist_emi, double abs_dist_col, double scat_dist, double order, double scattering_angle, double ind, unsigned int priority) 
        : Medium_t(ind, priority), order_(order), scattering_angle_(scattering_angle), unif_(0.0, 1.0) {
    colour_vol_ = -col_vol.ln()/abs_dist_col;
    emission_vol_ = emi_vol*emi_vol/abs_dist_emi; // CHECK probably not right.
    scattering_coefficient_ = 1.0/scat_dist;
}

APTracer::Materials::ScattererExp_t::~ScattererExp_t() {}

bool APTracer::Materials::ScattererExp_t::scatter(APTracer::Entities::Ray_t &ray) {
    const double distance = -std::log(unif_(APTracer::Entities::rng))/scattering_coefficient_;
    bool intersected = false;
    if (distance < ray.dist_) {
        intersected = true;
        ray.dist_ = distance;
        ray.origin_ += ray.direction_ * distance;

        const double rand1 = unif_(APTracer::Entities::rng) * 2.0 * pi;
        const double rand2 = std::pow(unif_(APTracer::Entities::rng), order_) * scattering_angle_ * pi;

        const Vec3f axis = ray.direction_[0] > 0.1 ? Vec3f(0.0, 1.0, 0.0) : Vec3f(1.0, 0.0, 0.0);

        const Vec3f u = axis.cross(ray.direction_).normalize_inplace();
        const Vec3f v = ray.direction_.cross(u).normalize_inplace();

        const Vec3f direction_fuzz = (u * std::cos(rand1) * std::sin(rand2) + v * std::sin(rand1) * std::sin(rand2) + ray.direction_ * std::cos(rand2)).normalize_inplace();

        ray.direction_ = direction_fuzz; //is this [normalize] needed? prob because of the cross() calls.
    }

    ray.colour_ += ray.mask_ * (emission_vol_ * ray.dist_).sqrt(); // sqrt may be slow
    ray.mask_ *= (-colour_vol_ * ray.dist_).exp();
    return intersected;
}