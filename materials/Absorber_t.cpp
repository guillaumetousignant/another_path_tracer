#include "Absorber_t.h"
#include <cmath>

Absorber_t::Absorber_t(Vec3f emi_vol, Vec3f col_vol, double abs_dist_emi, double abs_dist_col) {
    colour_vol_ = -col_vol.ln()/abs_dist_col;
    emission_vol_ = emi_vol*emi_vol/abs_dist_emi; // CHECK probably not right.
}

Absorber_t::~Absorber_t() {}

bool Absorber_t::scatter(Ray_t &ray) {
    //aray.colour = aray.colour + aray.mask .* exp(-obj.emission_vol * aray.dist);
    ray.colour_ += ray.mask_ * (emission_vol_ * ray.dist_).sqrt(); // sqrt may be slow
    ray.mask_ *= (-colour_vol_ * ray.dist_).exp();
    return false;
}