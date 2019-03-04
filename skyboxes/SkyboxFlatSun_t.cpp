#include "SkyboxFlatSun_t.h"
#include "DirectionalLight_t.h"

SkyboxFlatSun_t::SkyboxFlatSun_t(const Vec3f &background, DirectionalLight_t* light) : background_(background), n_lights_(1) {
    lights_ = new DirectionalLight_t*[n_lights_];
    lights_[0] = light;
} 

SkyboxFlatSun_t::SkyboxFlatSun_t(const Vec3f &background, DirectionalLight_t** lights, unsigned int n_lights) : background_(background), n_lights_(n_lights) {
    lights_ = new DirectionalLight_t*[n_lights_];
    for (unsigned int i = 0; i < n_lights_; i++){
        lights_[i] = lights[i];
    }
}

SkyboxFlatSun_t::~SkyboxFlatSun_t() {
    if (lights_ != nullptr){
        delete [] lights_;
    }
}

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