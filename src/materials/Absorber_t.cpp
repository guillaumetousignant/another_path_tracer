#include "materials/Absorber_t.hpp"
#include <cmath>

APTracer::Materials::Absorber_t::Absorber_t(Entities::Vec3f emi_vol, Entities::Vec3f col_vol, double abs_dist_emi, double abs_dist_col, double ind, unsigned int priority) : Medium_t(ind, priority) {
    colour_vol_   = -col_vol.ln() / abs_dist_col;
    emission_vol_ = emi_vol * emi_vol / abs_dist_emi; // CHECK probably not right.
}

auto APTracer::Materials::Absorber_t::scatter(APTracer::Entities::Ray_t& ray) -> bool {
    // ray.colour = ray.colour + ray.mask .* exp(-obj.emission_vol * ray.dist);
    ray.colour_ += ray.mask_ * (emission_vol_ * ray.dist_).sqrt(); // sqrt may be slow
    ray.mask_ *= (-colour_vol_ * ray.dist_).exp();
    return false;
}