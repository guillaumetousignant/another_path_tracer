#include "skyboxes/SkyboxTextureTransformation_t.hpp"
#include "entities/Texture_t.hpp"
#include "entities/TransformMatrix_t.hpp"
#include <array>

constexpr double pi = 3.141592653589793238463;

using APTracer::Entities::Vec3f;

APTracer::Skyboxes::SkyboxTextureTransformation_t::SkyboxTextureTransformation_t(APTracer::Entities::Texture_t* texture, APTracer::Entities::TransformMatrix_t* transformation) :
        texture_(texture), transformation_(transformation) {}

auto APTracer::Skyboxes::SkyboxTextureTransformation_t::get(const Vec3f& xyz) const -> Vec3f {
    const Vec3f sph = transformation_->multDir(xyz).to_sph();
    const std::array<double, 2> xy{sph[2] / (2.0 * pi) + 0.5, 1.0 - sph[1] / pi};
    return texture_->get(xy);
}