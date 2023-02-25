#include "skyboxes/SkyboxFlatSun_t.hpp"

using APTracer::Entities::Vec3f;

APTracer::Skyboxes::SkyboxFlatSun_t::SkyboxFlatSun_t(const Vec3f& background, APTracer::Entities::DirectionalLight_t* light) : background_(background), lights_{light} {}

APTracer::Skyboxes::SkyboxFlatSun_t::SkyboxFlatSun_t(const Vec3f& background, std::vector<APTracer::Entities::DirectionalLight_t*> lights) : background_(background), lights_(std::move(lights)) {}

auto APTracer::Skyboxes::SkyboxFlatSun_t::get(const Vec3f& xyz) const -> Vec3f {
    Vec3f colour = Vec3f();
    for (auto* light: lights_) {
        if (xyz.dot(light->direction_) < -light->radius_) {
            colour += light->intensity_;
        }
    }
    if (colour == Vec3f()) {
        colour = background_;
    }
    return colour;
}