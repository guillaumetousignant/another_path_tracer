#include "skyboxes/SkyboxFlatSun_t.h"
#include "entities/DirectionalLight_t.h"

using APTracer::Entities::Vec3f;

APTracer::Skyboxes::SkyboxFlatSun_t::SkyboxFlatSun_t(const Vec3f &background, APTracer::Entities::DirectionalLight_t* light) : background_(background), n_lights_(1) {
    lights_ = new APTracer::Entities::DirectionalLight_t*[n_lights_];
    lights_[0] = light;
} 

APTracer::Skyboxes::SkyboxFlatSun_t::SkyboxFlatSun_t(const Vec3f &background, APTracer::Entities::DirectionalLight_t** lights, size_t n_lights) : background_(background), n_lights_(n_lights) {
    lights_ = new APTracer::Entities::DirectionalLight_t*[n_lights_];
    for (size_t i = 0; i < n_lights_; i++) {
        lights_[i] = lights[i];
    }
}

APTracer::Skyboxes::SkyboxFlatSun_t::~SkyboxFlatSun_t() {
    delete [] lights_;
}

Vec3f APTracer::Skyboxes::SkyboxFlatSun_t::get(const Vec3f &xyz) const {
    Vec3f colour = Vec3f();
    for (size_t i = 0; i < n_lights_; i++) {
        if (xyz.dot(lights_[i]->direction_) < -lights_[i]->radius_) {
            colour += lights_[i]->intensity_;
        }
    }
    if (colour == Vec3f()) {
        colour = background_;
    }
    return colour;
}