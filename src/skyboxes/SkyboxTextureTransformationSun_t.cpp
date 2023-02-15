#include "skyboxes/SkyboxTextureTransformationSun_t.h"
#include "entities/Texture_t.h"
#include "entities/TransformMatrix_t.h"
#include <array>

constexpr double pi = 3.141592653589793238463;

using APTracer::Entities::Vec3f;

APTracer::Skyboxes::SkyboxTextureTransformationSun_t::SkyboxTextureTransformationSun_t(APTracer::Entities::Texture_t* texture, APTracer::Entities::TransformMatrix_t* transformation, std::array<double, 2> sun_pos, Vec3f sun_col, double sun_rad) 
    : texture_(texture), transformation_(transformation), sun_pos_(sun_pos), sun_col_(sun_col), sun_rad_(sun_rad) {}

auto APTracer::Skyboxes::SkyboxTextureTransformationSun_t::get(const Vec3f &xyz) const -> Vec3f {
    const Vec3f sph = transformation_->multDir(xyz).to_sph();
    const std::array<double, 2> xy{sph[2]/(2.0 * pi) + 0.5, 1.0 - sph[1]/pi};

    if ((std::pow(xy[0] - sun_pos_[0], 2) + std::pow(xy[1] - sun_pos_[1], 2)) < (std::pow(sun_rad_, 2))) {
        return sun_col_;
    }
    else {
        return texture_->get(xy);
    }
}