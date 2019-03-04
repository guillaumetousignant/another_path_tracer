#include "SkyboxFlatSun_t.h"
#include "DirectionalLight_t.h"

SkyboxFlatSun_t::SkyboxFlatSun_t(const Vec3f &background, DirectionalLight_t* light) : background_(background), lights_(&light), n_lights_(1) {} 

SkyboxFlatSun_t::SkyboxFlatSun_t(const Vec3f &background, DirectionalLight_t** lights, unsigned int n_lights) : background_(background), lights_(lights), n_lights_(n_lights) {}

SkyboxFlatSun_t::~SkyboxFlatSun_t() {}

Vec3f SkyboxFlatSun_t::get(const Vec3f &xyz) const {
    Vec3f colour = Vec3f();
    for (unsigned int i = 0; i < n_lights_; i++){
        if (xyz.dot(lights_[i]->direction_) < -lights_[i]->radius_){
            colour += lights_[i]->intensity_;
        }
    }
    if (colour == Vec3f()){
        colour = background_;
    }
    return colour;
}