#include "skyboxes/SkyboxTexture_t.h"
#include "entities/Texture_t.h"

constexpr double pi = 3.141592653589793238463;

using APTracer::Entities::Vec3f;

APTracer::Skyboxes::SkyboxTexture_t::SkyboxTexture_t(APTracer::Entities::Texture_t* texture) : texture_(texture) {}

APTracer::Skyboxes::SkyboxTexture_t::~SkyboxTexture_t() = default;

Vec3f APTracer::Skyboxes::SkyboxTexture_t::get(const Vec3f &xyz) const {
    const Vec3f sph = xyz.get_sph();
    const double xy[2] = {sph[2]/(2.0 * pi) + 0.5, 1.0 - sph[1]/pi};
    return texture_->get(xy);
}