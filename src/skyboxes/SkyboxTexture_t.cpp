#include "skyboxes/SkyboxTexture_t.hpp"
#include <array>

constexpr double pi = 3.141592653589793238463;

using APTracer::Entities::Vec3f;

APTracer::Skyboxes::SkyboxTexture_t::SkyboxTexture_t(APTracer::Entities::Texture_t* texture) : texture_(texture) {}

auto APTracer::Skyboxes::SkyboxTexture_t::get(const Vec3f& xyz) const -> Vec3f {
    const Vec3f sph = xyz.get_sph();
    const std::array<double, 2> xy{sph[2] / (2.0 * pi) + 0.5, 1.0 - sph[1] / pi};
    return texture_->get(xy);
}