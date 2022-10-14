#include "skyboxes/SkyboxTextureSun_t.h"
#include "entities/Texture_t.h"
#include <array>
#include <cmath>

constexpr double pi = 3.141592653589793238463;

using APTracer::Entities::Vec3f;

APTracer::Skyboxes::SkyboxTextureSun_t::SkyboxTextureSun_t(APTracer::Entities::Texture_t* texture, std::array<double, 2> sun_pos, Vec3f sun_col, double sun_rad) :
        texture_(texture), sun_pos_{sun_pos}, sun_col_(sun_col), sun_rad_(sun_rad) {}

auto APTracer::Skyboxes::SkyboxTextureSun_t::get(const Vec3f& xyz) const -> Vec3f {
    const Vec3f sph = xyz.get_sph();
    const std::array<double, 2> xy{sph[2] / (2.0 * pi) + 0.5, 1.0 - sph[1] / pi};

    if ((std::pow(xy[0] - sun_pos_[0], 2) + std::pow(xy[1] - sun_pos_[1], 2)) < std::pow(sun_rad_, 2)) {
        return sun_col_;
    }
    else {
        return texture_->get(xy);
    }
}