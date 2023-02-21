#include "skyboxes/SkyboxFlat_t.hpp"

using APTracer::Entities::Vec3f;

APTracer::Skyboxes::SkyboxFlat_t::SkyboxFlat_t(const Vec3f& background) : background_(background) {}

auto APTracer::Skyboxes::SkyboxFlat_t::get(const Vec3f& /*xyz*/) const -> Vec3f {
    return background_;
}