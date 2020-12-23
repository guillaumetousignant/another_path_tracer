#include "skyboxes/SkyboxFlat_t.h"

using APTracer::Entities::Vec3f;

APTracer::Skyboxes::SkyboxFlat_t::SkyboxFlat_t(const Vec3f &background) : background_(background) {}

APTracer::Skyboxes::SkyboxFlat_t::~SkyboxFlat_t() {}

Vec3f APTracer::Skyboxes::SkyboxFlat_t::get(const Vec3f &xyz) const {
    return background_;
}